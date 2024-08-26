#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include <memory>
#include <string>

#include "src/input.h"
#include "src/participant.h"

using namespace testing;
using namespace speaky;

int main() {
  InitGoogleTest();
  InitGoogleMock();
  return RUN_ALL_TESTS();
}

// TODO REFACTOR ALL DEPENDING CLASSES

class ParticipantMock : public Participant {
 public:
  ParticipantMock(const std::string& name) : Participant { name } {}

  virtual ~ParticipantMock() {}

  MOCK_METHOD(void, deliver, (const std::string& message), (override));

  void pretend_to_get_message(const std::string& message) { notify(message); }
};

using ParticipantMockPtr = std::shared_ptr<ParticipantMock>;

class send_message_tests : public Test, public Input {
 public:
  ParticipantMock participant1 { "Dima" };
  ParticipantMock participant2 { "Igor" };

  static constexpr int test_room_id { 0 };
  const std::string test_message { "Hello, World!" };
};

TEST_F(send_message_tests, send_message) {
  EXPECT_CALL(participant1, deliver(test_message)).Times(1);
  EXPECT_CALL(participant2, deliver(test_message)).Times(1);

  register_user(test_room_id, std::shared_ptr<ParticipantMock>(&participant1, [](auto*) {}));
  register_user(test_room_id, std::shared_ptr<ParticipantMock>(&participant2, [](auto*) {}));

  participant1.pretend_to_get_message(test_message);
}