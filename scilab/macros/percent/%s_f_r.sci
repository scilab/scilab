function f=%s_f_r(m,f)
// [m;f]
//!
// Copyright INRIA
f=rlist([m;f('num')],[ones(m);f('den')],f('dt'))
endfunction
