
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2000 - INRIA - Carlos Klimann
// 
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
// 


function [m]=nanmean(x,orient)
//
//This function returns in scalar m the mean of the values (ignoring the
//NANs) of  a  vector  or  matrix  x.
//
//For  a vector   or  matrix x,  m=nanmean(x) or  m=nanmean(x,'*')
//returns in scalar m the mean of all the entries (ignoring the NANs) of
//x.
//
//m=nanmean(x,'r')  (or,   equivalently, m=nanmean(x,1))  returns in
//each entry of  the row vector  m  of type 1xsize(x,'c')  the mean of
//each column of x (ignoring the NANs).
//
//m=nanmeanf(x,'c')(or, equivalently, m=nanmean(x,2))  returns    in
//each entry of the column vector m of  type size(x,'c')x1 the mean of
//each row of x (ignoring the NANs).
//
//
  if argn(2)==0 then error(msprintf(gettext("%s: Wrong number of input arguments: %d to %d expected.\n"),"nanmean",1,2)), end
  if argn(2)==1 then  orient='*',end
  //replace nans by 0
  isn=isnan(x)
  x(isn)=0

  N=size(x,orient)-sum(bool2s(isn),orient)
  allnans=find(N==0)
  N(allnans)=1
  m = sum(x,orient)./N;
  m(allnans)=%nan

endfunction
