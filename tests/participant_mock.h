#ifndef PARTICIPANT_MOCK_H
#define PARTICIPANT_MOCK_H

#include <gmock/gmock.h>

#include <string>

#include "src/participant.h"

class ParticipantMock : public speaky::Participant {
 public:
  ParticipantMock(const std::string& name) : Participant { name } {}

  virtual ~ParticipantMock() {}

  MOCK_METHOD(void, deliver, (const std::string& message), (override));
};

#endif