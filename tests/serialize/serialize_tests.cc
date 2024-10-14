#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include <string>

#include "proto_classes_wrapper.h"
#include "serialize.h"

using namespace testing;
using namespace speaky;

int main() {
  InitGoogleTest();
  InitGoogleMock();
  return RUN_ALL_TESTS();
}

class serialize_tests : public Test {};

TEST_F(serialize_tests, serialize_message) {
  constexpr const char* const test_message { "Hello, World!" };

  speaky::Message message;
  message.set_message(test_message);

  const auto actual { serialize<MessageProto>(test_message) };

  ASSERT_EQ(message.SerializeAsString(), actual);
}

TEST_F(serialize_tests, serialize_new_connection) {
  constexpr const char* const test_name { "Igor" };
  constexpr int test_room_id { 0 };

  speaky::NewConnection new_conn;
  new_conn.set_name(test_name);
  new_conn.set_room_id(test_room_id);

  const auto actual { serialize<NewConnectionProto>({ test_name, test_room_id }) };

  ASSERT_EQ(new_conn.SerializeAsString(), actual);
}