function [sr]=%lss_v_p(s,p)
// feedback sr=(eye+s*p)\s
//s=%lss_v_p(s,p) <=> sr=s/.p
// p : polynomial matrix
// s : state-space syslin list
//!
//origine S Steer INRIA 1992
// Copyright INRIA
sr=s/.tlist(['lss','A','B','C','D','X0','dt'],[],[],[],p,[],[])
endfunction
