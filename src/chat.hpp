#ifndef CHAT_HPP
#define CHAT_HPP

#include <memory>
#include <set>
#include <string>
#include <unordered_map>

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

#endif