function add_demo(title,path)
  global demolist
  path=pathconvert(path,%f,%t)
  if fileinfo(path)==[] then  error('file: '+path+' do not exist'),end
  if isdir(path) then 
    error('second argument should give the path to a scilab script file'),
  end
  k=find(demolist(:,1)==title)
  if k==[] then
    demolist=[demolist;title,path]
  else
    k1=find(demolist(k,2)==path)
    if k1==[] then demolist=[demolist;title+' ('+string(size(k,'*'))+')',path],end
  end
endfunction
