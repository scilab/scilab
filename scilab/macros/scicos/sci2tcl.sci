function tt=sci2tcl(ttx)
// Copyright INRIA
  for t=['\',''"','[',']','{','}']
    ttx=strsubst(ttx,t,'\'+t)
  end 
  if size(ttx,'*')<2 then tt=ttx,return,end
  tt=ttx(1)
  ttx=ttx(2:$)';
  for t=ttx
    tt=tt+'\n '+t
  end

endfunction
