function s=%lss_v_r(s1,s2)
//s=%lss_v_r(s1,s2) <=> s=s1/.s2 :feedback operation
//!
// origine s. steer inria 1988
// Copyright INRIA
[s1,s2]=sysconv(s1,s2);s=s1/.s2;
endfunction
