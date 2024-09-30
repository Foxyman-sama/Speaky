#ifndef CHAT_ROOM_H
#define CHAT_ROOM_H

#include <algorithm>
#include <memory>
#include <ranges>
#include <set>

#include "participant.h"

namespace speaky {

class ChatRoom : public Observer {
 public:
  void send(const std::string& message) override {
    std::ranges::for_each(participants, [&](auto&& participant) { participant->send(message); });
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