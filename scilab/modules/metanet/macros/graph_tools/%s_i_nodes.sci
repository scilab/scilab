function D=%s_i_nodes(i,d,D)
  if d<>[] then error('Invalid assignment'),end
  if i==[] return,end
  D.data(i)=[]
  D.graphics(i)=[]
  D.number=D.number-size(i,'*')
endfunction
