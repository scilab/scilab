function x=%r_det(a)
// Copyright INRIA
[n,d]=lcmdiag(a);
x=determ(n)/determ(d);
endfunction
