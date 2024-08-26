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

  virtual ~Participant() {}

  void attach(ObserverPtr observer) { observers.push_back(observer); }

  virtual void deliver(const std::string& message) = 0;

  void notify(const std::string& message) {
    for (auto&& observer : observers) {
      observer->update(message);
    }
  }

 protected:
  std::string name;
  std::vector<ObserverPtr> observers;
};

using ParticipantPtr = std::shared_ptr<Participant>;

}  // namespace speaky

#endif