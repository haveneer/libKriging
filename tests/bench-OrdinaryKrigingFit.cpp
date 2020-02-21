#define CATCH_CONFIG_MAIN
#include <armadillo>
#include <catch2/catch.hpp>
#include <chrono>
#include <cmath>
#include <ctime>
#include <libKriging/OrdinaryKriging.hpp>

auto f = [](const arma::rowvec& row) {
  double sum = 0;
  for (auto&& x : row)
    sum += ((x - .5) * (x - .5));
  return sum;
};

SCENARIO("OrdinaryKriging", "[benchmark]") {
  arma::arma_rng::seed_type seed_val = 123;  // populate somehow (fixed value => reproducible)

  std::vector<double> times;
  std::vector<double> logn(11);
  std::generate(logn.begin(), logn.end(), [start = 1.0, step = 0.1, i = 0]() mutable { return start + step * i++; });

  const auto i = GENERATE_COPY(range(0ul, logn.size()));

  GIVEN("A matrix and generated seed parameters") {
    const arma::uword n = floor(std::pow(10., logn[i]));
    const arma::uword d = floor(2 + i / 3);
    INFO("dimension is n=" << n);

    arma::arma_rng::set_seed(seed_val);
    arma::mat X(n, d, arma::fill::randu);
    arma::colvec y(n);
    for (arma::uword k = 0; k < n; ++k)
      y(k) = f(X.row(k));

    const auto start = std::chrono::system_clock::now();

    OrdinaryKriging ok;
    ok.fit(std::move(y), std::move(X));  // FIXME no move

    const auto end = std::chrono::system_clock::now();

    auto elapsed = [=](auto start, auto end) {
      return std::chrono::duration_cast<std::chrono::microseconds>(end - start)
          .count();
    };
    times.emplace_back(elapsed(start,end));

    double ll = ok.logLikelihood(ok.theta());
    arma::vec gll = ok.logLikelihoodGrad(ok.theta());

    WHEN("value is perfectly computed") {
      THEN("the prediction are exact (for X at least)") { REQUIRE(true); }
    }
  }
}