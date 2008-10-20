function s=%r_v_lss(s1,s2)
//s=%r_v_lss(s1,s2) <=> s= s1/.s2
//!
// origine s. steer inria 1988
//
// Copyright INRIA
[s1,s2]=sysconv(s1,s2);s=s1/.s2;
endfunction
