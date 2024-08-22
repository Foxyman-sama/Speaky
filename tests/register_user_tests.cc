#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include <memory>
#include <unordered_map>
#include <vector>

#include "src/chat_room.h"
#include "src/participant.h"

using namespace testing;
using namespace speaky;

int main() {
  InitGoogleTest();
  InitGoogleMock();
  return RUN_ALL_TESTS();
}

class Input {
 public:
  void register_user(int chat_id, ParticipantPtr participant) {
    if (is_room_found(chat_id) == false) {
      create_room(chat_id);
    }

    connect_participant_with_room(chat_id, participant);
  }

 protected:
  bool is_room_found(int chat_id) const noexcept { return rooms.find(chat_id) != rooms.end(); }

  void create_room(int chat_id) noexcept { rooms[chat_id] = std::make_shared<ChatRoom>(); }

  void connect_participant_with_room(int chat_id, ParticipantPtr participant) {
    participant->attach(rooms[chat_id]);
    rooms[chat_id]->join(participant);
  }

  std::unordered_map<int, ChatRoomPtr> rooms;
};

class register_user_tests : public Test, public Input {};

TEST_F(register_user_tests, register_common_user) {
  constexpr int chat_id { 0 };
  auto participant { std::make_shared<Participant>("Dima") };

  register_user(chat_id, participant);

  ASSERT_EQ(rooms[chat_id]->get_number_of_members(), 1);
}