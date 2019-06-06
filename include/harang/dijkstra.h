#pragma once

#include "graph_adj_list.h"

#include <limits>
#include <unordered_map>
#include <unordered_set>

namespace harang {

template <typename Graph>
constexpr decltype(auto) dijkstra(const Graph &g,
                                  const typename Graph::key_type &start_key) {
  std::unordered_map<typename Graph::key_type, int> dist;
  std::unordered_map<typename Graph::key_type, typename Graph::key_type> parent;
  std::unordered_set<typename Graph::key_type> vis;
  dist[start_key] = 0;

  for (int i = 1; i < g.size(); ++i) {
    typename Graph::key_type min_key;
    int min_dist = std::numeric_limits<int>::max();
    for (const auto &p : dist) {
      if (auto res = vis.find(p.first); res == end(vis)) {
        if (p.second < min_dist) {
          min_key = p.first;
          min_dist = p.second;
        }
      }
    }

    vis.insert(min_key);
    auto it = g.find(min_key);
    for (const auto &e : it->second) {
      if (auto res = vis.find(e.get_key()); res == end(vis)) {
        if (auto d = dist.find(e.get_key()); d == end(dist)) {
          parent.emplace(e.get_key(), min_key);
          dist.emplace(e.get_key(), min_dist + e.get_weight());
        } else if (d->second > min_dist + e.get_weight()) {
          d->second = min_dist + e.get_weight();
          parent[e.get_key()] = min_key;
        }
      }
    }
  }

  return std::pair<
      std::unordered_map<typename Graph::key_type, int>,
      std::unordered_map<typename Graph::key_type, typename Graph::key_type>>{
      std::move(dist), std::move(parent)};
}

} // namespace harang