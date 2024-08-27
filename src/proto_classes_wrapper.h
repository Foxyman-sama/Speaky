#ifndef PROTO_CLASSES_WRAPPER_H
#define PROTO_CLASSES_WRAPPER_H

#include "proto/message.pb.h"
#include "proto/new_connection.pb.h"

namespace speaky {

class ProtoClass {
 public:
  virtual ~ProtoClass() = default;

  virtual std::string serialize() const = 0;

  virtual void deserialize(const std::string& serialized) = 0;
};

class MessageProto : public ProtoClass {
 public:
  MessageProto() noexcept = default;

  MessageProto(const std::string& message) noexcept { obj.set_message(message); }

  MessageProto(const char* const message) noexcept { obj.set_message(message); }

  std::string serialize() const override { return obj.SerializeAsString(); }

  void deserialize(const std::string& serialized) override { obj.ParseFromString(serialized); }

  std::string get_message() const noexcept { return obj.message(); }

 private:
  speaky::Message obj;
};

class NewConnectionProto : public ProtoClass {
 public:
  NewConnectionProto() noexcept = default;

  NewConnectionProto(const std::string& name, int room_id) noexcept { init(name, room_id); }

  NewConnectionProto(const char* const name, int room_id) noexcept { init(name, room_id); }

  std::string serialize() const override { return obj.SerializeAsString(); }

  void deserialize(const std::string& serialized) override { obj.ParseFromString(serialized); }

  std::string get_name() const noexcept { return obj.name(); }

  int get_room_id() const noexcept { return obj.room_id(); }

 private:
  void init(const std::string& name, int room_id) noexcept {
    obj.set_name(name);
    obj.set_room_id(room_id);
  }

  speaky::NewConnection obj;
};
}  // namespace speaky

#endif