#ifndef CHAT_ROOM_H
#define CHAT_ROOM_H

#include <memory>
#include <set>

#include "participant.h"

namespace speaky {

class ChatRoom : public Observer {
 public:
  void deliver(const std::string& message) override {
    for (auto&& participant : participants) {
      participant->deliver(message);
    }
  }

  void join(ParticipantPtr participant) { participants.insert(participant); }

  int get_number_of_members() { return participants.size(); }

  void disconnect(ParticipantPtr participant) override { participants.erase(participant); }

 private:
  std::set<ParticipantPtr> participants;
};

using ChatRoomPtr = std::shared_ptr<ChatRoom>;

}  // namespace speaky

#endif