function add_demo(demotitle,path)
  global demolist
  path=pathconvert(path,%f,%t)
  if fileinfo(path)==[] then  error('file: '+path+' do not exist'),end
  if isdir(path) then 
    error('second argument should give the path to a scilab script file'),
  end
  k=find(demolist(:,1)==demotitle)
  if k==[] then
    demolist=[demolist;demotitle,path]
  else
    k1=find(demolist(k,2)==path)
    if k1==[] then demolist=[demolist;demotitle+' ('+string(size(k,'*'))+')',path],end
  end
endfunction
