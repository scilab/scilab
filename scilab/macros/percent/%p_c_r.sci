function f=%p_c_r(m,f)
// f= %p_c_r(m,f) <=> r= [m,f]  m:polynomial mat. r: rational mat.
//!
// Copyright INRIA
f=rlist([m,f('num')],[ones(m),f('den')],f('dt'))
endfunction
