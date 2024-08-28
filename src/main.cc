#include <print>

#include "acceptor_from_gadgets.h"

int main(int argc, char** argv) {
  std::print("Hello, World!\n");
  speaky::AcceptorFromGadgets acceptor { 9090 };
  acceptor.accept();
}