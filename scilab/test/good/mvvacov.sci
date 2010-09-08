
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2001 - INRIA - Carlos Klimann
// 
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
// 


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
//
  if x==[] then s=%nan, return, end
  [lhs,rhs]=argn(0)
  if rhs<>1 then error(msprintf(gettext("%s: Wrong number of input argument: %d expected.\n"),"mvvacov",1)), end
  [n p]=size(x);
  fact=1/n
  v=fact*((x'*x)-(fact*(x'*ones(n,1)*ones(1,n))*x))
endfunction

