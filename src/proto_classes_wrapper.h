#ifndef PROTO_CLASSES_WRAPPER_H
#define PROTO_CLASSES_WRAPPER_H

#include "proto/message.pb.h"
#include "proto/new_connection.pb.h"

namespace speaky {

class ProtoClass {
 public:
  virtual std::string serialize() = 0;

  virtual void deserialize(const std::string& serialized) = 0;
};

class MessageProto : public ProtoClass {
 public:
  MessageProto() noexcept = default;

  MessageProto(const std::string& message) { obj.set_message(message); }

  MessageProto(const char* const message) { obj.set_message(message); }

  std::string serialize() override { return obj.SerializeAsString(); }

  void deserialize(const std::string& serialized) override { obj.ParseFromString(serialized); }

  std::string get_message() const noexcept { return obj.message(); }

 private:
  speaky::Message obj;
};

class NewConnectionProto : public ProtoClass {
 public:
  NewConnectionProto() noexcept = default;

  NewConnectionProto(const std::string& name, int room_id) {
    obj.set_name(name);
    obj.set_room_id(room_id);
  }

  NewConnectionProto(const char* const name, int room_id) {
    obj.set_name(name);
    obj.set_room_id(room_id);
  }

  std::string serialize() override { return obj.SerializeAsString(); }

  void deserialize(const std::string& serialized) override { obj.ParseFromString(serialized); }

  std::string get_name() const noexcept { return obj.name(); }

  int get_room_id() const noexcept { return obj.room_id(); }

 private:
  speaky::NewConnection obj;
};
}  // namespace speaky

#endif