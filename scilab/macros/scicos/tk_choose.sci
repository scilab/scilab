function result=tk_choose(mat,lab,but);
  result=0
  
  l=list()
  for t=mat(:)'
    l($+1)=t,
  end
  Cmenu=mpopup(l);
  if Cmenu==[] then return,end
  result=find(Cmenu==mat)
  result=result(1)
endfunction

