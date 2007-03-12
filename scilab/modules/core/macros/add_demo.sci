function add_demo(demotitle,path)
  global demolist
  path=pathconvert(path,%f,%t)
  if fileinfo(path)==[] then  error(gettext('errors','core_error_274')+path+gettext('errors','core_error_275')),end
  if isdir(path) then 
    error(gettext('errors','core_error_273'));
  end
  k=find(demolist(:,1)==demotitle)
  if k==[] then
    demolist=[demolist;demotitle,path]
  else
    k1=find(demolist(k,2)==path)
    if k1==[] then demolist=[demolist;demotitle+' ('+string(size(k,'*'))+')',path],end
  end
  clear demolist
endfunction
