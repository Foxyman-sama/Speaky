#ifndef CHAT_HPP
#define CHAT_HPP

#include <boost/asio.hpp>
#include <boost/property_tree/ptree.hpp>
#include <deque>
#include <memory>
#include <set>
#include <string>
#include <unordered_map>

#include "boost/asio/io_context.hpp"
#include "boost/asio/ip/address_v4.hpp"
#include "boost/asio/read_until.hpp"
#include "boost/asio/streambuf.hpp"

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

struct UserMessage {
  std::string username;
  std::string text;

  bool operator==(const UserMessage& other) const { return username == other.username && text == other.text; }
};

class User {
 public:
  User(int room_id, std::string name) : room_id { room_id }, name { name } {}

  int get_room_id() { return room_id; }

  std::string get_name() { return name; }

  virtual void send(UserMessage user_message) = 0;

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

  void send_chat_history(std::shared_ptr<User> user) {
    for (auto&& user_message : chat_history) {
      user->send(user_message);
    }
  }

 protected:
  int room_id;
  std::set<std::shared_ptr<User>> users;
  std::deque<UserMessage> chat_history;
};

class TcpUser : public User {
 public:
  TcpUser(int room_id, std::string name, boost::asio::ip::tcp::socket&& socket)
      : User { room_id, name }, socket { std::move(socket) } {}

  void send(UserMessage user_message) override {}

  void send(const std::string& message) { boost::asio::write(socket, boost::asio::buffer(message)); }

 private:
  boost::asio::ip::tcp::socket socket;
};

template <typename RoomType = Room>
class Chat {
 public:
  Chat(unsigned short port, boost::asio::io_context& context)
      : acceptor { new boost::asio::ip::tcp::acceptor { context, boost::asio::ip::tcp::v4() } } {
    acceptor->set_option((boost::asio::ip::tcp::acceptor::reuse_address(true)));
    acceptor->bind(boost::asio::ip::tcp::endpoint { boost::asio::ip::address_v4::any(), port });
    acceptor->listen();
  }

  bool is_there_room(int room_id) { return rooms.contains(room_id); }

  void create_room(int room_id) { rooms[room_id] = std::make_shared<RoomType>(room_id); }

  void register_user(int room_id, std::shared_ptr<User> user) { rooms[room_id]->register_user(user); }

  void start() {
    while (true) {
      auto socket { acceptor->accept() };

      boost::asio::streambuf buf;
      boost::asio::read_until(socket, buf, '@');

      std::istream is { &buf };
      std::string room_id_temp;
      std::getline(is, room_id_temp, ';');

      std::string name;
      std::getline(is, name, '@');

      const auto room_id { std::stoi(room_id_temp) };
      if (is_there_room(room_id) == false) {
        create_room(room_id);
      }

      auto user { std::make_shared<TcpUser>(room_id, name, std::move(socket)) };
      register_user(room_id, user);
      user->send("OK@");
      rooms[room_id]->send_chat_history(user);
    }
  }

 protected:
  Chat() = default;

  std::unordered_map<int, std::shared_ptr<RoomType>> rooms;
  std::unique_ptr<boost::asio::ip::tcp::acceptor> acceptor;
};

#endif