function pt=gridpoint(pt)
//find the nearest grid point for a given point.
//!
// Copyright INRIA
xo=gridvalues(1);yo=gridvalues(2);dx=gridvalues(3);dy=gridvalues(4);
pt(1)=round((pt(1)-xo)/dx)*dx
pt(2)=round((pt(2)-yo)/dy)*dy
endfunction
