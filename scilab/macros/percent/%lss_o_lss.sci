function [r]=%lss_o_lss(s1,s2)
//%lss_o_lss(s1,s2) test s1==s2
// Copyright INRIA
for k=2:7,r=and(s1(k)==s2(k));if ~r then return,end,end
endfunction
