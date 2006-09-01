function [sr]=%s_r_lss(p,s)
// sr=%s_r_lss(p,s) ou sr=p/s
// s : syslin list
// p : constant matrix
//!
//origine S Steer INRIA 1992
// Copyright INRIA
sr=tlist(['lss','A','B','C','D','X0','dt'],[],[],[],p,[],[])/s
endfunction
