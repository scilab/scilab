
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2000 - INRIA - Carlos Klimann
// 
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
// 

function rho=correl(x,y,fre)
//
//This function computes  the correlation of two variables x
//and y where x is  a numerical vector of length  n, y is  a
//numerical  vector  of length  m  and fre   is an array  of
//dimensions nxm. In    fre  the element  of   indices (i,j)
//corresponds to the number or frequences of x_i&y_j.
//
//References: Wonacott,  T.H. & Wonacott, R.J.; Introductory Statistics,
//J.Wiley & Sons, 1990.
//
//
  if x==[] | y==[] then s=%nan; return, end
  [lhs,rhs]=argn(0)
  if rhs <> 3 then error(msprintf(gettext("%s: Wrong number of input arguments: %d expected.\n"),"correl",3)), end
  [lfre,cfre]=size(fre);
  [lx,cx]=size(x)
  [ly,cy]=size(y)
  
  if lx<>1 & cx<>1 then error(msprintf(gettext("%s: Wrong type for input argument #%d: Vector expected.\n"),"correl",1)), end
  if ly<>1 & cy<>1 then error(msprintf(gettext("%s: Wrong type for input argument #%d: Vector expected.\n"),"correl",2)), end
  lx=lx*cx;
  cy=ly*cy;

  
  if lx<>lfre then error(msprintf(gettext("%s: Wrong value for input argument #%d: Same number of line as first input argument expected.\n"),"correl",3)), end
  if cy<>cfre then error(msprintf(gettext("%s: Wrong value for input argument #%d: Same number of column as first input argument expected.\n"),"correl",3)), end
  x=matrix(x,lx,1);
  y=matrix(y,1,cy);
  fr=fre/sum(fre)
  px=sum(fr,'c')
  py=sum(fr,'r')
  mx=sum(x .* px)
  my=sum(y .* py)
  sx=sqrt(sum(((x-mx).^2).*px))
  sy=sqrt(sum(((y-my).^2).*py))
  sxy=sum(((x-mx)*ones(1,cy)).*fr.*(ones(lx,1)*(y-my)))
  rho=sxy/(sx*sy)
endfunction
