function [hk]=hank(m,n,cov)
//<hk>=hank(m,n,cov)
//this macro builds the hankel matrix of size (m*d,n*d)
//from the covariance sequence of a vector process
//   m  : number of bloc-rows
//   n  : number of bloc-columns
//   cov: sequence of covariances; it must be given as :[R0 R1 R2...Rk]
//   hk : computed hankel matrix
//!
//author: G. Le Vey  Date: 16 Febr. 1989
// Copyright INRIA

   hk=[];
   d=mini(size(cov));
   for k=0:m-1,hk=[hk;cov(:,(k*d+1):(k+n)*d)];end;
endfunction
