function add_palette(title,path)
  global scicos_pal
  path=pathconvert(path,%f,%t)
  
  k=find(scicos_pal(:,1)==title)
  if k==[] then
    scicos_pal=[scicos_pal;title,path]
  else
    k1=find(scicos_pal(k,1)==path)
    if k1==[] then scicos_pal=[scicos_pal;title+' ('+string(size(k,'*'))+')',path],end
  end
endfunction
