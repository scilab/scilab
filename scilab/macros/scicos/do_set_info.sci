function [ok,new_info]=do_set_info(info)
// Copyright INRIA
// This function may be redefined by the user to handle definition 
// of the informations associated with the current diagram

if prod(size(info))==0 then
  info = list(' ')
end

new_info = x_dialog('Set Diagram informations',info(1))

if new_info==[] then 
  ok = %f
else
  ok = %t
  new_info = list(new_info)
end

endfunction
