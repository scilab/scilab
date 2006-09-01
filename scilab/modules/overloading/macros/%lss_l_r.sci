function [s]=%lss_l_r(s1,s2)
//!
// Copyright INRIA
[s1,s2]=sysconv(s1,s2)
s=s1\s2
endfunction
