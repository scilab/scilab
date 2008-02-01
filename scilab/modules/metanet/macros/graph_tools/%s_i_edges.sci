function D=%s_i_edges(i,d,D)
  if d<>[] then error('Invalid assignment'),end
  if i==[] return,end
  D.tail(i)=[] 
  D.head(i)=[]
  D.data(i)=[]
  D.graphics(i)=[]
endfunction
