function sd=stdev(x,cr)
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
//author: carlos klimann
//
//date: 1999-05-12
//
  if argn(2)<2 then cr='*',end
  if x == [] then sd=%nan;return ;end 
  [m,n]=size(x);
  if cr=='*' then
    n=m*n
    select n
      case 0 then sd=%nan
      case 1 then sd=0
    else 
      sd=sqrt(sum((x-mean(x)).^2)/(n-1));
    end
  elseif cr=='c'|cr==2
    if n==1 then
      sd=zeros(m,1)
    else
      sd=sqrt(sum((x-mean(x,'c')*ones(x(1,:))).^2,'c')/(n-1));
    end
  elseif cr=='r'|cr==1
    if m==1 then
      sd=zeros(1,n)
    else
      sd=sqrt(sum((x-ones(x(:,1))*mean(x,'r')).^2,'r')/(m-1));
    end
  else
    error('Optional 2nd argument cr must be equal to ''*'', ''c'' or 2, ''r''or  1');
  end
endfunction
