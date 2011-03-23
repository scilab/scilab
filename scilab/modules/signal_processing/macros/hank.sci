// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) INRIA - 1989 - G. Le Vey 
// 
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at    
// http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt

function [hk]=hank(m,n,cov)
//<hk>=hank(m,n,cov)
//this macro builds the hankel matrix of size (m*d,n*d)
//from the covariance sequence of a vector process
//   m  : number of bloc-rows
//   n  : number of bloc-columns
//   cov: sequence of covariances; it must be given as :[R0 R1 R2...Rk]
//   hk : computed hankel matrix
//!

   hk=[];
   d=min(size(cov));
   for k=0:m-1,hk=[hk;cov(:,(k*d+1):(k+n)*d)];end;
endfunction
