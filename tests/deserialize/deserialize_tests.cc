#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include <string>

#include "proto/message.pb.h"
#include "proto/new_connection.pb.h"
#include "src/deserialize.h"
#include "src/proto_classes_wrapper.h"

using namespace testing;
using namespace speaky;

int main() {
  InitGoogleTest();
  InitGoogleMock();
  return RUN_ALL_TESTS();
}

class deserialize_tests : public Test {};

TEST_F(deserialize_tests, deserialize_message) {
  constexpr const char* const test_message { "Hello, World!" };

  speaky::Message message;
  message.set_message(test_message);

  const auto serialized { message.SerializeAsString() };

  const auto actual { deserialize<MessageProto>(serialized) };

  ASSERT_EQ(message.message(), actual.get_message());
}

TEST_F(deserialize_tests, deserialize_new_connection) {
  constexpr const char* const test_name { "Igor" };
  constexpr int test_room_id { 0 };

  speaky::NewConnection new_conn;
  new_conn.set_name(test_name);
  new_conn.set_room_id(test_room_id);

  const auto serialized { new_conn.SerializeAsString() };

  const auto actual { deserialize<NewConnectionProto>(serialized) };

  ASSERT_EQ(new_conn.name(), actual.get_name());
  ASSERT_EQ(new_conn.room_id(), actual.get_room_id());
}