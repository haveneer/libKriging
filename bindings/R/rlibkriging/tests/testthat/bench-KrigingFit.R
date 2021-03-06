library(testthat)

f <- function(X) apply(X, 1, function(x) prod(sin((x-.5)^2)))
n <- 500
set.seed(123)
X <- DiceDesign::lhsDesign(n,dimension=6)$design #cbind(runif(n),runif(n))
y <- f(X)
d = ncol(X)

N = 1
times <- list(R=rep(NA, N), cpp=rep(NA, N))

for (i in 1:N) {
    set.seed(i)
    times$R[i]   = system.time(
                        k <- DiceKriging::km(design=X,response=y,covtype = "gauss",
                         multistart = 1,control = list(trace=T,maxit=10)) #,lower=rep(0.001,d),upper=rep(2*sqrt(d),d))
                    )
    times$cpp[i] = system.time(
                        r <- kriging(y, X,"gauss","constant",FALSE,"BFGS","LL",
                            # to let start optim at same initial point
                            parameters=list(sigma2=0,has_sigma2=FALSE,theta=matrix(k@parinit,ncol=d),has_theta=TRUE))
                    )


  ll_cpp <- kriging_logLikelihood(r, kriging_model(r)$theta)
  e <- new.env()
  ll_R <- DiceKriging::logLikFun(k@covariance@range.val, k, e)
  
  gll_cpp <- kriging_logLikelihoodGrad(r, kriging_model(r)$theta)
  gll_R <- DiceKriging::logLikGrad(k@covariance@range.val, k, e)
    
  if (abs(ll_cpp - DiceKriging::logLikFun(param=as.numeric(kriging_model(r)$theta),model=k))/ll_cpp>.1)
    stop("LL function is not the same bw DiceKriging/libKriging: ",kriging_logLikelihood(r,kriging_model(r)$theta)," vs. ",DiceKriging::logLikFun(param=as.numeric(kriging_model(r)$theta),model=k))  
  
  if ((ll_cpp - ll_R)/ll_R < -.01 )
    warning("libKriging LL ",ll_cpp," << DiceKriging LL ",ll_R)

  if ((ll_R - ll_cpp)/ll_R < -.01 )
    warning("DiceKriging LL ",ll_R," << libKriging LL ",ll_cpp)
}

plot(times$R,times$cpp)
abline(a=0,b=1,col='red')
abline(a=0,b=2,col='red')
abline(a=0,b=0.5,col='red')


