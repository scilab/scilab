function [s]=%lss_f_r(s1,s2)
// Copyright INRIA
[s1,s2]=sysconv(s1,s2);s=[s1;s2];
endfunction
