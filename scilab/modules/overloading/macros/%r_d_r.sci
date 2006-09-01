function f=%r_d_r(s1,s2)
// f=s1./s2
//!
// Copyright INRIA
[s1,s2]=sysconv(s1,s2)
[num,den]=simp(s1('num').*s2('den'),s1('den').*s2('num'))
f=rlist(num,den,s1('dt'))
endfunction
