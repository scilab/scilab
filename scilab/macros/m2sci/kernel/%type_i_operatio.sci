function operation=%type_i_operatio(field,inslist,operation)
// Copyright INRIA

if field=="type" then
  operation.out(1).type=inslist
else
  error("%type_i_operatio(): not yet implemented...")
end
endfunction
