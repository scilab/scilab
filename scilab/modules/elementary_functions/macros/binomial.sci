function [P] = binomial(p,N)
  //
  //  PURPOSE
  //     Compute probabilities of the binomial law B(N,p)
  //     
  //  PARAMETERS
  //     p : a real in [0,1] 
  //     N : an integer >= 0
  //     P : a row vector with N+1 components and :
  //     
  //        P(k+1) = P(X=k) = C(k,N) (1-p)^k p^(N-k)
  //
  //  Rewritten by Bruno for a gain in speed (by using
  //  cdfbin which computes the cumulative probability)
  //

  if type(p) ~= 1 | type(N) ~= 1 then , error(" bad type argument(s) ") , end
  if length(p) ~= 1 | length(N) ~= 1 then , error(" argument(s) must be scalar(s) ") , end
  if  p < 0  | p > 1 then , error(" p out of range (p must be in [0,1]) ") , end
  if  N < 1  |  floor(N)-N ~= 0 then , error(" N must be an integer >= 1") , end
 
  un = ones(1,N+1);
  P = cdfbin("PQ", 0:N, N*un, p*un, (1-p)*un)
  P(2:N+1) = P(2:N+1) - P(1:N)
endfunction  
