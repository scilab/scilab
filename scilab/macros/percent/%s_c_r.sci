function f=%s_c_r(m,f)
// %s_c_r(M,r)=[M,r]
//!
// Copyright INRIA
f=rlist([m,f('num')],[ones(m),f('den')],f('dt'))
endfunction
