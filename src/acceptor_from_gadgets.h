#ifndef ACCEPTOR_FROM_GADGETS_H
#define ACCEPTOR_FROM_GADGETS_H

#include <boost/asio.hpp>
#include <exception>
#include <future>
#include <istream>
#include <memory>
#include <optional>
#include <print>
#include <stdexcept>
#include <string>
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

class ParticipantFromGadgets : public Participant {
 public:
  explicit ParticipantFromGadgets(const std::string& name, boost::asio::ip::tcp::socket&& socket)
      : Participant { name }, socket { std::move(socket) } {}

  void send(const std::string& message) {}

 private:
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
      auto socket { acceptor.accept() };
      auto request { read_request_from_socket(socket) };
      auto participant { make_connection(std::move(socket), request) };
      input.register_user(request.get_room_id(), participant);
    }
  }

 private:
  NewConnectionProto read_request_from_socket(boost::asio::ip::tcp::socket& socket) {
    const auto data { read_raw_data_from_socket(socket) };
    return deserialize<NewConnectionProto>(data);
  }

  std::string read_raw_data_from_socket(boost::asio::ip::tcp::socket& socket) {
    boost::asio::streambuf buf;
    boost::asio::read_until(socket, buf, delim);

    std::string result;
    std::istream is { &buf };
    std::getline(is, result, delim);

    return result;
  }

  ParticipantPtr make_connection(boost::asio::ip::tcp::socket&& socket, NewConnectionProto& nc) {
    return std::make_shared<ParticipantFromGadgets>(nc.get_name(), std::move(socket));
  }

  static constexpr int max_connections_wait { 30 };

  Input input;
  boost::asio::io_context io_context;
  boost::asio::ip::tcp::acceptor acceptor;
};

}  // namespace speaky

#endif