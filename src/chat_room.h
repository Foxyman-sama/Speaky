#ifndef CHAT_ROOM_H
#define CHAT_ROOM_H

#include <memory>
#include <set>

#include "participant.h"

namespace speaky {

class ChatRoom : public Observer {
 public:
  void update(const std::string& message) override { deliver(message); }

  void join(ParticipantPtr participant) { participants.insert(participant); }

  void kick(ParticipantPtr participant) { participants.erase(participant); }

  int get_number_of_members() { return participants.size(); }

  virtual void disconnect(ParticipantPtr participant) override { participants.erase(participant); }

 private:
  void deliver(const std::string& message) {
    for (auto&& participant : participants) {
      participant->deliver(message);
    }
  }

  std::set<ParticipantPtr> participants;
};

using ChatRoomPtr = std::shared_ptr<ChatRoom>;

}  // namespace speaky

#endif