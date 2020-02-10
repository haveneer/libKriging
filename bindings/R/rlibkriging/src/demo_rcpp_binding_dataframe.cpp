#include <RcppArmadillo.h>

#include "libKriging/demo/DemoDataFrame.hpp"
#include "type.hpp"

void display(const DemoDataFrame::DataFrame::value_type & v) {
  v.match([](const arma::vec & v) {  std::cout << "arma::vec" << '\n'; 
    for(auto && e : v) std::cout << e << '\n'; std::cout << '\n'; },
          [](const arma::ivec & v) {  std::cout << "arma::ivec" << '\n'; 
    for(auto && e : v) std::cout << e << '\n'; std::cout << '\n'; } );
}

auto RcppDataFrame2CppDataFrame(Rcpp::DataFrame DF, bool copy_mem = true) -> DemoDataFrame::DataFrame {
    DemoDataFrame::DataFrame DFx;
    DFx.reserve(DF.size());

    for(R_xlen_t i=0;i<DF.size();++i) {
    auto && field = DF[i];
    std::cout << TYPEOF(field) << '\n';
    
    switch (TYPEOF(field)) {
    case INTSXP: {
      Rcpp::IntegerVector v = field;
      if (copy_mem)
        DFx.emplace_back(Rcpp::as<arma::ivec>(v));
      else
        DFx.emplace_back(arma::ivec(v.begin(), v.size(), false));
    } break;
    case REALSXP: {
      Rcpp::NumericVector v = field;
      if (copy_mem)
        DFx.emplace_back(Rcpp::as<arma::vec>(v));
      else
        DFx.emplace_back(arma::vec(v.begin(), v.size(), false));
    } break;
    default:
      Rcpp::stop("Input dataframe field must be a Numeric or Integer Vector");
    }
    display(DFx.back());
  }
  return DFx;
}



// [[Rcpp::export]]
Rcpp::List buildDemoDataFrame(Rcpp::DataFrame DF, bool copy_mem = true) {
  Rcpp::XPtr<DemoDataFrame> impl_ptr(new DemoDataFrame{RcppDataFrame2CppDataFrame(DF,copy_mem)});

  Rcpp::List obj;
  obj.attr("impl") = impl_ptr;
  obj.attr("class") = "DemoDataFrame";

  return obj;
}

/*
// [[Rcpp::export]]
arma::vec getEigenValues(Rcpp::List obj) {
  if (!obj.inherits("DemoArmadillo"))
    Rcpp::stop("Input must be a DemoArmadillo object");

  SEXP impl = obj.attr("impl");
  Rcpp::XPtr<DemoArmadilloClass> impl_ptr(impl);
  return impl_ptr->getEigenValues();
}
 */