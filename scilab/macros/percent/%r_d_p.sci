function r=%r_d_p(r,m)
// r=r./p
//!
// Copyright INRIA
r=rlist(r('num').*ones(m),r('den').*m,r('dt'))
endfunction
