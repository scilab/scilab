function funcall=%l_i_funcall(field,inslist,funcall)
// Copyright INRIA
// V.C.

if field=="dims" then
  funcall.lhs(1).dims=inslist
elseif field=="type" then
  funcall.lhs(1).type=inslist
else
  error("Not yet implemented...")
end
endfunction
