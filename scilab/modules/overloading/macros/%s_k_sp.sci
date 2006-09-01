function r=%s_k_sp(a,b)
// a.*.b with a full b sparse
// Copyright INRIA
if a==[] then r=[],return,end
r=sparse(a).*.b
endfunction
