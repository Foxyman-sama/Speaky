#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include "message.pb.h"

using namespace testing;

int main() {
  InitGoogleTest();
  InitGoogleMock();
  return RUN_ALL_TESTS();
}

class serialize_tests : public Test {};