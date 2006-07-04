function [x,y,t]=getoutputs(o)
// Copyright INRIA
  execstr('[x,y,t]='+o.gui+'(''getoutputs'',o)')
endfunction
