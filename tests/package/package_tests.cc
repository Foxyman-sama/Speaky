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
  RegisterPackage(const std::string& room_id, const std::string& username)
      : room_id { room_id }, username { username } {}

  RegisterPackage(const boost::property_tree::ptree& ptree)
      : room_id { ptree.get<std::string>("room_id") }, username { ptree.get<std::string>("username") } {}

  boost::property_tree::ptree make_json() {
    boost::property_tree::ptree result;
    result.put("room_id", room_id);
    result.put("username", username);
    return result;
  }

  constexpr bool operator==(const RegisterPackage& other) const noexcept {
    return room_id == other.room_id && username == other.username;
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

TEST(package_tests, convert_to_struct_from_json) {
  const RegisterPackage expected { "1", "Dima" };

  boost::property_tree::ptree ptree;
  ptree.put("room_id", "1");
  ptree.put("username", "Dima");

  const RegisterPackage actual { ptree };

  ASSERT_EQ(expected, actual);
}