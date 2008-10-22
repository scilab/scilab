
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2002-2008 - INRIA - Serge STEER <serge.steer@inria.fr>
//
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt

function GraphList=ge_complete_defaults(GraphList)

  if GraphList.nodes.graphics.defaults.diam==[] then
    GraphList.nodes.graphics.defaults.diam=15
  end
  if GraphList.nodes.graphics.defaults.border==[]
    GraphList.nodes.graphics.defaults.border=1
  end
  if GraphList.nodes.graphics.defaults.font==[] then
    GraphList.nodes.graphics.defaults.font=[1 6 -1];
  end
  if GraphList.edges.graphics.defaults.width==[] then
    GraphList.edges.graphics.defaults.width=1
  end

  nn=node_number(GraphList)
  na=edge_number(GraphList)
  
  if GraphList.nodes.graphics.name==[] then
    GraphList.nodes.graphics.name=string(1:nn)'
  end
  if GraphList.nodes.graphics.type==[] then
    GraphList.nodes.graphics.name=zeros(1,nn)
  end
 
  if GraphList.nodes.graphics.colors==[] then
    GraphList.nodes.graphics.colors=zeros(2,nn)
  end
  if GraphList.nodes.graphics.type==[] then
    GraphList.nodes.graphics.type=zeros(nn,1)
  end
  if GraphList.nodes.graphics.diam==[] then
    GraphList.nodes.graphics.diam=zeros(1,nn)
  end
  
  if GraphList.nodes.graphics.border==[] then
    GraphList.nodes.graphics.border=zeros(1,nn)
  end
  
  if GraphList.nodes.graphics.font==[] then
    GraphList.nodes.graphics.font=zeros(3,nn)
  end

  F=getfield(1,GraphList.nodes.data);F=F(2:$);
  for f=F
    if GraphList.nodes.data(f)==[] then
      GraphList.nodes.data(f)=zeros(1,nn)
    end 
  end

  if GraphList.edges.graphics.name==[] then
    GraphList.edges.graphics.name=string(1:na)
  end
  if GraphList.edges.graphics.foreground==[] then
    GraphList.edges.graphics.foreground=zeros(1,na)
  end
  if GraphList.edges.graphics.width==[] then
    GraphList.edges.graphics.width=zeros(1,na)
  end
  if GraphList.edges.graphics.font==[] then
    GraphList.edges.graphics.font= zeros(3,na)
  end
  
  F=getfield(1,GraphList.edges.graphics)
  if or(F=='profile_index') then
    if GraphList.edges.graphics.profile_index==[] then
      loops=find(GraphList.edges.tail==GraphList.edges.head)
      GraphList.edges.graphics.profile_index(loops)=1;
      others=find(GraphList.edges.tail<>GraphList.edges.head)
      GraphList.edges.graphics.profile_index(others)=GraphList.edges.graphics.defaults.profile_index
    end
    if GraphList.edges.graphics.profiles==list() then
      GraphList.edges.graphics.profiles=list(1.2,ge_default_edge_profile())
    end
  else
    F=[F(:);'profile_index';'profiles'];
    edges=GraphList.edges
    setfield(1,F,edges);GraphList.edges=edges
    GraphList.edges.graphics.profile_index=bool2s(GraphList.edges.tail==GraphList.edges.head)+1
    GraphList.edges.graphics.profile=list(1,2,ge_default_edge_profile())
  end
  F=getfield(1,GraphList.edges.data);F=F(2:$);
  for f=F
    if GraphList.edges.data(f)==[] then
      GraphList.edges.data(f)=zeros(1,na)
    end 
  end
  
endfunction
