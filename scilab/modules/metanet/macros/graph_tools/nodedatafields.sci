function F=nodedatafields(g)
  if or(getfield(1,g.nodes)=='data') then 
    F=getfield(1,g.nodes.data)
    F=F(2:$)
  else
    F=[]
  end
endfunction
