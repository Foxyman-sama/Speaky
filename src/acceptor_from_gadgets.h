#ifndef ACCEPTOR_FROM_GADGETS_H
#define ACCEPTOR_FROM_GADGETS_H

#include <boost/asio.hpp>
#include <boost/asio/error.hpp>
#include <boost/asio/impl/read_until.hpp>
#include <boost/asio/io_context.hpp>
#include <boost/asio/ip/address_v4.hpp>
#include <boost/asio/registered_buffer.hpp>
#include <boost/asio/streambuf.hpp>
#include <boost/asio/write.hpp>
#include <boost/system/detail/error_code.hpp>
#include <exception>
#include <future>
#include <memory>
#include <optional>
#include <print>
#include <stdexcept>
#include <thread>
#include <utility>
#include <variant>

#include "deserialize.h"
#include "input.h"
#include "participant.h"
#include "proto_classes_wrapper.h"
#include "src/serialize.h"

namespace speaky {

constexpr char delim { '@' };

inline std::string extract_from_buffer_until(boost::asio::streambuf&& buf, char delim);

inline std::string read_until(boost::asio::ip::tcp::socket& socket, char delim) {
  boost::system::error_code ec;
  boost::asio::streambuf buf;
  boost::asio::read_until(socket, buf, delim, ec);

  if (ec) {
    throw std::runtime_error { ec.what() };
  }

  return extract_from_buffer_until(std::move(buf), delim);
}

inline std::string extract_from_buffer_until(boost::asio::streambuf&& buf, char delim) {
  std::istream is { &buf };
  std::string serialized_data;
  std::getline(is, serialized_data, delim);
  return serialized_data;
}

class ParticipantFromGadgets : public Participant {
 public:
  explicit ParticipantFromGadgets(const std::string& name, boost::asio::ip::tcp::socket&& socket)
      : Participant { name }, socket { std::move(socket) } {}

  void send(const std::string& message) override {
    auto serialized_message { serialize_message(message) };
    do_send(serialized_message);
  };

  void read() {
    try {
      try_read();
    } catch (const std::exception& e) {
      disconnect();
      std::print("User: [{0}] has been disconnected. Reason: [{1}]\n", name, e.what());
    }
  }

 private:
  std::string serialize_message(const std::string& message) { return serialize<MessageProto>(message) + delim; }

  void do_send(const std::string& message) { boost::asio::write(socket, boost::asio::buffer(message)); }

  void try_read() {
    for (;;) {
      auto serialized_data { read_until(socket, delim) };
      auto message { deserialize<MessageProto>(serialized_data) };
      notify(message.get_message());
    }
  }

  boost::asio::ip::tcp::socket socket;
};

using ParticipantFromGadgetsPtr = std::shared_ptr<ParticipantFromGadgets>;

class AcceptorFromGadgets {
 public:
  AcceptorFromGadgets(unsigned short port) : acceptor { io_context, boost::asio::ip::tcp::v4() } {
    using namespace boost::asio::ip;

    acceptor.set_option(tcp::acceptor::reuse_address(true));
    acceptor.bind(tcp::endpoint { address_v4::any(), port });
    acceptor.listen(max_connections_wait);
  }

  void accept() {
    for (;;) {
      handle_new_connections();
    }
  }

 private:
  void handle_new_connections() {
    auto&& [socket, user_info] { accept_new_connection() };
    make_participant(std::move(socket), user_info);
  }

  std::pair<boost::asio::ip::tcp::socket, NewConnectionProto> accept_new_connection() {
    auto socket { acceptor.accept() };
    return { std::move(socket), read_user_info(socket) };
  }

  NewConnectionProto read_user_info(boost::asio::ip::tcp::socket& socket) {
    auto serialized_data { read_until(socket, delim) };
    return deserialize<NewConnectionProto>(serialized_data);
  }

  void make_participant(boost::asio::ip::tcp::socket&& socket, const NewConnectionProto& user_info) {
    auto participant { connect_socket_with_participant(std::move(socket), user_info) };
    input.register_user(user_info.get_room_id(), participant);
    make_new_handle_thread(participant);
  }

  ParticipantFromGadgetsPtr connect_socket_with_participant(boost::asio::ip::tcp::socket&& socket,
                                                            const NewConnectionProto& user_info) {
    return std::make_shared<ParticipantFromGadgets>(user_info.get_name(), std::move(socket));
  }

  void make_new_handle_thread(ParticipantFromGadgetsPtr participant) {
    std::thread th { [participant]() { participant->read(); } };
    th.detach();
  }

  static constexpr int max_connections_wait { 30 };

  Input input;
  boost::asio::io_context io_context;
  boost::asio::ip::tcp::acceptor acceptor;
};

}  // namespace speaky

#endif