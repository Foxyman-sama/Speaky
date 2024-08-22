#ifndef CHAT_ROOM_H
#define CHAT_ROOM_H

#include <memory>
#include <set>

#include "observer.h"
#include "participant.h"

namespace speaky {

class ChatRoom : public Observer {
 public:
  void join(ParticipantPtr participant) { participants.insert(participant); }

  int get_number_of_members() { return participants.size(); }

 private:
  std::set<ParticipantPtr> participants;
};

using ChatRoomPtr = std::shared_ptr<ChatRoom>;

}  // namespace speaky

#endif