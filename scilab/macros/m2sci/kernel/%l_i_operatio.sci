function operation=%l_i_operatio(field,inslist,operation)
// Copyright INRIA
// V.C.

if field=="dims" then
  operation.out(1).dims=inslist
elseif field=="type" then
  operation.out(1).type=inslist
else
  error("Not yet implemented...")
end
endfunction
