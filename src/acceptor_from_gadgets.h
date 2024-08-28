#ifndef ACCEPTOR_FROM_GADGETS_H
#define ACCEPTOR_FROM_GADGETS_H

#include <boost/asio.hpp>
#include <boost/asio/impl/read_until.hpp>
#include <boost/asio/io_context.hpp>
#include <boost/asio/ip/address_v4.hpp>
#include <boost/asio/streambuf.hpp>
#include <memory>

#include "deserialize.h"
#include "input.h"
#include "participant.h"
#include "proto_classes_wrapper.h"

namespace speaky {

class ParticipantFromGadgets : public Participant {
 public:
  explicit ParticipantFromGadgets(const std::string& name, boost::asio::ip::tcp::socket&& socket)
      : Participant { name }, socket { std::move(socket) } {}

  virtual ~ParticipantFromGadgets() {}

  void deliver(const std::string& message) override {};

 private:
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
      boost::asio::streambuf buf;
      boost::asio::read_until(socket, buf, "@");

      std::istream is { &buf };
      std::string serialized_data;
      std::getline(is, serialized_data, '@');

      auto new_conn { deserialize<NewConnectionProto>(serialized_data) };
      auto participant { std::make_shared<ParticipantFromGadgets>(new_conn.get_name(), std::move(socket)) };
      input.register_user(new_conn.get_room_id(), participant);
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