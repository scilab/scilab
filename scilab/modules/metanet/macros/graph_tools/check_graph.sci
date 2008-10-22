
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2002-2008 - INRIA - Serge STEER <serge.steer@inria.fr>
// Copyright (C) 2002-2008 - INRIA - Claude GOMEZ <claude.gomez@inria.fr>
//
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt

function check_graph(g,checkgraphics)
// checks if g is really a graph data structure
  if argn(2)==1 then checkgraphics=%t, end
  
  // The fields, types and size reference
  // ------------------------------------
  // The ttgraph, ttnodes, ttdefnodes, ttedges, ttdefedges are copied from ge_new_graph

  // - main graph data structure
  ttgraph = ['graph','version','name','directed','nodes','edges'];
  GraphFieldTypes=['string','string','constant','nodes','edges']
  GraphFieldSizes=list([1 1],[1 1],[1 1],-1,-2)

  //Checking graph data structure
  //-----------------------------
  if type(g)<>16|typeof(g)<>'graph' then
    error('Graph list check: Invalid data type')
  end
  if or(getfield(1,g)<>ttgraph) then
    error('Graph list check: invalid graph fields')
  end
  for k=2:size(ttgraph,2)
    gk=g(ttgraph(k))
    if typeof(gk)<>GraphFieldTypes(k-1) then
      error('Graph list check: incorrect type for field '''+ttgraph(k)+'''')
    end
    sgk=size(gk)
    if prod(sgk)<>0 then
      if or(GraphFieldSizes(k-1)>0&sgk<>GraphFieldSizes(k-1)) then
	error('Graph list check: incorrect dimension for field '''+ttgraph(k)+'''')
      end
    end
  end
  th=[g.edges.tail g.edges.head]
  if min(th)<=0|max(th)>g.nodes.number then
    error('Graph list check: invalid edges')
  end
  if and(g.directed<>[0 1]) then
    error('Graph list check: ""directed"" must be 0 or 1')
  end
  check_nodes(g.nodes,checkgraphics)
  check_edges(g.edges,checkgraphics)
endfunction


function check_nodes(nodes,checkgraphics)
  
  
// -  main nodes data structure
  ttnodes=           ['nodes','number','graphics','data'];
  NodeFieldTypes=    ['constant','ngraphic','nodedata']
  NodeFieldSizes=    list([1 1],-1,-1);
  // - graphic nodes data structure
  ttgnodes=          ['ngraphic','display','defaults','name','x','y','type','diam','border','colors','font','displaymode']
  GNodeFieldTypes=   ['string','nodedefs','string','constant','constant','constant','constant','constant','constant','constant','string']
  GNodeFieldSizes=   list([1 1],6,[1 -1],[1 -1], [1 -1],[1 -1], [1 -1],[1 -1],[2 -1],[3 -1],[1 1])
  // - default node graphic properties data structure
  ttdefnodes=        ['nodedefs','type','diam','border','colors','font']
  DefNodeFieldTypes= ['constant', 'constant', 'constant', 'constant', 'constant']
  DefNodeFieldSizes= list([1 1], [1 1], [1,1],[2 1], [3 1])

  //Checking main node data structure
  //---------------------------------
  if or(getfield(1,nodes)<>ttnodes) then
    error('Graph list check: invalid graphic node fields')
  end
  
  n=nodes.number
  for k=2:size(ttnodes,2)
    nk=nodes(ttnodes(k))
    if typeof(nk)<>NodeFieldTypes(k-1) then
      error('Graph list check: incorrect type for nodes field '''+ttnodes(k)+'''')
    end
    snk=size(nk)
    if prod(snk)<>0 then
      if or(NodeFieldSizes(k-1)>0&size(nk)<>NodeFieldSizes(k-1)) then
	error('Graph list check: incorrect dimension for nodes field '''+ttnodes(k)+'''')
      end
      //cross dimension validation
      if or(NodeFieldSizes(k-1)==-1) then
	if n<>snk(NodeFieldSizes(k-1)==-1) then
	  error('Graph list check: incorrect dimension for nodes field '''+ttnodes(k)+'''')
	end
      end
    end
  end
  if ~checkgraphics then return,end
  
  //Checking graphic node data structure  
  //------------------------------------
  if or(getfield(1,nodes.graphics)<>ttgnodes) then
    error('Graph list check: invalid node fields')
  end
  for k=2:size(ttgnodes,2)
    nk=nodes.graphics(ttgnodes(k))
    if typeof(nk)<>GNodeFieldTypes(k-1) then
      error('Graph list check: incorrect type for graphic nodes field '''+ttgnodes(k)+'''')
    end
    snk=size(nk)
    if prod(snk)<>0 then
      if or(GNodeFieldSizes(k-1)>0&size(nk)<>GNodeFieldSizes(k-1)) then
	error('Graph list check: incorrect dimension for nodes field '''+ttgnodes(k)+'''')
      end
      //cross dimension validation
      if or(GNodeFieldSizes(k-1)==-1) then
	if n<>snk(GNodeFieldSizes(k-1)==-1) then
	  error('Graph list check: incorrect dimension for graphic nodes field '''+ttgnodes(k)+'''')
	end
      end
    end
  end
  
  // Checking nodes graphic defaults subfields
  //------------------------------------------
  if or(getfield(1,nodes.graphics.defaults)<>ttdefnodes) then
      error('Graph list check: invalid node graphic defaults fields')
  end
  for k=2:size(ttdefnodes,2)
    nk=nodes.graphics.defaults(ttdefnodes(k))
    if typeof(nk)<>DefNodeFieldTypes(k-1) then
      error('Graph list check: incorrect type for node graphic defaults field '''+ttdefnodes(k)+'''')
    end
    snk=size(nk)
    if prod(snk)<>0 then
      if or(DefNodeFieldSizes(k-1)>0&size(nk)<>DefNodeFieldSizes(k-1)) then
	error('Graph list check: incorrect dimension for node graphic defaults field '''+ttdefnodes(k)+'''')
      end
    end
  end
endfunction

function check_edges(edges,checkgraphics)
  // - main edges data structure
  ttedges=           ['edges','tail','head','graphics','data'];
  EdgeFieldTypes=    ['constant','constant','egraphic','edgedata']
  EdgeFieldSizes=    list([1 -1],[1 -1], -1,-1)
  // - graphic edges data structure
  ttgedges=          ['egraphic','display','defaults','profiles','name','width','foreground','font','profile_index','displaymode']
  GEdgeFieldTypes=   ['string','edgedefs','list','string','constant', 'constant', 'constant', 'constant','string']
  GEdgeFieldSizes=   list([1 1],5,-2,[1 -1],[1 -1],[1 -1],[3 -1],[1 -1],[1 1])
  // - default edge graphic properties data structure
  ttdefedges=        ['edgedefs','width','foreground','font','profile_index'];
  DefEdgeFieldTypes= ['constant', 'constant', 'constant', 'constant']
  DefEdgeFieldSizes=  list([1 1], [1 1], [3 1], [1 1])

  //Checking main edges data structure fields
  //-----------------------------------------
  if or(getfield(1,edges)<>ttedges) then
    error('Graph list check: invalid edge fields')
  end
  
  n=-1
  for k=2:size(ttedges,2)
    ek=edges(ttedges(k))
    if typeof(ek)<>EdgeFieldTypes(k-1) then
      error('Graph list check: incorrect type for edges field '''+ttedges(k)+'''')
    end
    sek=size(ek)
    if prod(sek)<>0 then
      if or(EdgeFieldSizes(k-1)>0&size(ek)<>EdgeFieldSizes(k-1)) then
	error('Graph list check: incorrect dimension for edges field '''+ttedges(k)+'''')
      end
      //cross dimension validation
      if or(EdgeFieldSizes(k-1)==-1) then
	if n==-1 then n= sek(EdgeFieldSizes(k-1)==-1),end
	if n<>sek(EdgeFieldSizes(k-1)<0) then
	  error('Graph list check: incorrect dimension for edges field '''+ttedges(k)+'''')
	end
      end
    end
  end
  if ~checkgraphics then return,end
  
  //Checking  edges graphic data structure fields
  //---------------------------------------------
  if or(getfield(1,edges.graphics)<>ttgedges) then
    error('Graph list check: invalid edges graphic fields')
  end
  for k=2:size(ttgedges,2)
    ek=edges.graphics(ttgedges(k))
    if typeof(ek)<>GEdgeFieldTypes(k-1) then
      error('Graph list check: incorrect type for edges graphic field '''+ttgedges(k)+'''')
    end
    sek=size(ek)
    if prod(sek)<>0 then
      if or(GEdgeFieldSizes(k-1)>0&size(ek)<>GEdgeFieldSizes(k-1)) then
	error('Graph list check: incorrect dimension for edges graphic field '''+ttgedges(k)+'''')
      end
      //cross dimension validation
      if or(GEdgeFieldSizes(k-1)==-1) then
	if n<>sek(GEdgeFieldSizes(k-1)<0) then
	  error('Graph list check: incorrect dimension for edges graphic field '''+ttgedges(k)+'''')
	end
      end
    end
  end

  
  // Checking edges graphic defaults subfield
  //-----------------------------------------
  if or(getfield(1,edges.graphics.defaults)<>ttdefedges) then
      error('Graph list check: invalid edge defaults fields')
  end
  for k=2:size(ttdefedges,2)
    ek=edges.graphics.defaults(ttdefedges(k))
    if typeof(ek)<>DefEdgeFieldTypes(k-1) then
      error('Graph list check: incorrect type for edge defaults field '''+ttdefedges(k)+'''')
    end
    sek=size(ek)
    if prod(sek)<>0 then
      if or(DefEdgeFieldSizes(k-1)>0&size(ek)<>DefEdgeFieldSizes(k-1)) then
	error('Graph list check: incorrect dimension for edge defaults field '''+ttdefedges(k)+'''')
      end
    end
  end
endfunction
