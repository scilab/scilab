
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2002-2008 - INRIA
//
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt

function spc=cycle_basis(g,job)
  if argn(2)<1 then error(39), end
  if argn(2)==1 then job='mat',end
  if and(job<>['mat','list']) then
    error('Second argument value must be ''mat'' or ''list''')
  end
  check_graph(g,%f)
  // finds a cycle basis in a simple connected undirected graph
  if g.directed then
    error('The graph must be undirected')
  end

  if is_connex(g)<>1 then
    error('The graph must be connected')
  end
  n=node_number(g);m=edge_number(g);
  if ( n < 3) then
    error('Not enough nodes in the graph to have a cycle')
  end
  nu=m-n+1;
  ta=g.edges.tail;he=g.edges.head;
  t=min_weight_tree(g);

  //
  ta1=ta;he1=he;
  ta1(t)=[];he1(t)=[];
  if (ta1 == []) then error('No cycle in the graph'),end

  bac=[];Dir=[];
  if job=='mat' then
    spc=sparse([],[],[nu m]);
  else
    spc=list()
  end
  spt=sparse([ta' he'],1:m,[n n]);
  spt=spt+spt';

  t=[0 t];
  for i=1:nu,
    cycle=[];
    
    i1=ta1(i);
    i2=he1(i);
    Dir=full(spt(i1,i2));
    bac=[];
    while ((i1)<>1)
      iedge=t(i1);
      bac=[iedge bac];i1=ta(iedge)+he(iedge)-i1;
    end
    
    while ((i2)<>1)
      iedge=t(i2);
      Dir=[iedge Dir];i2=ta(iedge)+he(iedge)-i2;
    end
    
    jmax=min(size(bac,2),size(Dir,2));
    itron=find(bac(1:jmax)==Dir(1:jmax))
    
    bac(itron)=[];Dir(itron)=[];
    cycle=[Dir bac($:-1:1)];

    if job=='mat' then
      spc(i,1:size(cycle,2))=cycle;
    else
      spc($+1)=cycle;
    end
  end
endfunction
