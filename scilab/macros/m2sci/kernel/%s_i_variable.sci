function var=%s_i_variable(field,cste,var)
// Copyright INRIA
if field=="vtype" then
  var.infer.type.vtype=cste
elseif field=="property" then
  var.infer.type.property=cste
else
  error("Not yet implemented...")
end
endfunction
