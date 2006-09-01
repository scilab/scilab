function f=%p_f_r(m,f)
// [m;f]   [polynomial;rational]
//!
// Copyright INRIA
f=rlist([m;f('num')],[ones(m);f('den')],f('dt'))
endfunction
