function op=%s_i_operatio(field,cste,op)
// Copyright INRIA
if field=="vtype" then
  op.type.vtype=cste
elseif field=="property" then
  op.type.property=cste
else
  error(gettext("errors","m2sci_error_2"))
end
endfunction
