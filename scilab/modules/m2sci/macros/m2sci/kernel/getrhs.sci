function [varargout] = getrhs(instr_tree)
// Copyright INRIA

varargout=list()

if typeof(instr_tree)<>"funcall" then
  error(gettext("errors","m2sci_error_31"))
end

rhsnb=size(instr_tree.rhs)

for k=1:rhsnb
  varargout(k)=instr_tree.rhs(k)
end

endfunction
