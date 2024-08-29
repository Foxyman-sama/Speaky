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
#include <variant>

#include "deserialize.h"
#include "input.h"
#include "participant.h"
#include "proto_classes_wrapper.h"

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

  void deliver(const std::string& message) override { boost::asio::write(socket, boost::asio::buffer(message)); };

  void read() {
    try {
      try_read();
    } catch (const std::exception& e) {
      // TODO: disconnect
      std::print("{0}", e.what());
    }
  }

 private:
  void try_read() {
    for (;;) {
      auto serialized_data { read_until(socket, delim) };
      auto message { deserialize<MessageProto>(serialized_data) };
      notify(message.get_message());
    }
  }

  boost::asio::ip::tcp::socket socket;
};

class AcceptorFromGadgets {
 public:
  AcceptorFromGadgets(unsigned short port) : acceptor { io_context, boost::asio::ip::tcp::v4() } {
    acceptor.set_option(boost::asio::ip::tcp::acceptor::reuse_address(true));
    acceptor.bind(boost::asio::ip::tcp::endpoint { boost::asio::ip::address_v4::any(), port });
    acceptor.listen(max_connections_wait);
  }

  void accept() {
    for (;;) {
      auto socket { acceptor.accept() };
      auto user_info { acceptor_info(socket) };
      make_connection(std::move(socket), user_info);
    }
  }

 private:
  NewConnectionProto acceptor_info(boost::asio::ip::tcp::socket& socket) {
    auto serialized_data { read_until(socket, delim) };
    return deserialize<NewConnectionProto>(serialized_data);
  }

  void make_connection(boost::asio::ip::tcp::socket&& socket, const NewConnectionProto& user_info) {
    auto participant { std::make_shared<ParticipantFromGadgets>(user_info.get_name(), std::move(socket)) };
    input.register_user(user_info.get_room_id(), participant);

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