function [sr]=%lss_r_p(s,p)
// sr=%lss_r_p(s,p) <=> sr=s/p
// s : syslin list
// p : polynomial matrix
//!
// Copyright INRIA
sr=s/tlist(['lss','A','B','C','D','X0','dt'],[],[],[],p,[],[])
endfunction
