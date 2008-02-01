function g=add_edge_data(g,name,value)
//Author Serge Steer, 2008, 
//copyright INRIA
  F=getfield(1,g.edges.data)
  if and(F<>name) then 
    data=g.edges.data
    setfield(1,[F name],data)
    g.edges.data=data
  end
  if argn(2)==3 then 
    if size(value,'*')<>edge_number(g) then
      error('the third argument should be a vector with '+ ...
	    string(edge_number(g))+' elements')
    end
    g.edges.data(name)=matrix(value,1,-1)
  end
endfunction
