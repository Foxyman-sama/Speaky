#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include <boost/property_tree/json_parser.hpp>
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/ptree_fwd.hpp>

#include "src/chat.hpp"

using namespace testing;

int main() {
  InitGoogleTest();
  InitGoogleMock();
  return RUN_ALL_TESTS();
}

TEST(package_tests, convert_to_json) {
  boost::property_tree::ptree expected;
  expected.put("room_id", "1");
  expected.put("username", "Dima");

  RegisterPackage package { "1", "Dima" };

  const auto actual { package.make_json() };

  ASSERT_EQ(expected, actual);
}

TEST(package_tests, convert_to_struct_from_json) {
  const RegisterPackage expected { "1", "Dima" };

  boost::property_tree::ptree ptree;
  ptree.put("room_id", "1");
  ptree.put("username", "Dima");

  const RegisterPackage actual { ptree };

  ASSERT_EQ(expected, actual);
}