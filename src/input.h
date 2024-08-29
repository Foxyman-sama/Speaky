#ifndef INPUT_H
#define INPUT_H

#include <memory>
#include <stdexcept>
#include <unordered_map>

#include "chat_room.h"
#include "participant.h"

namespace speaky {

class Input {
 public:
  void register_user(int chat_id, ParticipantPtr participant) {
    if (is_room_found(chat_id) == false) {
      create_room(chat_id);
    }

    connect_participant_with_room(chat_id, participant);
  }

  void unregister_user(int chat_id, ParticipantPtr participant) {
    check_room_existence(chat_id);
    rooms[chat_id]->kick(participant);
  }

 protected:
  bool is_room_found(int chat_id) const noexcept { return rooms.find(chat_id) != rooms.end(); }

  void create_room(int chat_id) noexcept { rooms[chat_id] = std::make_shared<ChatRoom>(); }

  void connect_participant_with_room(int chat_id, ParticipantPtr participant) {
    participant->attach(rooms[chat_id]);
    rooms[chat_id]->join(participant);
  }

  void check_room_existence(int chat_id) {
    if (is_room_found(chat_id) == false) {
      throw std::runtime_error { "The room wasn't found." };
    }
  }

  std::unordered_map<int, ChatRoomPtr> rooms;
};

}  // namespace speaky

#endif