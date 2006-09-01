function [sr]=%p_r_lss(p,s)
// sr=%lssrp(p,s) ou sr=p/s
// s : state-space syslin list
// p : polynomial matrix
//!
//origine S Steer INRIA 1992
// Copyright INRIA
sr=tlist(['lss','A','B','C','D','X0','dt'],[],[],[],p,[],[])/s
endfunction
