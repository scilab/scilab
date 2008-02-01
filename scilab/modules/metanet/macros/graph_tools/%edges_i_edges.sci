function D=%edges_i_edges(i,d,D)
  D.tail(i)=d.tail 
  D.head(i)=d.head
  D.data(i)=d.data
  D.graphics(i)=d.graphics
endfunction
