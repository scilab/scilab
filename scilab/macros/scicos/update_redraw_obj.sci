function scs_m=update_redraw_obj(scs_m,k,o)
// Copyright INRIA
if size(k)==1 then
  if o(1)=='Link'|o(1)=='Text' then
    drawobj(scs_m(k))
    scs_m(k)=o
    drawobj(scs_m(k))
  else
    scs_m=changeports(scs_m,k,o)
  end
else // change a block in a sub-level
  scs_m(k)=o
end
