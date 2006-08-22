function [v]=mvvacov(x)
//
//This    function    computes    v,   the    matrix    of
//variance-covariance of the "tableau" x (x is a numerical
//matrix nxp)  who gives  the values of  p variables for n
//individuals:   the    (i,j)   coefficient   of    v   is
//v(i,j)=E(xi-xibar)(xj-xjbar),  where   E  is  the  first
//moment of a variable, xi  is the i-th variable and xibar
//the mean of the xi variable.
//
//References: Saporta, Gilbert, Probabilites,  Analyse des
//Donnees et Statistique, Editions Technip, Paris, 1990.
//Mardia,  K.V., Kent,  J.T. &  Bibby,  J.M., Multivariate
//Analysis, Academic Press, 1979.
//
//Date: 2001-10-03, Fixed: 2002-05-30
//
//Author: Carlos Klimann
//
  if x==[] then s=%nan, return, end
  [lhs,rhs]=argn(0)
  if rhs<>1 then error('mvvacov requires one input.'), end
  [n p]=size(x);
  fact=1/n
  v=fact*((x'*x)-(fact*(x'*ones(n,1)*ones(1,n))*x))
endfunction

