function [s]=%r_s_lss(s1,s2)
//s=%r_s_lss(s1,s2) ou s=s1-s2
// s1 : transfer
// s2 : state-space
//!
// Copyright INRIA
[s1,s2]=sysconv(s1,s2)
s=s1-s2
endfunction
