function [sr]=%p_l_lss(p,s)
//sr=%p_l_lss(p,s) <=> sr=p\s
// p polynomial mat.
// s state-space syslin list
//!
//origine S Steer INRIA 1992
// Copyright INRIA
sr=tlist(['lss','A','B','C','D','X0','dt'],[],[],[],p,[],[])\s
endfunction
