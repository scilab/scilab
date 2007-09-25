function cste=%type_i_cste(field,inslist,cste)
// Copyright INRIA

if field=="type" then
  cste.infer.type=inslist
else
  error(gettext("errors","Not yet implemented."))
end
endfunction
