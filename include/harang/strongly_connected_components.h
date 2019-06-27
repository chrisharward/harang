#pragma once

#include "dfs.h"
#include "graph_adj_list.h"
#include <algorithm>
#include <unordered_set>

namespace {

template <typename Graph>
constexpr void scc_dfs_visit(const Graph &g,
                             const typename Graph::key_type &key,
                             std::unordered_set<typename Graph::key_type> &vis,
                             std::vector<typename Graph::key_type> &res) {
  vis.insert(key);
  res.push_back(key);
  auto git = g.find(key);
  for (const auto &e : git->second) {
    if (auto it = vis.find(e.get_key()); it != end(vis)) {
      continue;
    }
    scc_dfs_visit(g, e.get_key(), vis, res);
  }
}

} // namespace

namespace harang {

template <typename Graph>
constexpr decltype(auto) graph_transpose(const Graph &g) {
  Graph gt;
  for (auto &p : g) {
    gt[p.first];
    for (auto &e : p.second) {
      gt[e.get_key()].emplace_back(p.first, e.get_weight());
    }
  }
  return gt;
}

template<typename Graph>
constexpr decltype(auto) strongly_connected_components(const Graph &g) {
  if (g.empty())
    return std::vector<std::vector<typename Graph::key_type>>{};
  auto dfs_elems = dfs_vec(g, g.begin()->first);

  // sort by _d
  std::sort(
      begin(dfs_elems), end(dfs_elems),
      [](const auto &elem1, const auto &elem2) { return elem1.f_ > elem2.f_; });

  // create Gt
  auto gt = graph_transpose(g);

  std::vector<std::vector<typename Graph::key_type>> res;
  std::unordered_set<typename Graph::key_type> vis;
  for (const auto &elem : dfs_elems) {
    if (auto it = vis.find(elem.key_); it != end(vis)) {
      continue;
    }
    res.emplace_back();
    scc_dfs_visit(gt, elem.key_, vis, res.back());
  }

  return res;
}

} // namespace harang 