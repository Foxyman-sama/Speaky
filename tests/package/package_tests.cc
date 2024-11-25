#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include <boost/property_tree/json_parser.hpp>
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/ptree_fwd.hpp>

#include "boost/asio/io_context.hpp"
#include "src/chat.hpp"

using namespace testing;

int main() {
  InitGoogleTest();
  InitGoogleMock();
  return RUN_ALL_TESTS();
}

struct RegisterPackage {
  boost::property_tree::ptree make_json() {
    boost::property_tree::ptree result;
    result.put("room_id", room_id);
    result.put("username", username);
    return result;
  }

  std::string room_id;
  std::string username;
};

TEST(package_tests, convert_to_json) {
  boost::property_tree::ptree expected;
  expected.put("room_id", "1");
  expected.put("username", "Dima");

  RegisterPackage package { "1", "Dima" };

  const auto actual { package.make_json() };

  ASSERT_EQ(expected, actual);
}