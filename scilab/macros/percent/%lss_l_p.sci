function [sr]=%lss_l_p(s,p)
//sr=%lss_l_p(s,p) <=> sr=s\p
// p polynomial matrix
// s syslin list
//!
//origine S Steer INRIA 1992
// Copyright INRIA
sr=s\tlist(['lss','A','B','C','D','X0','dt'],[],[],[],p,[],[])
endfunction
