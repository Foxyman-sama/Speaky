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
  void register_user(int room_id, ParticipantPtr participant) {
    if (is_room_found(room_id) == false) {
      create_room(room_id);
    }

    connect_participant_with_room(room_id, participant);
  }

 protected:
  bool is_room_found(int room_id) const noexcept { return rooms.find(room_id) != rooms.end(); }

  void create_room(int room_id) noexcept { rooms[room_id] = std::make_shared<ChatRoom>(); }

  void connect_participant_with_room(int room_id, ParticipantPtr participant) {
    participant->attach(rooms[room_id]);
    rooms[room_id]->join(participant);
  }

  std::unordered_map<int, ChatRoomPtr> rooms;
};

}  // namespace speaky

#endif