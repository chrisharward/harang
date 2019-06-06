#pragma once

#include <algorithm>
#include <functional>
#include <ostream>
#include <unordered_map>
#include <vector>
#include <exception>

namespace harang {

template <typename Key, typename Weight> class edge {
public:
  constexpr edge(Key key, Weight weight)
      : neighbour_key_(std::move(key)), weight_(std::move(weight)) {}

  constexpr bool operator==(const edge &e) const {
    return std::tie(neighbour_key_, weight_) ==
           std::tie(e.neighbour_key_, e.weight_);
  }

  constexpr bool operator<(const edge &other) const {
    return neighbour_key_ < other.neighbour_key_;
  }

  constexpr bool operator<(const Key &key) const {
    return neighbour_key_ < key;
  }

  constexpr std::ostream &print(std::ostream &os) const {
    os << " { " << neighbour_key_ << ", " << weight_ << " } ";
    return os;
  }

  constexpr const Key &get_key() const { return neighbour_key_; }

  constexpr const Weight &get_weight() const { return weight_; }

private:
  Key neighbour_key_;
  Weight weight_;
};

template <typename Key, typename Weight>
constexpr std::ostream &operator<<(std::ostream &os,
                                   const edge<Key, Weight> &e) {
  return e.print(os);
}

class edge_not_found : public std::exception {
};

template <typename Key, typename Weight> class vertex {
public:
  constexpr vertex() = default;
  constexpr vertex(std::initializer_list<edge<Key, Weight>> &&ilist)
      : edges_(std::move(ilist)) {
    std::sort(std::begin(edges_), std::end(edges_));
  }
  constexpr vertex<Key, Weight> &
  operator=(std::initializer_list<edge<Key, Weight>> &&ilist) {
    edges_ = std::move(ilist);
    std::sort(std::begin(edges_), std::end(edges_));
    return *this;
  }

  constexpr decltype(auto) begin() { return std::begin(edges_); }

  constexpr decltype(auto) begin() const { return std::cbegin(edges_); }

  constexpr bool empty() const { return edges_.empty(); }

  constexpr decltype(auto) end() { return std::end(edges_); }

  constexpr decltype(auto) end() const { return std::cend(edges_); }

  constexpr const edge<Key, Weight>& operator[](const Key &key) const {
    auto it = std::lower_bound(std::begin(edges_), std::end(edges_), key);
    if (it == std::end(edges_) || it->get_key() != key) {
      throw edge_not_found{};
    }
    return *it;
  }

  constexpr size_t size() const { return edges_.size(); }

private:
  std::vector<edge<Key, Weight>> edges_;
};

template <typename Key, typename Weight = int> class graph {
public:
  using vertex_map = std::unordered_map<Key, vertex<Key, Weight>>;
  using key_type = Key;
  using value_type = vertex<Key, Weight>;
  using edge_type = edge<Key, Weight>;

  constexpr decltype(auto) begin() { return std::begin(vertices_); }

  constexpr decltype(auto) begin() const { return std::cbegin(vertices_); }

  template <typename... Args>
  constexpr decltype(auto) emplace(Args &&... args) {
    return vertices_.emplace(std::piecewise_construct,
                             std::forward_as_tuple(args...),
                             std::forward_as_tuple());
  }

  constexpr decltype(auto) empty() const { return vertices_.empty(); }

  constexpr decltype(auto) end() { return std::end(vertices_); }

  constexpr decltype(auto) end() const { return std::cend(vertices_); }

  constexpr decltype(auto) find(key_type key) { return vertices_.find(key); }

  constexpr decltype(auto) find(key_type key) const {
    return vertices_.find(key);
  }

  constexpr decltype(auto)
  insert_or_assign(const key_type &k,
                   std::initializer_list<edge<Key, Weight>> &&ilist) {
    return vertices_.insert_or_assign(k, std::move(ilist));
  }

  constexpr vertex<key_type, Weight> &operator[](const key_type &key) {
    return vertices_[key];
  }

  constexpr decltype(auto) size() const { return vertices_.size(); }

  constexpr decltype(auto)
  try_emplace(const key_type &k,
              std::initializer_list<edge<Key, Weight>> &&ilist) {
    return vertices_.try_emplace(k, std::move(ilist));
  }

private:
  vertex_map vertices_;
};

} // namespace harang
