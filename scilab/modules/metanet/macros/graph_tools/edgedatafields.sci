function F=edgedatafields(g)
  if or(getfield(1,g.edges)=='data') then 
    F=getfield(1,g.edges.data)
    F=F(2:$)
  else
    F=[]
  end
endfunction
