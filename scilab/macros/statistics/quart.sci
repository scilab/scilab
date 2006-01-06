function q=quart(x,orien)
//
//This function computes the quartiles of a vector or a matrix x.
//
//For a vector  or a matrix x, q=quart(x)  returns  in the vector q  the
//three quartiles entries of x.
//
//q=quart(x,'r')  (or,   equivalently,  q=quart(x,1))   is  the  rowwise
//quartiles range.  It returns in each  column of the 3xlength(x)-matrix
//q, the quartiles of the corresponding column of x.
//
//q=quart(x,'c')  (or, equivalently,   q=quart(x,2))  is the  columnwise
//interquartile range. It returns  in  each row of  length(x)x3-matrix q
//the quartiles of each row of x.
//
//author: carlos klimann
//
//date: 1999-05-12
//
  function y = vperctl(x,p)
  //inline function wich computes percentiles of a vector
    k=isnan(x)
    x(k)=[];
    n=size(x,'*')
    x=gsort(x,'g','i')
    data=[0     100*(0.5:(n-0.5))./n  100;
	  x(1), matrix(x,1,-1),       x(n)]
    y=interpln(data,p);
  endfunction

  if x==[] then q=%nan, return, end
  [lhs,rhs]=argn(0)
  if rhs==0 then error('quart requires at least one input.'), end
  if rhs==1 then
    q=vperctl(x(:),[25 50 75])
    q=q(:)
  else
    q=[]
    if orien=='r'|orien==1 then
      for i=x
	p=vperctl(i,[25 50 75])
	q=[q p(:)]
      end
    else
      for i=x'
	p=vperctl(i,[25 50 75])
	q=[q;p(:)']
      end
    end
  end
endfunction
