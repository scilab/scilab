function f=%r_c_s(f,m)
// f=%r_c_s(r,m) <=> f=[r, m]   [rational, constant]
//!
// Copyright INRIA
f=rlist([f('num'),m],[f('den'),ones(m)],f('dt'))
endfunction
