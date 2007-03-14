function variable=%contents_i_variable(field,contlist,variable)
// Copyright INRIA
// V.C.

if field=="contents" then
  variable.infer.contents=contlist
else
  error(gettext("errors","m2sci_error_2"))
end
endfunction
