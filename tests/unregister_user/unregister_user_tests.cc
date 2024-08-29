#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include <memory>
#include <unordered_map>
#include <vector>

#include "../participant_mock.h"
#include "src/chat_room.h"
#include "src/input.h"
#include "src/participant.h"

using namespace testing;
using namespace speaky;

int main() {
  InitGoogleTest();
  InitGoogleMock();
  return RUN_ALL_TESTS();
}

class unregister_user_tests : public Test, public Input {
 public:
  void SetUp() {
    participant = std::make_shared<ParticipantMock>("Dima");
    register_user(chat_id, participant);

    ASSERT_EQ(rooms[chat_id]->get_number_of_members(), 1);
  }

  static constexpr int chat_id { 0 };
  std::shared_ptr<ParticipantMock> participant;
};

TEST_F(unregister_user_tests, unregister_common_user) {
  unregister_user(chat_id, participant);
  ASSERT_EQ(rooms[chat_id]->get_number_of_members(), 0);
}

TEST_F(unregister_user_tests, passes_wrong_chat_id_cause_exception) {
  ASSERT_ANY_THROW(unregister_user(chat_id + 1, participant));
}