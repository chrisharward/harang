#pragma once

#include "graph_adj_list.h"
#include "graph_utils.h"

#include <limits>
#include <optional>
#include <unordered_set>

namespace harang {

template<typename key_type>
struct dfs_elem {
  key_type key_;
  std::optional<key_type> parent_;
  int d_ = -1;
  int f_ = -1;
};

namespace {

template<typename key_type>
struct dfs_vis_elem {
  key_type key_;
  std::optional<key_type> parent_;
  int d_ = -1;
  int f_ = -1;
  color color_ = color::WHITE;
};

template <typename Graph>
constexpr void dfs_visit(const Graph &g, const typename Graph::key_type &key,
  std::unordered_map<typename Graph::key_type, dfs_vis_elem<typename Graph::key_type>> &vis,
  int &time) {
  auto &elem = vis[key];
  elem.d_ = ++time;
  elem.color_ = color::GRAY;
  auto it = g.find(key);
  for (const auto &e : it->second) {
    if (vis[e.get_key()].color_ == color::WHITE) {
      vis[e.get_key()].parent_ = key;
      dfs_visit(g, e.get_key(), vis, time);
    }
  }
  elem.color_ = color::BLACK;
  elem.f_ = ++time;
}

template<typename key_type>
dfs_elem<key_type> create_dfs_elem(const dfs_vis_elem<key_type>& elem) {
  return { elem.key_, elem.parent_ , elem.d_, elem.f_ };
}

} // namespace

template <typename Graph>
constexpr decltype(auto) dfs(const Graph &g, const typename Graph::key_type &start_key) {
  std::unordered_map<typename Graph::key_type, dfs_vis_elem<typename Graph::key_type>> vis;
  for (const auto &p : g) {
    vis[p.first].key_ = p.first;
  }

  int time = 0;
  dfs_visit(g, start_key, vis, time);
  for (const auto &p : g) {
    if (vis[p.first].color_ == color::WHITE) {
      dfs_visit(g, p.first, vis, time);
    }
  }

  std::unordered_map<typename Graph::key_type, dfs_elem<typename Graph::key_type>> res;
  for (const auto &p : vis) {
    res.emplace(p.first, create_dfs_elem(p.second));
  }
  return res;
}

} // namespace harang