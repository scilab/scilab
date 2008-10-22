
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 1999 - INRIA - Carlos Klimann
// 
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
// 


function [m]=nanmeanf(val,fre,orient)
//
//This function returns in scalar m the mean of the values (ignoring the
//NANs) of  a  vector  or  matrix  val, each counted  with   a frequency
//signaled by the  corresponding values of the  integer vector or matrix
//fre with the same type of val.
//
//For     a   vector   or      matrix   val,   m=nanmeanf(val,fre)    or
//m=nanmeanf(val,fre,'*')    returns in scalar  m  the   mean of all the
//entries (ignoring   the NANs) of   val, each  value  counted  with the
//multiplicity indicated by the corresponding value of fre.
//
//m=nanmeanf(val,fre,'r')(or,     equivalently,   m=nanmeanf(val,fre,1))
//returns in each entry of the row vector m  of type 1xsize(val,'c') the
//mean of each column  of  val (ignoring  the NANs), each  value counted
//with the multiplicity indicated by the corresponding value of fre.
//
//m=nanmeanf(val,fre,'c')(or,   equivalently,     m=nanmeanf(val,fre,2))
//returns in each  entry of the column  vector m of type size(val,'c')x1
//the mean of  each row of val  (ignoring the NANs),  each value counted
//with the multiplicity indicated by the corresponding value of fre.
//
//
  [lhs,rhs]=argn(0)
  if rhs<2|rhs>3 then error(msprintf(gettext("%s: Wrong number of input arguments: %d to %d expected.\n"),"nanmeanf",2,3)), end
  if val==[]|fre==0 then m=[], return,end
  isn=isnan(val)
  fre(isn)=0
  val(isn)=0
  if rhs==2 then
    m=sum(val.*fre)/sum(fre)
  elseif orient=='*' then
    m=sum(val.*fre)/sum(fre)
  elseif orient=='r'|orient=='c'|orient==1|orient==2 then
    m=sum(val.*fre,orient)./sum(fre,orient)
  else error(msprintf(gettext("%s: Wrong value for input argument #%d: ''%s'', ''%s'', ''%s'', %d or %d expected.\n"),"nanmeanf",3,"*","r","c",1,2)),
  end
endfunction
