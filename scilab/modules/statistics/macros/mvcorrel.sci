// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2001 - INRIA - Carlos Klimann
// 
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
// 


function [r]=mvcorrel(x)
//
//This function  computes r, the matrix  of correlation of
//the "tableau" x (x is  a numerical matrix mxn) who gives
//the values  of n variables for m  individuals: the (i,j)
//coefficient              of             r             is
//v(i,j)=E(xi-xibar)(xj-xjbar)/(stdi*stdj), where E is the
//first  moment of a  variable, xi  is the  i-th variable,
//xibar the mean of the  xi variable and stdi the standard
//deviation of the i-th variable.
//
//References: Saporta, Gilbert, Probabilites,  Analyse des
//Donnees et Statistique, Editions Technip, Paris, 1990.
//
//
  if x==[] then s=%nan; return, end
  [lhs,rhs]=argn(0)
  if rhs <> 1 then error(msprintf(gettext("%s: Wrong number of input argument: %d expected.\n"),"mvcorrel",1)), end
  [lx cx]=size(x)
  if lx==1 then r=zeros(lx,cx), return, end
  xbar=sum(x,'r')/lx
  r=x-ones(lx,1)*xbar
  std=(sum(r .^2,'r')) .^ .5
  r=(r'*r) ./ (std'*std)
endfunction
