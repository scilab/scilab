function add_help_chapter(title,path)
  global %helps
  path=pathconvert(path,%f,%t)
  if ~isdir(path) then 
    error('second argument should give the path to a directory'),
  end

  k=find(%helps(:,2)==title)
  if k==[] then
    %helps=[%helps;path,title]
  else
    k1=find(%helps(k,1)==path)
    if k1==[] then %helps=[%helps;path,title+' ('+string(size(k,'*'))+')'],end
  end
endfunction
