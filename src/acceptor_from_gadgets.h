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
      boost::asio::streambuf buf;
      boost::asio::read_until(socket, buf, delim);

      std::string temp;
      std::istream is { &buf };
      std::getline(is, temp, delim);

      auto new_connection { deserialize<NewConnectionProto>(temp) };
      auto participant { std::make_shared<ParticipantFromGadgets>(new_connection.get_name(), std::move(socket)) };
      input.register_user(new_connection.get_room_id(), participant);
    }
  }

 private:
  static constexpr int max_connections_wait { 30 };

  Input input;
  boost::asio::io_context io_context;
  boost::asio::ip::tcp::acceptor acceptor;
};

}  // namespace speaky

#endif