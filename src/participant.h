#ifndef PARTICIPANT_H
#define PARTICIPANT_H

#include <memory>
#include <ranges>
#include <string>
#include <vector>

namespace speaky {

class Participant;

using ParticipantPtr = std::shared_ptr<Participant>;

class Observer {
 public:
  virtual void deliver(const std::string& message) = 0;

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
    std::ranges::for_each(observers, [&](auto&& observer) { observer->deliver(message); });
  }

  void disconnect() {
    auto self { shared_from_this() };
    std::ranges::for_each(observers, [&self](auto&& observer) { observer->disconnect(self); });
  }

 protected:
  std::string name;
  std::vector<ObserverPtr> observers;
};

}  // namespace speaky

#endif