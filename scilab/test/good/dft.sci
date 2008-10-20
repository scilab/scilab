function xf=dft(x,flag);
//xf=dft(x,flag);
//macro which computes dft of vector x
// x    :input vector
// flag :indicates dft or idft
// xf   :output vector
//!
//author: C. Bunks  date: 29 Sept 1988
// Copyright INRIA

n=maxi(size(x));
arg=(0:n-1);
am=-2*%pi*%i*arg'*arg/n;
if flag==1 then,
  am=-am;
end,
xf=exp(am)*matrix(x,n,1);
if flag==1 then,
  xf=xf/n;
end
endfunction
