function [r]=%lss_n_lss(s1,s2)
//%lss_n_lss(s1,s2) : inequality test s1<>s2
//!
// Copyright INRIA
for k=2:7,r=or(s1(k)<>s2(k));if r then return,end,end
endfunction
