function [x]=cmb_lin(Alfa,x,Beta,y)
//  x =cmb_lin(Alfa,x,Beta,y) evaluates Alfa*x-Beta*y 
// (low-level routine)
//!
// Copyright INRIA
n=prod(size(x));
for j=1:n,
    x(j)=addf(mulf(Alfa,x(j)),mulf(mulf('-1',Beta),y(j)));
end
endfunction
