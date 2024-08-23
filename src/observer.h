#ifndef OBSERVER_H
#define OBSERVER_H

#include <memory>
#include <string>

namespace speaky {

class Observer {
 public:
  virtual void update(const std::string& message) = 0;
};

using ObserverPtr = std::shared_ptr<Observer>;

}  // namespace speaky

#endif