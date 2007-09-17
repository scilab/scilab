function [x]=cmb_lin(alfa,x,Beta,y)
//  x =cmb_lin(alfa,x,Beta,y) evaluates alfa*x-Beta*y 
// (low-level routine)
//!
// Copyright INRIA
n=prod(size(x));
for j=1:n,
    x(j)=addf(mulf(alfa,x(j)),mulf(mulf('-1',Beta),y(j)));
end
endfunction
