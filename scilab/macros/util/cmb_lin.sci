function [x]=cmb_lin(alfa,x,beta,y)
//  x =cmb_lin(alfa,x,beta,y) evaluates alfa*x-beta*y 
// (low-level routine)
//!
// Copyright INRIA
n=prod(size(x));
for j=1:n,
    x(j)=addf(mulf(alfa,x(j)),mulf(mulf('-1',beta),y(j)));
end
endfunction
