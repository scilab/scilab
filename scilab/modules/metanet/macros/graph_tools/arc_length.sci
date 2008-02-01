function len=arc_length(g)
// returns the length of the arcs of the graph g
  ma=edge_number(g)
  if or(getfield(1,g.edges.data)=='length')& g.edges.data.length<>[] then 
    //there is a a data field for arc lengths
    mmi=min(g.edges.data.length); mma=max(g.edges.data.length);
    if (mmi<=0|mma<=0) then
      len=ones(1,ma)
    else
      len=g.edges.data.length;
    end
  else
    len=ones(1,ma)
  end
  if ~g.directed then
    len=[len len]
  end
endfunction
