#pragma once

#include "graph_adj_list.h"
#include <unordered_map>

namespace harang {

enum class color {
  WHITE,
  GRAY,
  BLACK
};

template <typename Graph> constexpr auto calculate_indegree(const Graph &g) {
  std::unordered_map<typename Graph::key_type, int> indegree;
  for (const auto &v : g) {
    for (const auto &e : v.second) {
      indegree[e.get_key()]++;
    }
  }
  return indegree;
}

} // namespace harang