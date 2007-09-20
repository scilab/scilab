function ok=set_cmap(cmap)
//appends new colors to the colormap
// Copyright INRIA
  if cmap==[] then ok=%t,return,end
  d=xget('colormap');  
  for k=1:size(cmap,1)
    [mc,kk]=mini(abs(d-ones(size(d,1),1)*cmap(k,:))*[1;1;1])
    if mc>.0001 then
      d=[d;cmap(k,:)]
    end
  end
  ierr=execstr('set(gcf(),'"color_map'",d)','errcatch')
  if ierr<>0 then
    ok=%f
  else
    ok=%t
  end
endfunction
