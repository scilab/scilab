function [s]=%lss_m_p(s1,d2)
//   [s]=%lss_m_p(s1,d2)  
//   s=s1*d2
//
//   s1 : state-space
//   d2 : polynomial matrix
//!
// origine s. steer inria 1992
//
// Copyright INRIA
s=s1*tlist(['lss','A','B','C','D','X0','dt'],[],[],[],d2,[],[])
endfunction
