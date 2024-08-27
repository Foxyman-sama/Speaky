#ifndef SERIALIZE_H
#define SERIALIZE_H

#include <string>

namespace speaky {

template <typename ProtoClass>
std::string serialize(ProtoClass&& proto_class) {
  return proto_class.serialize();
}

}  // namespace speaky

#endif