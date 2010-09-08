// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2000 - INRIA - Carlos Klimann
// 
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
// 

function sd=stdev(x,o)
//
//This function computes  the  standard deviation  of  the values  of  a
//vector or matrix x.
//
//For a vector or a  matrix x, s=stdev(x)  returns in  the scalar s  the
//standard deviation of all the entries of x.
//
//s=stdev(x,'r')  (or,  equivalently,   s=stdev(x,1))   is the   rowwise
//standard deviation. It returns  in each entry of the  row vector s the
//standard deviation of each column of x.
//
//s=stdev(x,'c')  (or,  equivalently, s=stdev(x,2))  is   the columnwise
//standard  deviation. It returns in each  entry of the  column vector y
//the standard deviation of each row of x.
//
//
  if argn(2)<2 then o='*',end
  if x == [] then sd=%nan;return ;end 
  
  if typeof(x)=='hypermat' then sd=%hm_st_deviation(x,o),return,end

  // remove the mean
  if o=='*' then
    y=x - mean(x)
  elseif o=='c'|o==2 then
    y=x - mean(x,'c')*ones(x(1,:))
  elseif o=='r'|o==1 then
    y=x - ones(x(:,1))*mean(x,'r')
  else
    error(msprintf(gettext("%s: Wrong value for input argument #%d: ''%s'', ''%s'', ''%s'', %d or %d expected.\n"),"stdev",2,"*","c","r",1,2)),
  end
  
  if size(x,o)==1 then
    sd=0*y
  else
    sd=sqrt(sum(y.^2,o)/(size(x,o)-1));
  end

endfunction
