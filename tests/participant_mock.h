#ifndef PARTICIPANT_MOCK_H
#define PARTICIPANT_MOCK_H

#include <gmock/gmock.h>

#include "src/participant.h"

using namespace speaky;

class ParticipantMock : public Participant {
 public:
  ParticipantMock(const std::string& name) : Participant { name } {}

  virtual ~ParticipantMock() {}

  MOCK_METHOD(void, send, (const std::string& message), (override));
};

#endif