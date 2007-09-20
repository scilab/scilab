function palettes=add_palette(palettes,path,kpal)
// Copyright INRIA
  path=stripblanks(path)
  for k=size(palettes)+1:kpal, palettes(k)=list(),end

  [ok,scs_m,cpr,edited]=do_load(path,'palette')
  if ok & size(scs_m.objs)>0 then 
    palettes(kpal)=scs_m;
  else 
    palettes(kpal)=list(),
  end

endfunction
