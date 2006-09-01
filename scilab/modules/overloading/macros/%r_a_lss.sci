function s=%r_a_lss(s1,s2)
//s=%r_a_lss(s1,s2) <=> s=s1+s2  rational + state-space
//!
// origine s. steer inria 1988
//
// Copyright INRIA
[s1,s2]=sysconv(s1,s2);s=s1+s2;
endfunction
