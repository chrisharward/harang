#pragma once

#include "graph_adj_list.h"

#include <limits>
#include <unordered_map>

namespace harang {

template <typename Graph>
constexpr decltype(auto) kruskal(const Graph &g,
                                 const typename Graph::key_type &start_key) {
  std::unordered_map<typename Graph::key_type, int> dist;
  std::unordered_map<typename Graph::key_type, typename Graph::key_type> parent;

  return std::pair<
      std::unordered_map<typename Graph::key_type, int>,
      std::unordered_map<typename Graph::key_type, typename Graph::key_type>>{
      std::move(dist), std::move(parent)};
}

} // namespace harang