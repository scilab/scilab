function f=%r_f_s(f,m)
// [f;m]   [rational;constant]
//!
// Copyright INRIA
f=rlist([f('num');m],[f('den');ones(m)],f('dt'))
endfunction
