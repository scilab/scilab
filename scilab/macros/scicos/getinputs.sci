function [x,y,t]=getinputs(o)
// Copyright INRIA
  execstr('[x,y,t]='+o.gui+'(''getinputs'',o)')
endfunction
