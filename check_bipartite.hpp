/**
@file include/check_bipartite.hpp
*/
#ifndef INCLUDE_CHECK_BIPARTITE_HPP_
#define INCLUDE_CHECK_BIPARTITE_HPP_

#include <graph.hpp>
#include <queue>
#include <unordered_map>
#include <vector>

namespace graph {

struct BipartiteResult {
  bool is_bipartite;
  std::vector<size_t> part0;
  std::vector<size_t> part1;

  BipartiteResult() : is_bipartite(false) {}
};

BipartiteResult CheckBipartite(const Graph& g) {
  BipartiteResult result;
  std::unordered_map<size_t, int> part;

  for (size_t v : g.Vertices()) {
    part[v] = -1;
  }

  for (size_t start : g.Vertices()) {
    if (part[start] != -1) {
      continue;
    }

    std::queue<size_t> q;
    q.push(start);
    part[start] = 0;

    while (!q.empty()) {
      size_t v = q.front();
      q.pop();

      const auto& neighbors = g.Edges(v);
      for (size_t to : neighbors) {
        if (part[to] == -1) {
          part[to] = part[v] ^ 1;
          q.push(to);
        } else if (part[to] == part[v]) {
          return result;
        }
      }
    }
  }

  result.is_bipartite = true;
  for (size_t v : g.Vertices()) {
    if (part[v] == 0) {
      result.part0.push_back(v);
    } else {
      result.part1.push_back(v);
    }
  }

  return result;
}

}  // namespace graph

#endif  // INCLUDE_CHECK_BIPARTITE_HPP_
