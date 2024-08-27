#ifndef PROTO_CLASSES_WRAPPER_H
#define PROTO_CLASSES_WRAPPER_H

#include "proto/message.pb.h"
#include "proto/new_connection.pb.h"

namespace speaky {

class ProtoClass {
 public:
  virtual std::string serialize() = 0;
};

class MessageProto : public ProtoClass {
 public:
  MessageProto(const std::string& message) { obj.set_message(message); }

  MessageProto(const char* const message) { obj.set_message(message); }

  std::string serialize() override { return obj.SerializeAsString(); }

 private:
  speaky::Message obj;
};

class NewConnectionProto : public ProtoClass {
 public:
  NewConnectionProto(const std::string& name, int room_id) {
    obj.set_name(name);
    obj.set_room_id(room_id);
  }

  NewConnectionProto(const char* const name, int room_id) {
    obj.set_name(name);
    obj.set_room_id(room_id);
  }

  std::string serialize() override { return obj.SerializeAsString(); }

 private:
  speaky::NewConnection obj;
};
}  // namespace speaky

#endif