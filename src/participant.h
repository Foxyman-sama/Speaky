#ifndef PARTICIPANT_H
#define PARTICIPANT_H

#include <memory>
#include <string>
#include <vector>

#include "observer.h"

namespace speaky {

class Participant {
 public:
  explicit Participant(const std::string& name) : name { name } {}

  void attach(ObserverPtr observer) { observers.push_back(observer); }

 private:
  std::string name;
  std::vector<ObserverPtr> observers;
};

using ParticipantPtr = std::shared_ptr<Participant>;

}  // namespace speaky

#endif