#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include "Packages/packages.h"
#include "boost/asio/io_context.hpp"
#include "src/chat.hpp"

using namespace testing;

int main() {
  InitGoogleTest();
  InitGoogleMock();
  return RUN_ALL_TESTS();
}

class RoomMock : public Room {
 public:
  RoomMock(int room_id) : Room { room_id } {}

  void add_user_message(MessagePackage user_message) { chat_history.push_back(user_message); }
};

class UserMock : public User {
 public:
  UserMock(int room_id, std::string name) : User { room_id, name } {}

  MOCK_METHOD(void, send, (MessagePackage), (override));
  MOCK_METHOD(void, send, (StatusPackage), (override));
};

class register_tests : public Test, public Chat<RoomMock> {};

TEST_F(register_tests, register_new_user) {
  constexpr int test_id { 1 };
  const std::string test_name { "Dima" };

  ASSERT_FALSE(is_there_room(test_id));

  auto user { std::make_shared<UserMock>(test_id, test_name) };
  ASSERT_EQ(test_id, user->get_room_id());
  ASSERT_EQ(test_name, user->get_name());

  create_room(test_id);
  ASSERT_TRUE(is_there_room(test_id));
  ASSERT_EQ(test_id, rooms[test_id]->get_room_id());

  register_user(test_id, user);
  ASSERT_EQ(1, rooms[test_id]->get_number_of_users());
  ASSERT_TRUE(rooms[test_id]->is_joined(user));

  const MessagePackage user_messages[3] {
    MessagePackage { test_name, "Hello, World!" },
    MessagePackage { "Ignat", "Hello, World!2" },
    MessagePackage { "Amir", "Hello, World!3" },
  };

  for (auto&& user_message : user_messages) {
    EXPECT_CALL(*user, send(user_message));
    rooms[test_id]->add_user_message(user_message);
  }

  rooms[test_id]->send_chat_history(user);

  EXPECT_TRUE(Mock::VerifyAndClearExpectations(user.get()));
}