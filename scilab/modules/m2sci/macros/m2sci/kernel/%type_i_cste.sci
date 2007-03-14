function cste=%type_i_cste(field,inslist,cste)
// Copyright INRIA

if field=="type" then
  cste.infer.type=inslist
else
  error(gettext("errors","m2sci_error_2"))
end
endfunction
