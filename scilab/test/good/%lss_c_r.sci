function [s]=%lss_c_r(s1,s2)
//s=%lss_c_r(s1,s2) iff s=[s1,s2]
// Copyright INRIA
[s1,s2]=sysconv(s1,s2);s=[s1,s2];
endfunction
