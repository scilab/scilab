function funcall=%type_i_funcall(field,inslist,funcall)
// Copyright INRIA

if field=="type" then
  funcall.lhs(1).type=inslist
else
  error("%type_i_funcall(): not yet implemented...")
end
endfunction
