function D=%nodes_i_nodes(i,d,D)
  D.data(i)=d.data
  D.graphics(i)=d.graphics
  n=1:D.number;n(i)=1:d.number
  D.number=size(n,'*')
endfunction

