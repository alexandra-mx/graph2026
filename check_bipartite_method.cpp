/**
@file methods/check_bipartite_method.cpp
*/

#include <check_bipartite.hpp>
#include <nlohmann/json.hpp>

using graph::Graph;

namespace graph {


int CheckBipartiteMethod(const nlohmann::json& input, nlohmann::json* output) {
  Graph g;

  size_t n = input["n"].get<size_t>();

  for (size_t i = 0; i < n; ++i) {
    g.AddVertex(i);
  }

  const auto& edges = input["edges"];
  for (const auto& edge : edges) {
    size_t v1 = edge[0].get<size_t>();
    size_t v2 = edge[1].get<size_t>();
    g.AddEdge(v1, v2);
  }

  BipartiteResult result = CheckBipartite(g);

  (*output)["is_bipartite"] = result.is_bipartite;

  if (result.is_bipartite) {
    (*output)["part0"] = result.part0;
    (*output)["part1"] = result.part1;
  }

  return 0;
}

}  // namespace graph
