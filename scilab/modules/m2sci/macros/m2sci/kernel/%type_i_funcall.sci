function funcall=%type_i_funcall(field,inslist,funcall)
// Copyright INRIA

if field=="type" then
  funcall.lhs(1).type=inslist
else
  error(gettext("errors","m2sci_error_2"))
end
endfunction
