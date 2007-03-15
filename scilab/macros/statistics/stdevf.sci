function [s]=stdevf(x,fre,o)
//
//This function  computes the  standard  deviation  of  the values of  a
//vector or matrix x, each of them  counted with a frequency signaled by
//the corresponding values of the integer vector  or matrix fre with the
//same type of x.
//
//For  a  vector or matrix   x,  s=stdevf(x,fre) (or s=stdevf(x,fre,'*')
//returns in scalar s the  standard deviation of all  the entries of  x,
//each  value counted   with    the   multiplicity indicated    by   the
//corresponding value of fre.
//
//s=stdevf(x,fre,'r')(or,   equivalently, s=stdevf(x,fre,1)) returns  in
//each  entry of the row   vector s  the standard
//deviation of  each    column of  x,    each value  counted   with  the
//multiplicity indicated by the corresponding value of fre.
//
//s=stdevf(x,fre,'c')(or, equivalently, s=stdevf(x,fre,2))  returns   in
//each entry  of the column vector s  the standard
//deviation of  each row of x, each  value counted with the multiplicity
//indicated by the corresponding value of fre.
//
//author: carlos klimann
//
//date: 2000-01-20
//
  rhs=argn(2)
  if rhs<2 then
    error('stdevf requires two or three inputs.'),
  elseif rhs==2 then
     o='*'
  end
  if or(size(x)<>size(fre)) then
    error('stdevf, first and second argument mist have the same dimensions')
  end
  if x==[] then s=%nan;return,end
  
 
  //remove the median
  if o=='*' then
    y=x - meanf(x,fre)
  elseif o=='r'|o==1,
    y=x - ones(size(x,o),1)*meanf(x,fre,o)
  elseif o=='c'|o==2,
    y=x - meanf(x,fre,o)*ones(1,size(x,o))
  else 
    error('3rd argument of stdevf must be equal to *, c, r, 1 or 2'),
  end
  if size(x,1)==1 then
    s=0*sum((y.^2).*fre,o)
  else
    s=sqrt(sum((y.^2).*fre,o)./(sum(fre,o)-1));
  end
 
endfunction
