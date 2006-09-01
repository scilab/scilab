function [s]=%lss_m_r(s1,s2)
//s=%lss_m_r(s1,s2) <=> s= s2*s1
//!
// Copyright INRIA
[s1,s2]=sysconv(s1,s2);s=s1*s2
endfunction
