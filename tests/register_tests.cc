#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include "src/chat.hpp"

using namespace testing;

int main() {
  InitGoogleTest();
  InitGoogleMock();
  return RUN_ALL_TESTS();
}

class register_tests : public Test, public Chat {};

TEST_F(register_tests, register_new_participant) {
  constexpr int test_id { 1 };
  const std::string test_name { "Dima" };

  ASSERT_FALSE(is_there_room(test_id));

  auto user { std::make_shared<User>(test_id, test_name) };
  ASSERT_EQ(test_id, user->get_room_id());
  ASSERT_EQ(test_name, user->get_name());

  create_room(test_id);
  ASSERT_TRUE(is_there_room(test_id));
  ASSERT_EQ(test_id, rooms[test_id]->get_room_id());

  register_user(test_id, user);
  ASSERT_EQ(1, rooms[test_id]->get_number_of_users());
  ASSERT_TRUE(rooms[test_id]->is_joined(user));
}