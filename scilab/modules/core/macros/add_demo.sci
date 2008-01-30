function add_demo(demotitle,path)
  global demolist
  path=pathconvert(path,%f,%t)
  if fileinfo(path)==[] then  error(msprintf(gettext("%s: The file %s does not exist.\n"),"add_demo",path)),end
  if isdir(path) then 
    error(msprintf(gettext("%s: Wrong second input argument: Path to a scilab script file expected.\n"),"add_demo"));
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
