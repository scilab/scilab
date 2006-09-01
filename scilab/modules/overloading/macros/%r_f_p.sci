function f=%r_f_p(f,m)
// [f;m]
//!
// Copyright INRIA
f=rlist([f('num');m],[f('den');ones(m)],f('dt'))
endfunction
