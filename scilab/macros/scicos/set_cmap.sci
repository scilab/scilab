function ok=set_cmap(cmap)
//appends new colors to the colormap
// Copyright INRIA
if cmap==[] then ok=%t,return,end
if xget('use color')==1 then
  d=xget('colormap');  
  for k=1:size(cmap,1)
    [mc,kk]=mini(abs(d-ones(size(d,1),1)*cmap(k,:))*[1;1;1])
    if mc>.0001 then
      d=[d;cmap(k,:)]
    end
  end
  errcatch(9999,'continue')
  xset('colormap',d);
  errcatch(-1)
  if iserror(9999) then
    errclear(9999)
    ok=%f
  else
    ok=%t
  end
end

