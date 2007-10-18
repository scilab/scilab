function cste=%l_i_cste(field,inslist,cste)
// Copyright INRIA

if field=="dims" then
  cste.infer.dims=inslist
elseif field=="type" then
  cste.infer.type=inslist
else
  error(gettext("Not yet implemented."))
end
endfunction
