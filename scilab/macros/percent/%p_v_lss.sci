function [sr]=%p_v_lss(p,s)
//  feedback sr=(eye+p*s)\p
//s=%p_v_lss(p,s) <=> sr=p/.s
// p : polynomial matrix
// s : state-space syslin list
//!
// Copyright INRIA
sr=tlist(['lss','A','B','C','D','X0','dt'],[],[],[],p,[],[])/.s
endfunction
