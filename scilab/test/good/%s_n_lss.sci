 function [r]=%s_n_lss(s1,s2)
//%s_n_lss(s1,s2) <=> s1<>s2   gain<>state-space
//!
// Copyright INRIA
  r=s2(2)<>[]|s2(5)<>s1
endfunction
