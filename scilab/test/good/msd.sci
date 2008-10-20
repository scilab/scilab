function m=msd(x,orien)
//
//This function computes  the mean squared  deviation of the values of a
//vector or matrix x.
//
//For a vector or a matrix x, m=msd(x) returns in  the scalar y the mean
//squared deviation of all the entries of x.
//
//m=msd(x,'r') (or,   equivalently,   m=msd(x,1)) is the   rowwise  mean
//squared  deviation. It returns  in each entry of the  row vector y the
//mean squared deviation of each column of x.
//
//m=msd(x,'c') (or,   equivalently, m=msd(x,2)) is  the  columnwise mean
//squared deviation. It returns in each entry of the column vector y the
//mean squared deviation of each row of x.
//
//Reference: Wonacott  T.H.& Wonacott  R.J. .-  Introductory Statistics,
//5th edition, John Wiley, 1990.
//
//author: carlos klimann
//date: 1999-06-18
//
  rhs=argn(2)
  if rhs==0 then error('msd requires at least one input.'), end
  if x==[] then s=%nan, return, end
  if rhs==1 then orien='*',end
  if orien=='*' then
    m=sqrt(sum((x-mean(x)).^2)/size(x,orien));
  elseif orien=='c'|orien==2 then
    m=sqrt(sum((x-mean(x,orien)*ones(x(1,:))).^2,orien)/size(x,orien));
  elseif orien=='r'|orien==1 then
    m=sqrt(sum((x-ones(x(:,1))*mean(x,orien)).^2,orien)/size(x,orien));
  else
    error('2nd argument of msd must be equal to ''c'', ''r'', 1 or 2');
  end
endfunction
