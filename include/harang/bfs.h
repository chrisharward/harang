#pragma once

#include "graph_adj_list.h"
#include "graph_utils.h"

#include <functional>
#include <optional>
#include <queue>
#include <unordered_set>
#include <unordered_map>

namespace harang {

template<typename key_type>
struct bfs_elem {
    int dist_ = std::numeric_limits<int>::max();
    key_type key_;
    std::optional<key_type> parent_;
};

namespace {

template<typename key_type>
struct bfs_vis_elem {
    color color_ = color::WHITE;
    int dist_ = std::numeric_limits<int>::max();
    key_type key_;
    std::optional<key_type> parent_;
};

template<typename key_type>
bfs_elem<key_type> create_bfs_elem(const bfs_vis_elem<key_type>& elem) {
  return { elem.dist_, elem.key_, elem.parent_ };
}

template <typename Graph, typename Callable>
constexpr void bfs(const Graph &g, const typename Graph::key_type &start_key,
                   Callable &&callback) {
  std::unordered_map<typename Graph::key_type, bfs_vis_elem<typename Graph::key_type>> vis;
  for (const auto &p : g) {
    vis[p.first].key_ = p.first;
  }

  vis[start_key].color_ = color::GRAY;
  vis[start_key].dist_ = 0;

  std::queue<std::reference_wrapper<bfs_vis_elem<typename Graph::key_type>>> vertices;
  vertices.push(std::ref(vis[start_key]));
  while (!vertices.empty()) {
    auto &elem = vertices.front().get();
    vertices.pop();
    callback(create_bfs_elem(elem));

    auto it = g.find(elem.key_);
    for (const auto &e : it->second) {
      auto &edge_elem = vis[e.get_key()];
      if (edge_elem.color_ == color::WHITE) {
        edge_elem.color_ = color::GRAY;
        edge_elem.dist_ = elem.dist_ + 1;
        edge_elem.parent_ = elem.key_;
        vertices.push(std::ref(edge_elem));
      }
    }
    elem.color_ = color::BLACK;
  }
}

} // namespace

template <typename Graph>
constexpr decltype(auto) bfs(const Graph &g, const typename Graph::key_type &start_key) {
  std::unordered_map<typename Graph::key_type, bfs_elem<typename Graph::key_type>> res;
  auto cb = [&res](auto&& elem) mutable {
    res.emplace(elem.key_, std::move(elem));
  };
  bfs(g, start_key, cb);
  return res;
}

} // namespace harang