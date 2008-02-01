function xyp=ge_get_edge_profile(n)
  prof=GraphList.edges.graphics.profile_index
  if prof==[] then
    index=0
  else
    index=GraphList.edges.graphics.profile_index(n)
  end
  if index==0 then
    xyp=ge_default_edge_profile()
  else
    xyp=GraphList.edges.graphics.profiles(index)
  end
endfunction
