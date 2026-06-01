/**
 * @file check_bipartite_test.cpp
 */

#include <httplib.h>
#include <iostream>
#include <algorithm>
#include <random>
#include <string>
#include <unordered_set>
#include <vector>
#include <nlohmann/json.hpp>

#include "test_core.hpp"

static void TestBipartitePath(httplib::Client* cli);
static void TestNonBipartiteTriangle(httplib::Client* cli);
static void TestRandomBipartite(httplib::Client* cli);

void TestCheckBipartite(httplib::Client* cli) {
  TestSuite suite("TestCheckBipartite");
  RUN_TEST_REMOTE(suite, cli, TestBipartitePath);
  RUN_TEST_REMOTE(suite, cli, TestNonBipartiteTriangle);
  RUN_TEST_REMOTE(suite, cli, TestRandomBipartite);
}

static void TestBipartitePath(httplib::Client* cli) {
  nlohmann::json input;
  input["n"] = 5;
  input["edges"] = nlohmann::json::array();

  input["edges"].push_back({0, 1});
  input["edges"].push_back({1, 2});
  input["edges"].push_back({2, 3});
  input["edges"].push_back({3, 4});

  auto res = cli->Post("/CheckBipartite", input.dump(), "application/json");
  if (!res) REQUIRE(false);

  nlohmann::json output = nlohmann::json::parse(res->body);

  REQUIRE(output["is_bipartite"].get<bool>() == true);

  size_t part0_size = output["part0"].size();
  size_t part1_size = output["part1"].size();
  REQUIRE(part0_size + part1_size == 5);
}

static void TestNonBipartiteTriangle(httplib::Client* cli) {
  nlohmann::json input;
  input["n"] = 3;
  input["edges"] = nlohmann::json::array();

  input["edges"].push_back({0, 1});
  input["edges"].push_back({1, 2});
  input["edges"].push_back({2, 0});

  auto res = cli->Post("/CheckBipartite", input.dump(), "application/json");
  if (!res) REQUIRE(false);

  nlohmann::json output = nlohmann::json::parse(res->body);

  REQUIRE(output["is_bipartite"].get<bool>() == false);
}

static void TestRandomBipartite(httplib::Client* cli) {
  nlohmann::json input;
  size_t n = 10;
  input["n"] = n;
  input["edges"] = nlohmann::json::array();

  std::random_device rd;

  std::mt19937 gen(rd());
  std::uniform_int_distribution<int> distrib(0, 2);

  for (size_t i = 0; i < n / 2; ++i) {
    for (size_t j = n / 2; j < n; ++j) {
      if (distrib(gen) % 2 == 0) {
        input["edges"].push_back({i, j});
      }
    }
  }

  auto res = cli->Post("/CheckBipartite", input.dump(), "application/json");
  if (!res) REQUIRE(false);

  nlohmann::json output = nlohmann::json::parse(res->body);

  REQUIRE(output["is_bipartite"].get<bool>() == true);
}
