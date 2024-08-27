#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include <memory>
#include <unordered_map>
#include <vector>

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

class ParticipantMock : public Participant {
 public:
  ParticipantMock(const std::string& name) : Participant { name } {}

  virtual ~ParticipantMock() {}

  MOCK_METHOD(void, deliver, (const std::string& message), (override));
};

class register_user_tests : public Test, public Input {};

TEST_F(register_user_tests, register_common_user) {
  constexpr int chat_id { 0 };
  auto participant { std::make_shared<ParticipantMock>("Dima") };

  register_user(chat_id, participant);

  ASSERT_EQ(rooms[chat_id]->get_number_of_members(), 1);
}