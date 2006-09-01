function r=%r_d_s(r,m)
// r=r./m
//!
// Copyright INRIA
if size(m,'*')==0 then r=[],return,end
r=rlist(r('num').*ones(m),r('den').*m,r('dt'))
endfunction
