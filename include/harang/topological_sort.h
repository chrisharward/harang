#pragma once

#include "graph_adj_list.h"
#include <stack>
#include <unordered_map>
#include <unordered_set>
#include <vector>

namespace harang {

namespace {

template <typename Graph>
constexpr void
topological_sort_vertex(const Graph &g, const typename Graph::key_type &key,
                        std::unordered_set<typename Graph::key_type> &vis,
                        std::stack<typename Graph::key_type> &vertices) {
  auto it = g.find(key);
  for (const auto &e : it->second) {
    if (auto res = vis.insert(e.get_key()); res.second) {
      topological_sort_vertex(g, e.get_key(), vis, vertices);
    }
  }
  vertices.push(key);
}

template <typename Graph>
constexpr void all_topological_sort_impl(
    const Graph &g, std::unordered_set<typename Graph::key_type> &vis,
    std::vector<std::vector<typename Graph::key_type>> &res,
    std::vector<typename Graph::key_type> &curr,
    std::unordered_map<typename Graph::key_type, int> &indegree) {
  bool flag = false;

  for (const auto &p : g) {
    if (indegree[p.first] != 0)
      continue;
    if (auto it = vis.find(p.first); it == end(vis)) {
      // reduce indegree of adjacent vertices
      for (const auto &e : p.second) {
        indegree[e.get_key()]--;
      }

      curr.push_back(p.first);
      vis.insert(p.first);
      all_topological_sort_impl(g, vis, res, curr, indegree);

      // resetting visited, res and indegree for backtracking
      vis.erase(p.first);
      curr.erase(curr.end() - 1);
      for (const auto &e : p.second) {
        indegree[e.get_key()]++;
      }

      flag = true;
    }
  }

  if (!flag) {
    res.push_back(curr);
  }
}

} // namespace

template <typename Graph> constexpr auto topological_sort(const Graph &g) {
  std::stack<typename Graph::key_type> vertices;
  std::unordered_set<typename Graph::key_type> vis;
  for (const auto &p : g) {
    if (auto it = vis.insert(p.first); it.second) {
      topological_sort_vertex(g, p.first, vis, vertices);
    }
  }

  std::vector<typename Graph::key_type> sorted;
  for (sorted.reserve(vertices.size()); !vertices.empty(); vertices.pop()) {
    sorted.push_back(vertices.top());
  }

  return sorted;
}

template <typename Graph> constexpr auto all_topological_sort(const Graph &g) {
  std::unordered_set<typename Graph::key_type> vis;
  std::vector<std::vector<typename Graph::key_type>> res;
  std::vector<typename Graph::key_type> curr;
  auto indegree = calculate_indegree(g);

  all_topological_sort_impl(g, vis, res, curr, indegree);
  return res;
}

} // namespace harang