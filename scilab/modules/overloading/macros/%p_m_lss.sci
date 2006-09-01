function [s]=%p_m_lss(d2,s1)
//   [s]=%lssmp(d2,s1)  
//   d2*s1
//
//   s1 : state-space
//   d2 : polynomial matrix
//!
// origine s. steer inria 1992
//
// Copyright INRIA
s=tlist(['lss','A','B','C','D','X0','dt'],[],[],[],d2,[],[])*s1
endfunction
