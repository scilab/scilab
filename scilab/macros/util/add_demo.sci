function add_demo(title,path)
  global demolist
  path=pathconvert(path,%f,%t)
  k=find(demolist(:,1)==title)
  if k==[] then
    demolist=[demolist;title,path]
  else
    k1=find(demolist(k,1)==path)
    if k1==[] then demolist=[demolist;title+' ('+string(size(k,'*'))+')',path],end
  end
endfunction
