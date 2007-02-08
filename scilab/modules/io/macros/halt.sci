function []=halt(varargin)
//halt() stops execution until something is entered in the keyboard.
//!
// Copyright INRIA

[lhs,rhs]=argn(0);
if (rhs == 0) then 
  msg = 'halt';
else
  if (rhs == 1) then
    msg=string(varargin(1));
  else
    error('incorrect parameter(s). see help halt',999);
  end
end
mprintf(msg);mscanf('%c')
endfunction
