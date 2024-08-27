#ifndef DESERIALIZE_H
#define DESERIALIZE_H

#include <string>

namespace speaky {

template <typename ProtoClass>
ProtoClass deserialize(const std::string& serialized) {
  ProtoClass proto_class;
  proto_class.deserialize(serialized);
  return proto_class;
}

}  // namespace speaky

#endif