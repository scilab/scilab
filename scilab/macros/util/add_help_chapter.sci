function add_help_chapter(title,path)
  global %helps
  path=pathconvert(path,%t,%t)
  k=find(%helps(:,2)==title)
  if k==[] then
    %helps=[%helps;path,title]
  else
    k1=find(%helps(k,1)==path)
    if k1==[] then %helps=[%helps;path,title+' ('+string(size(k,'*'))+')'],end
  end
endfunction
