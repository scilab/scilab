function [s]=%lss_r_lss(s1,s2)
//
// Copyright INRIA
[s1,s2]=sysconv(s1,s2)
s=s1*inv(s2)
endfunction
