function tt=sci2tcl(ttx)
// Copyright INRIA
  for t=['\',''"','[',']','{','}']
    ttx=strsubst(ttx,t,'\'+t)
  end
  //** strcat should be used : tt=strcat(ttx,"\n")
  if size(ttx,'*')<2 then tt=ttx,return,end
  tt=ttx(1)
  ttx=ttx(2:$)';
  for t=ttx
    tt=tt+'\n '+t
  end

endfunction
