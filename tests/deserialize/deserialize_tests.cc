#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include <string>

#include "src/proto_classes_wrapper.h"

using namespace testing;
using namespace speaky;

int main() {
  InitGoogleTest();
  InitGoogleMock();
  return RUN_ALL_TESTS();
}

class deserialize_tests : public Test {};