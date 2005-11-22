function x=fft2(varargin)
// Copyright INRIA
// Two-dimensional fast Fourier transform
// Syntax : y = fft2(x) or y = fft2(x,m,n)
// Inputs :
// x : scalar, vector, matrix, array (real or complex) 
// m, n : numbers (respectively) of rows and colums of x which used for the perfom of DFT,if the rows number respectively (columns) of x is more than m then x is truncated in order to have m rows, else if the rows (respectively columns) number of x is less than m then x rows are completed by 0 to have m rows.
//
// Outputs :
// y : scalar, vector, matrix, array (real or complex), if there is one input argument x then y and x have the same size, else if there are 3 inputs arguments then the sizes of the first and second dimension of y are equal to m and n, the others dimension sizes are equal to the size of x
// F.B

if size(varargin) == 1 then
  a = varargin(1);
  if type(a) == 1 then
    x = fft(a);
  elseif typeof(a) == 'hypermat' then
    dims = matrix(x.dims,-1,1);
    v = matrix(x.entries,-1,1);
    incr = 1;
    for k=1:2
      dk = double(dims(k));
      v = fft(v ,-1,dk,incr);
      incr = incr*dk;
    end
    x = matrix(v,double(dims));
  end
elseif size(varargin) == 3 then
  a = varargin(1);
  m = varargin(2);
  n = varargin(3);
  asize1 = size(a,1);
  asize2 = size(a,2);
  if type(a) == 1 then
    x(1:min(m,asize1),1:asize2)=a(1:min(m,asize1),1:asize2);
  elseif typeof(a) == 'hypermat' then
    dims = matrix(a.dims,-1,1);
    dims = double(dims);
    dims(1) = m;
    dims(2) = n;
    x=hypermat(dims)
    for i=1:prod(dims(3:$))
      x(1:min(m,asize1),1:min(n,asize2),i)=a(1:min(m,asize1),1:min(n,asize2),i);
    end
  end
  x=fft2(x);
else error("wrong number of input arguments");
end
endfunction