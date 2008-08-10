
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 1999 - INRIA - Carlos Klimann
// 
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
// 

function [m]=meanf(val,fre,orient)
//
//This function returns in scalar m the  mean of the  values of a vector
//or matrix   val,  each  counted  with a  frequency   signaled  by  the
//corresponding values of the integer vector or matrix fre with the same
//type of val.
//
//For  a vector or matrix  val, m=meanf(val,fre) or m=meanf(val,fre,'*')
//returns in scalar m the  mean of all the  entries  of val, each  value
//counted with the multiplicity indicated  by the corresponding value of
//fre.
//
//m=meanf(val,fre,'r')(or, equivalently, m=meanf(val,fre,1))  returns in
//each entry of  the row vector m  of type  1xsize(val,'c') the mean  of
//each column of val, each value counted with the multiplicity indicated
//by the corresponding value of fre.
//
//m=meanf(val,fre,'c')(or, equivalently, m=meanf(val,fre,2)) returns  in
//each entry of the column vector m of  type size(val,'c')x1 the mean of
//each row of val, each value counted with the multiplicity indicated by
//the corresponding value of fre.
//
//References:  Wonacott, T.H. & Wonacott, R.J.; Introductory
//Statistics, J.Wiley & Sons, 1990.
//
  [lhs,rhs]=argn(0)
  if rhs==0|rhs==1|rhs>=4 then error(msprintf(gettext("%s: Wrong number of input argument: %d to %d expected.\n"),"meanf",2,3)), end

  if val==[]|fre==[]|fre==0, m=%nan;return,end
  if rhs==2 then
    m=sum(val .* fre)/sum(fre);
  elseif rhs==3 then
    if orient=='*' then
      m=sum(val .* fre)/sum(fre)
    elseif orient=='r'|orient==1 then
      m=sum(val .* fre,'r') ./ sum(fre,'r')
    elseif orient=='c'|orient==2 then
      m=sum(val .* fre,'c') ./ sum(fre,'c')
    else
	  error(msprintf(gettext("%s: Wrong value for input argument #%d: ''%s'', ''%s'',''%s'', %d or %d.\n"),"meanf",3,"r","c",1,2)),
    end
  end
endfunction
