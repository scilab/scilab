function s=%lss_t(s)
//s=%lss_t(s)  <=> s=s'  in state-space
//!
// Copyright INRIA
[a,b,c,d,x,dom]=s(2:7)
s=tlist(['lss','A','B','C','D','X0','dt'],a',c',b',d',x,dom)
endfunction
