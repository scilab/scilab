function palettes=add_palette(palettes,path,kpal)
// Copyright INRIA
path=stripblanks(path(:))
n=size(path,1)
for k=size(palettes)+1:max(kpal), palettes(k)=list(),end
for k=1:n
  [ok,scs_m,cpr,edited]=do_load(path(k),'palette')
  if ok then palettes(kpal(k))=scs_m;end
end
