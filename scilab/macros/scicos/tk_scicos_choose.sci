function result = tk_scicos_choose(mat,lab,but);
// Copyright INRIA
//** 17 April 2007: name changed from "tk_choose" to "tk_scicos_choose"
  result = 0
  
  l=list()
  for t=mat(:)'
    l($+1)=t,
  end
  Cmenu = mpopup(l);
  
  if Cmenu==[] then return, end
  
  result = find(Cmenu==mat)
  result = result(1)
endfunction

