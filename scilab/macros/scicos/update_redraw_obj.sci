function scs_m=update_redraw_obj(scs_m,path,o)
// Copyright INRIA
if length(path)==2 then
  if typeof(o)=='Link'|typeof(o)=='Text' then
    drawobj(scs_m(path))
    scs_m(path)=o
    drawobj(scs_m(path))
  else
    scs_m=changeports(scs_m,path,o)
  end
else // change a block in a sub-level
  scs_m(path)=o
end
endfunction
