function [x,y]=getorigin(o)
// Copyright INRIA
//execstr('[x,y]='+o(5)+'(''getorigin'',o)')
execstr('[x,y]='+o.gui+'(''getorigin'',o)')
endfunction
