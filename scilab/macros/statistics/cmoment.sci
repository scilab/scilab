function mom= cmoment(x,ord,orien)
//
//This function computes the central moment of all orders of a vector or
//matrix x.
//
//For a vector or matrix x, mom=cmoment(x,ord) returns in the scalar mom
//the moment of order ord of all the entries of x.
//
//mom=cmoment(x,ord,'r')(or, equivalently, mom=cmoment(x,ord,1)) returns
//in  each entry of the  row vector mom the moment  of order ord of each
//column of x.
//
//mom=cmoment(x,ord,'c')(or, equivalently, mom=cmoment(x,ord,2)) returns
//in each entry of the column vector mom the moment of order ord of each
//row of x.
//
//Nota: In the   calculations the  divisor is  n-1  ,  where  n  is  the
//dimension of the data vector.
//
//References:  Wonacott, T.H. & Wonacott, R.J.; Introductory
//Statistics, J.Wiley & Sons, 1990.
//
//
//author: carlos klimann
//
//date: 1999-05-31
//
  if x==[]|ord==[] then mom=%nan, return, end
  [lhs,rhs]=argn(0)
  if rhs<2 then error('cmoment requires at least two parameters.'), end
  if rhs==2 then
    if ord==1 then mom=0, return, end
    le=length(x)
    m=sum(x)/le
    mom=sum((x-m).^ord)/(le)
    return
  elseif rhs==3 then
    le=size(x,orien)
    m=mean(x,orien)
    if orien=='r'|orien==1 then
      m=m(ones(le,1),:)
    elseif orien=='c'|orien==1 then
      m=m(:,ones(le,1))
    else error('The third parameter must be ''r'', ''c'', 1 or 2'),
    end
    mom=sum((x-m).^ord,orien)/(le)
    return
  else error('The function moment must have two or three parameters')
  end
endfunction
