function [s]=stdevf(x,fre,orien)
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
//each  entry of the row   vector s of   type 1xsize(x,'c') the standard
//deviation of  each    column of  x,    each value  counted   with  the
//multiplicity indicated by the corresponding value of fre.
//
//s=stdevf(x,fre,'c')(or, equivalently, s=stdevf(x,fre,2))  returns   in
//each entry  of the column vector s  of type size(x,'c')x1 the standard
//deviation of  each row of x, each  value counted with the multiplicity
//indicated by the corresponding value of fre.
//
//author: carlos klimann
//
//date: 2000-01-20
//
  if x==[] then s=%nan, return, end
  [lhs,rhs]=argn(0)
  if rhs<2|rhs>3 then
    error('stdevf requires two or three inputs.'),
  end
  if x==[]|fre==[]|fre==0, s=%nan;return,end
  if rhs==2 then
    s=sqrt(sum(((x-meanf(x,fre)).^2).*fre))/(sum(fre)-1),
    return,
  end
  if orien=='*',
    s=sqrt(sum(((x-meanf(x,fre)).^2).*fre))/(sum(fre)-1),
  elseif orien=='r'|orien==1,
    s=sqrt(sum(((x-ones(size(x,'r'),1)*meanf(x,fre,'r')).^2).*fre))./ ..
      (sum(fre,'r')-1)
  elseif orien=='c'|orien==2,
    s=sqrt(sum((x-(meanf(x,fre,'c')*ones(1,size(x,'c')))).^2,'c'))./..
      (sum(fre,'c')-1)
  else error('3rd argument of stdevf must be equal to *, c, r, 1 or 2'),
  end
endfunction
