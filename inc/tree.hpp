#pragma once

#include <tuple>
#include "util.hpp"

struct Node {
  const char* str;
};

using std::pair;
using empty = std::tuple<>;

/// Create a binary tree
constexpr auto tree{pair(Node{"a"},pair(
                                        pair(Node{"b"}, pair(
                                                             empty(),
                                                             empty()
                                                            )
                                             ),
                                        pair(Node{"c"}, pair(
                                                             pair(Node{"d"}, pair(
                                                                                  empty(),
                                                                                  pair(Node{"f"}, pair(
                                                                                                       empty(),
                                                                                                       empty()
                                                                                                      )
                                                                                      )
                                                                                 )
                                                                  ),
                                                             pair(Node{"e"}, pair(
                                                                                  empty(),
                                                                                  empty()
                                                                                 )
                                                                 )
                                                            )
                                             )
                                      )
                         )
                   };
