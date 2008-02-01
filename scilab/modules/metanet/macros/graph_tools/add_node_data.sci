function g=add_node_data(g,name,value)
//Author Serge Steer, 2008, 
//copyright INRIA
  F=getfield(1,g.nodes.data)
  if and(F<>name) then 
    data=g.nodes.data
    setfield(1,[F name],data)
    g.nodes.data=data
  end
  if argn(2)==3 then 
    if size(value,'*')<>g.nodes.number then
      error('the third argument should be a vector with '+ ...
	    string(g.nodes.number)+' elements')
    end
    g.nodes.data(name)=matrix(value,1,-1)
  end
endfunction
