function [pop_out] = codage_identity(pop_in,direction,param)
// pop_in is a list().
// pop_out must be a list().
if (direction=='code') then
  pop_out = pop_in;
elseif (direction=='decode') then
  pop_out = pop_in;
else
  error('codage_identity: wrong direction');
end
endfunction
