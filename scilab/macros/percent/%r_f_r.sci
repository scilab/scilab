function [f]=%r_f_r(s1,s2)
//f=[s1;s2] 
//!
// Copyright INRIA
[s1,s2]=sysconv(s1,s2)
f=rlist([s1('num');s2('num')],[s1('den');s2('den')],s1('dt'))
endfunction
