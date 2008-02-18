
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2000 - INRIA - Carlos Klimann
// 
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
// 


function [m]=nanmedian(x,orient)
//
//For a vector or a matrix x, [m]=median(x) returns in  the vector p the
//median of the values (ignoring the  NANs) of vector x.
//
//[m]=median(x,'r')  (or, equivalently, [m]=pctl(x,1))  are the  rowwise
//medians. It returns in each  position of the row  vector m the medians
//of data (ignoring the NANs) in the corresponding column of x.
//
//[m]=median(x,'c')   (or,  equivalently,   [m]=median(x,2))    are  the
//columnwise   percentiles.  It returns in  each  position of the column
//vector m the medians of data (ignoring the  NANs) in the corresponding
//row of x.
//
//
  [lhs,rhs]=argn(0)
  if type(x)<>1 then error('first entry to nanmedian must be numeric'), end
  if rhs<1|rhs>2 then error('nanmedian requires one or two inputs.'), end
  if x==[] then m=[], return,end
  if rhs==1 then 
    p=perctl(x(~isnan(x)),50)
    if p==[] then p=%nan,end
    m=p(1)
  elseif orient=='r'|orient==1 then
    m=[]
    for i=x 
      p=perctl(i(~isnan(i)),50)
      if p==[] then p=%nan,end
      m=[m p(1)]
    end
  elseif orient=='c'|orient==2 then
    m=[]
    for i=x' do
      p=perctl(i(~isnan(i)),50)
      if p==[] then p=%nan,end
      m=[m;p(1)];
    end
  else ('Third parameter must be r, c, 1 or 2')
  end
endfunction
