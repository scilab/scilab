function f=%r_q_s(f1,n2)
//  r.\m
//!
// Copyright INRIA
if size(n2,'*')==0 then f=[],return,end
f=rlist(f1('den').*n2,f1('num').*ones(n2),f1('dt'))
endfunction
