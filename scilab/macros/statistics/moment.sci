function [mom]= moment(x,ord,orien)
//
//This function computes  the  non  central  moment of all  orders  of a
//vector or matrix x.
//
//For a vector or matrix x, mom=moment(x,ord) returns in the scalar mom
//the moment of order ord of all the entries of x.
//
//mom=moment(x,ord,'r')(or,  equivalently,  mom=moment(x,ord,1)) returns
//in each  entry of the row  vector mom the  moment of order ord of each
//column of x.
//
//mom=moment(x,ord,'c')(or, equivalently,  mom=cmoment(x,ord,2)) returns
//in each entry of the column vector mom the moment of order ord of each
//row of x.
//
//Nota: In the calculations  the divisor is  n, where n is the dimension
//of the data vector.
//
//References:  Wonacott, T.H. & Wonacott, R.J.; Introductory
//Statistics, J.Wiley & Sons, 1990.
//
//author: carlos klimann
//
//date: 1999-06-09
//
  [lhs,rhs]=argn(0)
  if rhs==2 then
    if x==[] then mom=%nan, return, end
    le=length(x)
    mom=sum(x.^ord)/le
  elseif rhs==3 then
    if x==[] then mom=%nan, return, end
    le=size(x,orien)
    mom=sum((x.^ord),orien)/le
  else 
    error('The function moment must have two or three parameters')
  end
endfunction
