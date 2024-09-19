#ifndef PARTICIPANT_H
#define PARTICIPANT_H

#include <memory>
#include <string>
#include <vector>

namespace speaky {

class Participant;

using ParticipantPtr = std::shared_ptr<Participant>;

class Observer {
 public:
  virtual void update(const std::string& message) = 0;

  virtual void disconnect(ParticipantPtr participant) = 0;
};

using ObserverPtr = std::shared_ptr<Observer>;

class Participant : public std::enable_shared_from_this<Participant> {
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

  void disconnect() {
    auto self { shared_from_this() };
    for (auto&& observer : observers) {
      observer->disconnect(self);
    }
  }

 protected:
  std::string name;
  std::vector<ObserverPtr> observers;
};

}  // namespace speaky

#endif