function s1=%r_q_r(s1,s2)
// %r_q_r(s1,s2) <=> s1= s1.\s2   for rationals
//!
// Copyright INRIA
[s1,s2]=sysconv(s1,s2)
[num,den]=simp(s1('den').*s2('num'),s1('num').*s2('den'))
s1=rlist(num,den,s1('dt'))
endfunction
