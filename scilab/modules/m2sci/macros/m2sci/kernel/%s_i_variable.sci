function var=%s_i_variable(field,cste,var)
// Copyright INRIA
if field=="vtype" then
  var.infer.type.vtype=cste
elseif field=="property" then
  var.infer.type.property=cste
elseif field=="contents" then
  var.infer.contents=cste
else
  error("Not yet implemented...")
end
endfunction
