
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2002-2008 - INRIA
//
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt

function g=add_edge(i,j,g)
// Add edges in the given graph. 
// The edges to add are specified by their tail and head node index:
// (i(k), j(k)) defines an edge
// i  : vector of tail node index.
// j  : vector of head node index. 
// g  : graph data structure

  //Check inputs
  //------------
  if argn(2)==2 then //add_edge(ij,g)
    g=j
    ij=i
    if size(ij,1)<>2 then
      error('The first argument must be a matrix with two rows')
    end
  elseif argn(2)==3 then //add_edge(i,j,g)
    if size(i,'*')<>size(j,'*') then 
      error('The first two arguments should have the same size')
    end
    ij=[i(:);j(:)]
  end
  //- check g
  check_graph(g,%f)
  //- check ij
  if or(ij<1|ij>node_number(g)) then
    error('Node numbers should be in [1 '+string(node_number(g))+']')
  end

  // adding  edges from nodes i to nodes j
  //--------------------------------------
  ma=size(g.edges);
  m1=size(ij,2) //number of added edges
  //- tail and head
  g.edges.tail=[g.edges.tail ij(1,:)]; 
  g.edges.head=[g.edges.head ij(2,:)];
  //- graphic properties
  if or(getfield(1,g.edges)=='graphics') then
    // set correponding graphic fields to their default values
    if g.edges.graphics.name<>[]       then 
      g.edges.graphics.name=[g.edges.graphics.name string(ma+(1:m1))];
    end;
    if g.edges.graphics.foreground<>[] then 
      g.edges.graphics.foreground=[g.edges.graphics.foreground zeros(1,m1)];
    end;
    if g.edges.graphics.width<>[]      then 
      g.edges.graphics.width=[g.edges.graphics.width zeros(1,m1)];
    end;
    if g.edges.graphics.font<>[]       then 
      g.edges.graphics.font=[g.edges.graphics.font zeros(3,m1)];
    end;
    if g.edges.graphics.profile_index<>[] then 
      g.edges.graphics.profile_index=[g.edges.graphics.profile_index zeros(1,m1)];
    end;
  end
  //- associated data
  if or(getfield(1,g.edges)=='data') then
    // extend data with the value of the last edge
    for k=1:m1
      g.edges.data($+1)=g.edges.data($)
    end
  end
endfunction
