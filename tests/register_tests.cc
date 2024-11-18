#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include <memory>
#include <string_view>
#include <unordered_map>

using namespace testing;

int main() {
  InitGoogleTest();
  InitGoogleMock();
  return RUN_ALL_TESTS();
}

class User {
 public:
  User(int room_id, std::string name) {}

  int get_chat_id() { return 0; }

  std::string get_name() { return ""; }
};

class Room {
 public:
  int get_number_of_users() { return 0; }

  bool is_joined(std::shared_ptr<User> user) { return true; }
};

class Chat {
 public:
  bool is_there_room(int room_id) { return true; }

  void create_room(int room_id) {}

  void register_user(std::shared_ptr<User> user) {}

 protected:
  std::unordered_map<int, std::shared_ptr<Room>> rooms;
};

class register_tests : public Test, public Chat {};

TEST_F(register_tests, register_new_participant) {
  constexpr int test_id { 1 };
  const std::string test_name { "Dima" };

  ASSERT_FALSE(is_there_room(test_id));

  auto user { std::make_shared<User>(test_id, test_name) };
  ASSERT_EQ(test_id, user->get_chat_id());
  ASSERT_EQ(test_name, user->get_name());

  create_room(test_id);
  ASSERT_TRUE(is_there_room(test_id));

  register_user(user);
  ASSERT_EQ(1, rooms[test_id]->get_number_of_users());
  ASSERT_TRUE(rooms[test_id]->is_joined(user));
}