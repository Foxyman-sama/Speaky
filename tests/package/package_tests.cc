#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include "boost/asio/io_context.hpp"
#include "src/chat.hpp"

using namespace testing;

int main() {
  InitGoogleTest();
  InitGoogleMock();
  return RUN_ALL_TESTS();
}
