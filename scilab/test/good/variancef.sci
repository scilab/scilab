
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2000 - INRIA - Carlos Klimann
// 
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
// 

function [s]=variancef(x,fre,orien)
//
//This function  computes  the variance  of the values  of   a vector or
//matrix x, each  of  them  counted with  a  frequency signaled   by the
//corresponding values of the integer vector or matrix fre with the same
//type of x.
//
//For a vector or matrix  x, s=variancef(x,fre) (or s=variancef(x,fre,'*') returns
//in scalar s the variance  of all the  entries of x, each value counted
//with the multiplicity indicated by the corresponding value of fre.
//
//s=variancef(x,fre,'r')(or,   equivalently, s=variancef(x,fre,1)) returns in each
//entry of the row vector s  of type 1xsize(x,'c')  the variance of each
//column of x, each value counted with the multiplicity indicated by the
//corresponding value of fre.
//
//s=variancef(x,fre,'c')(or, equivalently,   s=variancef(x,fre,2)) returns in each
//entry of  the column vector  s of type   size(x,'c')x1 the variance of
//each row of  x, each value counted with  the multiplicity indicated by
//the corresponding value of fre.
//
//
  if x==[] then s=%nan, return, end
  [lhs,rhs]=argn(0)
  if rhs<2|rhs>3 then
    error(msprintf(gettext("%s: Wrong number of input arguments: %d to %d expected.\n"),"variancef",2,3)),
  end
  if x==[]|fre==[]|fre==0, s=%nan;return,end
  if rhs==2 then
    sumfre=sum(fre)
    if sumfre <= 1 then error(msprintf(gettext("%s: Wrong value for input argument #%d: Must be > %d.\n"),"variancef", 2, 1)), end
    s=(sum(((x-meanf(x,fre)).^2).*fre))/(sumfre-1),
    return,
  end
  if orien=='*',
    sumfre=sum(fre)
    if sumfre <= 1 then error(msprintf(gettext("%s: Wrong value for input argument #%d: Must be > %d.\n"),"variancef", 2, 1)),end
    s=(sum(((x-meanf(x,fre)).^2).*fre))/(sumfre-1),
  elseif orien=='r'|orien==1,
    sumfre=sum(fre,'r')
    if or(sumfre==0) then error(msprintf(gettext("%s: Wrong value for input argument #%d: Must be > %d.\n"),"variancef",2,1)),end
    s=(sum(((x-ones(size(x,'r'),1)*meanf(x,fre,'r')).^2).*fre))./ ..
      (sumfre-1)
  elseif orien=='c'|orien==2,
    sumfre=sum(fre,'c')
    if or(sumfre==0) then error(msprintf(gettext("%s: Wrong value for input argument #%d: Must be > %d.\n"),"variancef",2,1)),end
    s=(sum((x-(meanf(x,fre,'c')*ones(1,size(x,'c')))).^2,'c'))./..
      (sumfre-1)
  else error(msprintf(gettext("%s: Wrong value for input argument #%d: ''%s'', ''%s'', ''%s'', %d or %d expected.\n"),"variancef",3,"*","c","r",1,2))
  end
endfunction
