function operation=%l_i_operatio(field,inslist,operation)
// Copyright INRIA
if field=="dims" then
  operation.out(1).dims=inslist
elseif field=="type" then
  operation.out(1).type=inslist
elseif field=="contents" then
  operation.out(1).infer.contents=inslist
else
  error("Not yet implemented...")
end
endfunction
