#ifndef OBSERVER_H
#define OBSERVER_H

#include <memory>
#include <string>

namespace speaky {

class Observer {
 public:
  void update(const std::string& message) {}
};

using ObserverPtr = std::shared_ptr<Observer>;

}  // namespace speaky

#endif