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

class ParticipantFromGadgets : public Participant {
 public:
  explicit ParticipantFromGadgets(const std::string& name, boost::asio::ip::tcp::socket&& socket)
      : Participant { name }, socket { std::move(socket) } {}

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