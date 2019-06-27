#include "catch2/catch.hpp"
#include "graph.h"

#include <sstream>

TEST_CASE( "graph_basic", "[graph]" ) {
  harang::graph<int> g;
  REQUIRE(g.empty());
  REQUIRE(g.size() == 0ull);

  {
    auto res = g.find(1);
    REQUIRE(res == g.end());
  }

  {
    auto res = g.emplace(1);
    REQUIRE(res.second);
    REQUIRE(res.first != g.end());
    res = g.try_emplace(2, { { 1, 1 }, { 3, 1 } });
    res = g.emplace(3);
    REQUIRE(g[3].empty());
    res = g.emplace(4);
    
    g[1] = { { 2, 1 }, { 4, 1 } };
    REQUIRE(g[1].size() == 2);

    g.insert_or_assign(4, { { 1, 1 }, { 3, 1 }});
    REQUIRE(g[4].size() == 2);

    g[3] = { { 2, 1 }, { 4, 1 } };
    REQUIRE(g[3].size() == 2);
  }
}

TEST_CASE( "dfs_basic", "[graph]" ) {
  harang::graph<int, int> g;
  g[0] = { {1,1}, {2,1}, {3,1} };
  g[1];
  g[2] = { {4,1} };
  g[3];
  g[4];

  auto res = dfs(g, 0);
  REQUIRE( !res.empty() );
  REQUIRE( res.size() == 5 );
  REQUIRE( res[0].key_ == 0 );
  REQUIRE( !res[0].parent_ );
  REQUIRE( res[0].d_ == 1 );
  REQUIRE( res[0].f_ == 10 );
  REQUIRE( res[1].key_ == 1 );
  REQUIRE( res[1].parent_.value() == 0 );
  REQUIRE( res[1].d_ == 2 );
  REQUIRE( res[1].f_ == 3 );
  REQUIRE( res[2].key_ == 2 );
  REQUIRE( res[2].parent_.value() == 0 );
  REQUIRE( res[2].d_ == 4 );
  REQUIRE( res[2].f_ == 7 );
  REQUIRE( res[3].key_ == 3 );
  REQUIRE( res[3].parent_.value() == 0 );
  REQUIRE( res[3].d_ == 8 );
  REQUIRE( res[3].f_ == 9 );
  REQUIRE( res[4].key_ == 4 );
  REQUIRE( res[4].parent_.value() == 2 );
  REQUIRE( res[4].d_ == 5 );
  REQUIRE( res[4].f_ == 6 );
}

TEST_CASE( "bfs_basic", "[graph]" ) {
  harang::graph<int, int> g;
  g[0] = { { 1, 4 }, { 7, 8 } };
  g[1] = { { 0, 4 }, { 2, 8 }, { 7, 11 } };
  g[2] = { { 1, 8 }, { 3, 7 }, { 5, 4 }, { 8, 2 } };
  g[3] = { { 2, 7 }, { 4, 9 }, { 5, 14 } };
  g[4] = { { 3, 9 }, { 5, 10 } };
  g[5] = { { 2, 4 }, { 3, 14 }, { 4, 10 }, { 6, 2 } };
  g[6] = { { 5, 2 }, { 7, 1 }, { 8, 6 } };
  g[7] = { { 0, 8 }, { 1, 11 }, { 6, 1 }, { 8, 7 } };
  g[8] = { { 2, 2 }, { 6, 6 }, { 7, 7 } };

  auto res = bfs(g, 0);

  REQUIRE( res.size() == 9 );
  REQUIRE( res[0].dist_ == 0 );
  REQUIRE( res[0].key_ == 0 );
  REQUIRE( !res[0].parent_ );
  REQUIRE( res[1].dist_ == 1 );
  REQUIRE( res[1].key_ == 1 );
  REQUIRE( res[1].parent_.value() == 0 );
  REQUIRE( res[2].dist_ == 2 );
  REQUIRE( res[2].key_ == 2 );
  REQUIRE( res[2].parent_.value() == 1 );
  REQUIRE( res[3].dist_ == 3 );
  REQUIRE( res[3].key_ == 3 );
  REQUIRE( res[3].parent_.value() == 2 );
  REQUIRE( res[4].dist_ == 4 );
  REQUIRE( res[4].key_ == 4 );
  REQUIRE( res[4].parent_.value() == 3 );
  REQUIRE( res[5].dist_ == 3 );
  REQUIRE( res[5].key_ == 5 );
  REQUIRE( res[5].parent_.value() == 2 );
  REQUIRE( res[6].dist_ == 2 );
  REQUIRE( res[6].key_ == 6 );
  REQUIRE( res[6].parent_.value() == 7 );
  REQUIRE( res[7].dist_ == 1 );
  REQUIRE( res[7].key_ == 7 );
  REQUIRE( res[7].parent_.value() == 0 );
  REQUIRE( res[8].dist_ == 2 );
  REQUIRE( res[8].key_ == 8 );
  REQUIRE( res[8].parent_.value() == 7 );
}

TEST_CASE( "topological_sort_basic", "[graph]" ) {
  harang::graph<int, int> g;
  g[0];
  g[1];
  g[2] = { { 3, 1 } };
  g[3] = { { 1, 1 } };
  g[4] = { { 0, 1 }, { 1, 1 } };
  g[5] = { { 0, 1 }, { 2, 1 } };

  auto sorted = topological_sort(g);
  REQUIRE( sorted == std::vector<int>{ 4, 5, 2, 3, 1, 0 } );
}

TEST_CASE( "all_topological_sort_basic", "[graph]" ) {
  harang::graph<int, int> g;
  g[0];
  g[1];
  g[2] = { { 3, 1 } };
  g[3] = { { 1, 1 } };
  g[4] = { { 0, 1 }, { 1, 1 } };
  g[5] = { { 0, 1 }, { 2, 1 } };

  auto indegree = calculate_indegree(g);
  REQUIRE( indegree[0] == 2 );
  REQUIRE( indegree[1] == 2 );
  REQUIRE( indegree[2] == 1 );
  REQUIRE( indegree[3] == 1 );
  REQUIRE( indegree[4] == 0 );
  REQUIRE( indegree[5] == 0 );

  auto sorted = all_topological_sort(g);
  auto exp = std::vector<std::vector<int>>{ 
    { 4, 5, 0, 2, 3, 1 },
    { 4, 5, 2, 0, 3, 1 },
    { 4, 5, 2, 3, 0, 1 },
    { 4, 5, 2, 3, 1, 0 },
    { 5, 2, 3, 4, 0, 1 },
    { 5, 2, 3, 4, 1, 0 },
    { 5, 2, 4, 0, 3, 1 },
    { 5, 2, 4, 3, 0, 1 },
    { 5, 2, 4, 3, 1, 0 },
    { 5, 4, 0, 2, 3, 1 },
    { 5, 4, 2, 0, 3, 1 },
    { 5, 4, 2, 3, 0, 1 },
    { 5, 4, 2, 3, 1, 0 },
   };
  
  REQUIRE( sorted.size() == exp.size() );
  for (const auto &v : sorted) {
    REQUIRE( std::find(begin(exp), end(exp), v) != end(exp) );
  }
}

TEST_CASE( "dijkstra_basic", "[graph]" ) {
  harang::graph<int, int> g;
  g[0] = { { 1, 4 }, { 7, 8 } };
  g[1] = { { 0, 4 }, { 2, 8 }, { 7, 11 } };
  g[2] = { { 1, 8 }, { 3, 7 }, { 5, 4 }, { 8, 2 } };
  g[3] = { { 2, 7 }, { 4, 9 }, { 5, 14 } };
  g[4] = { { 3, 9 }, { 5, 10 } };
  g[5] = { { 2, 4 }, { 3, 14 }, { 4, 10 }, { 6, 2 } };
  g[6] = { { 5, 2 }, { 7, 1 }, { 8, 6 } };
  g[7] = { { 0, 8 }, { 1, 11 }, { 6, 1 }, { 8, 7 } };
  g[8] = { { 2, 2 }, { 6, 6 }, { 7, 7 } };

  auto p = dijkstra(g, 0);
  REQUIRE( p.first[0] == 0  );
  REQUIRE( p.first[1] == 4  );
  REQUIRE( p.first[2] == 12 );
  REQUIRE( p.first[3] == 19 );
  REQUIRE( p.first[4] == 21 );
  REQUIRE( p.first[5] == 11 );
  REQUIRE( p.first[6] == 9  );
  REQUIRE( p.first[7] == 8  );
  REQUIRE( p.first[8] == 14 );

  REQUIRE( p.second[1] == 0 );
  REQUIRE( p.second[2] == 1 );
  REQUIRE( p.second[3] == 2 );
  REQUIRE( p.second[4] == 5 );
  REQUIRE( p.second[5] == 6 );
  REQUIRE( p.second[6] == 7 );
  REQUIRE( p.second[7] == 0 );
  REQUIRE( p.second[8] == 2 );
}

TEST_CASE( "prim_basic", "[graph]" ) {
  harang::graph<int, int> g;
  g[0] = { { 1, 4 }, { 7, 8 } };
  g[1] = { { 0, 4 }, { 2, 8 }, { 7, 11 } };
  g[2] = { { 1, 8 }, { 3, 7 }, { 5, 4 }, { 8, 2 } };
  g[3] = { { 2, 7 }, { 4, 9 }, { 5, 14 } };
  g[4] = { { 3, 9 }, { 5, 10 } };
  g[5] = { { 2, 4 }, { 3, 14 }, { 4, 10 }, { 6, 2 } };
  g[6] = { { 5, 2 }, { 7, 1 }, { 8, 6 } };
  g[7] = { { 0, 8 }, { 1, 11 }, { 6, 1 }, { 8, 7 } };
  g[8] = { { 2, 2 }, { 6, 6 }, { 7, 7 } };

  auto span = prim(g, 0);
  REQUIRE( span[0] == std::vector<typename harang::graph<int, int>::edge_type>{ { 1, 4 } } );
  REQUIRE( span[1] == std::vector<typename harang::graph<int, int>::edge_type>{ { 2, 8 } } );
  REQUIRE( span[2] == std::vector<typename harang::graph<int, int>::edge_type>{ { 8, 2 }, { 3, 7 }, { 5, 4 } } );
  REQUIRE( span[3] == std::vector<typename harang::graph<int, int>::edge_type>{ { 4, 9 } } );
  REQUIRE( span[4].empty() );
  REQUIRE( span[5] == std::vector<typename harang::graph<int, int>::edge_type>{ { 6, 2 } } );
  REQUIRE( span[6] == std::vector<typename harang::graph<int, int>::edge_type>{ { 7, 1 } } );
  REQUIRE( span[7].empty() );
  REQUIRE( span[8].empty() );
}

TEST_CASE( "prim_basic2", "[graph]" ) {
  harang::graph<int, int> g;
  g[0] = { { 1, 2 }, { 3, 6 } };
  g[1] = { { 0, 2 }, { 2, 3 }, { 3, 8 }, { 4, 5 } };
  g[2] = { { 1, 3 }, { 4, 7 } };
  g[3] = { { 0, 6 }, { 1, 8 }, { 4, 9 } };
  g[4] = { { 1, 5 }, { 2, 7 }, { 3, 9 } };

  auto span = prim(g, 0);
  REQUIRE( span[0] == std::vector<typename harang::graph<int, int>::edge_type>{ { 1, 2 }, { 3, 6 } } );
  REQUIRE( span[1] == std::vector<typename harang::graph<int, int>::edge_type>{ { 4, 5 }, { 2, 3 } } );
  REQUIRE( span[2].empty() );
  REQUIRE( span[3].empty() );
  REQUIRE( span[4].empty() );
}

TEST_CASE( "bellman_ford_basic", "[graph]" ) {
  harang::graph<int, int> g;
  g[0] = { { 1, 4 }, { 7, 8 } };
  g[1] = { { 0, 4 }, { 2, 8 }, { 7, 11 } };
  g[2] = { { 1, 8 }, { 3, 7 }, { 5, 4 }, { 8, 2 } };
  g[3] = { { 2, 7 }, { 4, 9 }, { 5, 14 } };
  g[4] = { { 3, 9 }, { 5, 10 } };
  g[5] = { { 2, 4 }, { 3, 14 }, { 4, 10 }, { 6, 2 } };
  g[6] = { { 5, 2 }, { 7, 1 }, { 8, 6 } };
  g[7] = { { 0, 8 }, { 1, 11 }, { 6, 1 }, { 8, 7 } };
  g[8] = { { 2, 2 }, { 6, 6 }, { 7, 7 } };

  auto span = bellman_ford(g, 0);
  REQUIRE( span );
  REQUIRE( span.value().first[0] == 0  );
  REQUIRE( span.value().first[1] == 4  );
  REQUIRE( span.value().first[2] == 12 );
  REQUIRE( span.value().first[3] == 19 );
  REQUIRE( span.value().first[4] == 21 );
  REQUIRE( span.value().first[5] == 11 );
  REQUIRE( span.value().first[6] == 9  );
  REQUIRE( span.value().first[7] == 8  );
  REQUIRE( span.value().first[8] == 14 );

  REQUIRE( span.value().second.find(0) == std::end(span.value().second) );
  REQUIRE( span.value().second[1] == 0 );
  REQUIRE( span.value().second[2] == 1 );
  REQUIRE( span.value().second[3] == 2 );
  REQUIRE( span.value().second[4] == 5 );
  REQUIRE( span.value().second[5] == 6 );
  REQUIRE( span.value().second[6] == 7 );
  REQUIRE( span.value().second[7] == 0 );
  REQUIRE( span.value().second[8] == 2 );
}

TEST_CASE("connected_components", "[graph]") {
  harang::graph<int, int> g;
  g[1] = {{2, 1}};
  g[2] = {{3, 1}};
  g[3] = {{1, 1}, {4, 1}};
  g[4];

  decltype(auto) gt = graph_transpose(g);
  REQUIRE(gt[1].size() == 1);
  REQUIRE(gt[1].begin()->get_key() == 3);
  REQUIRE(gt[2].size() == 1);
  REQUIRE(gt[2].begin()->get_key() == 1);
  REQUIRE(gt[3].size() == 1);
  REQUIRE(gt[3].begin()->get_key() == 2);
  REQUIRE(gt[4].size() == 1);
  REQUIRE(gt[4].begin()->get_key() == 3);

  auto comps = strongly_connected_components(g);
  REQUIRE(!comps.empty());
  REQUIRE(comps.size() == 2);
  REQUIRE(comps[0].size() == 3);
  REQUIRE(comps[0][0] == 3);
  REQUIRE(comps[0][1] == 2);
  REQUIRE(comps[0][2] == 1);
  REQUIRE(comps[1].size() == 1);
  REQUIRE(comps[1][0] == 4);
}