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
  User(int room_id, std::string name) : room_id { room_id }, name { name } {}

  int get_room_id() { return room_id; }

  std::string get_name() { return name; }

 private:
  int room_id;
  std::string name;
};

class Room {
 public:
  Room(int room_id) : room_id { room_id } {}

  int get_number_of_users() { return users.size(); }

  bool is_joined(std::shared_ptr<User> user) { return users.contains(user); }

  int get_room_id() { return room_id; }

  void register_user(std::shared_ptr<User> user) { users.emplace(user); }

 private:
  int room_id;
  std::set<std::shared_ptr<User>> users;
};

class Chat {
 public:
  bool is_there_room(int room_id) { return rooms.contains(room_id); }

  void create_room(int room_id) { rooms[room_id] = std::make_shared<Room>(room_id); }

  void register_user(int room_id, std::shared_ptr<User> user) { rooms[room_id]->register_user(user); }

 protected:
  std::unordered_map<int, std::shared_ptr<Room>> rooms;
};

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