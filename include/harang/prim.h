#pragma once

#include "graph_adj_list.h"

#include <limits>
#include <unordered_map>
#include <unordered_set>
#include <vector>

namespace harang {

template <typename Graph>
constexpr decltype(auto) prim(const Graph &g,
                              const typename Graph::key_type &start_key) {
  std::unordered_set<typename Graph::key_type> mst_set;
  std::unordered_map<typename Graph::key_type, int> key_edge;
  std::unordered_map<typename Graph::key_type, typename Graph::key_type> parent;

  key_edge[start_key];
  for (int i = 1; i < g.size(); ++i) {
    typename Graph::key_type min_key;
    int min_dist = std::numeric_limits<int>::max();
    for (const auto &p : key_edge) {
      if (auto it = mst_set.find(p.first); it == end(mst_set)) {
        if (p.second < min_dist) {
          min_key = p.first;
          min_dist = p.second;
        }
      }
    }

    mst_set.insert(min_key);
    auto it = g.find(min_key);
    for (const auto &e : it->second) {
      if (auto mst_it = mst_set.find(e.get_key()); mst_it == end(mst_set)) {
        if (auto key_edge_it = key_edge.find(e.get_key()); key_edge_it == end(key_edge)) {
          parent.emplace(e.get_key(), min_key);
          key_edge.emplace(e.get_key(), e.get_weight());
        } else if (key_edge_it->second > e.get_weight()) {
          parent[e.get_key()] = min_key;
          key_edge_it->second = e.get_weight();
        }
      }
    }
  }

  std::unordered_map<typename Graph::key_type,
                     std::vector<typename Graph::edge_type>>
      res{};
  // for (const auto &p : g) {
  //   res[p.first];
  //   for (const auto &pk : parent) {
  //     if (pk.second == p.first) {
  //       res[p.first].emplace_back(pk.first, key_edge[pk.first]);
  //     }
  //   }
  // }

  for (const auto &pk : parent) {
    auto it = g.find(pk.second);
    res[pk.second].emplace_back(it->second[pk.first]);
  }

  return res;
}

} // namespace harang