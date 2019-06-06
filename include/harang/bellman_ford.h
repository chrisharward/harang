#pragma once

#include "graph_adj_list.h"

#include <limits>
#include <optional>
#include <unordered_map>
#include <vector>

namespace harang {

template <typename Graph>
constexpr decltype(auto) bellman_ford(const Graph &g,
                                 const typename Graph::key_type &start_key) {
  std::unordered_map<int, typename Graph::key_type> index_map;
  std::unordered_map<typename Graph::key_type, int> key_map;
  std::unordered_map<typename Graph::key_type, typename Graph::key_type> parent;
  std::vector<int> dist(g.size(), std::numeric_limits<int>::max());

  index_map[0] = start_key;
  key_map[start_key] = 0;
  {
    int index = 1;
    for (const auto &p : g) {
      if (p.first != start_key) {
        key_map[p.first] = index;
        index_map[index++] = p.first;
      }
    }
  }
  dist[0] = 0;

  for (int i = 0; i < g.size() - 1; ++i) {
    if (dist[i] == std::numeric_limits<int>::max()) continue;

    auto it = g.find(index_map[i]);
    for (const auto &e : it->second) {
      if (dist[key_map[e.get_key()]] > dist[i] + e.get_weight()) {
        dist[key_map[e.get_key()]] = dist[i] + e.get_weight();
        parent[e.get_key()] = index_map[i];
      }
    }
  }

  std::optional<std::pair<
      std::unordered_map<typename Graph::key_type, int>,
      std::unordered_map<typename Graph::key_type, typename Graph::key_type>>> res;

  // check for negative cycles
  for (int i = 0; i < g.size() - 1; ++i) {
    auto it = g.find(index_map[i]);
    for (const auto &e : it->second) {
      if (dist[key_map[e.get_key()]] > dist[i] + e.get_weight()) {
        return res;
      }
    }
  }

  std::unordered_map<typename Graph::key_type, int> dist_map;
  for (const auto &p : g) {
    dist_map[p.first] = dist[key_map[p.first]];
  }

  res = std::pair<
      std::unordered_map<typename Graph::key_type, int>,
      std::unordered_map<typename Graph::key_type, typename Graph::key_type>>{
      std::move(dist_map), std::move(parent)};
  return res;
}

} // namespace harang