#ifndef LIBKRIGING_DEMODATAFRAME_HPP
#define LIBKRIGING_DEMODATAFRAME_HPP

#include <armadillo>

#include "mapbox/variant.hpp"

#include "libKriging/libKriging_exports.h"

class DemoDataFrame {
 public:
  using DataFrame = std::vector<mapbox::util::variant<arma::vec, arma::ivec>>;

 public:
  /** \deprecated Demo only */
  LIBKRIGING_EXPORT explicit DemoDataFrame(const DataFrame &M);
};

#endif  // LIBKRIGING_DEMODATAFRAME_HPP
