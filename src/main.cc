#include <print>

#include "boost/asio/io_context.hpp"
#include "chat.hpp"

int main(int argc, char** argv) {
  boost::asio::io_context context;
  Chat chat { 9090, context };
  chat.start();
}