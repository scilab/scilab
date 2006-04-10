function add_help_chapter(helptitle,path)
  global %helps
  path=pathconvert(path,%f,%t)
  if ~isdir(path) then 
    error('second argument should give the path to a directory'),
  end

  k=find(%helps(:,2)==helptitle)
  if k==[] then
    %helps=[%helps;path,helptitle]
  else
    k1=find(%helps(k,1)==path)
    if k1==[] then %helps=[%helps;path,helptitle+' ('+string(size(k,'*'))+')'],end
  end
endfunction
