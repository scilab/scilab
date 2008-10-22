
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2002-2008 - INRIA
//
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt

function plot_graph(g,rep,rep1)
//This function plot a graph with multiple arcs in the Scilab graphic window
//with the orientation of arcs
//It uses the data of the list defining the graph
  [lhs,rhs]=argn(0)
  // g
  check_graph(g)
  if (g.nodes.graphics.x==[]|g.nodes.graphics.y==[]) then 
    error('Coordinates of nodes needed for plotting');
    return;
  end
  if rhs==1 then
    // use menu to choose parameters
    l1=list('Frame definition',1,['Automatic','Queried']);
    l2=list('Plotting arrows',2,['yes','no']);
    l3=list('Plotting sink and source nodes',1,['yes','no']);
    l4=list('Plotting node names',1,['yes','no']);
    l5=list('Plotting node labels',2,['yes','no']);
    l6=list('Plotting arc names ',2,['yes','no']);
    l7=list('Plotting arc labels',2,['yes','no']);
    l8=list('Plotting node demand',2,['yes','no']);
    l9=list('Plotting edge length',2,['yes','no']);
    l10=list('Plotting edge cost',2,['yes','no']);
    l11=list('Plotting edge min cap',2,['yes','no']);
    l12=list('Plotting edge max cap',2,['yes','no']);
    l13=list('Plotting edge weight',2,['yes','no']);
    rep=x_choices('PARAMETERS OF THE PLOT',list(l1,l2,l3,l4,l5,l6,l7,l8,l9,l10,l11,l12,l13));
    if (rep==[]) then, return;end;
    if (rep(1)==2) then,
      rep1=x_mdialog(['Frame definition'],['x-origin of the plot';
		    'y-origine of the plot';
		    'Width of the plot';
		    'Height of the plot'],...
		     ['0','0','600','400']);
    end;
    rect=ge_dig_bound(g)
  elseif rhs==2 then 
    if or(size(rep)<>[1 13]) then
      error('Second argument must be a row vector with size 13')
    end
    rect=ge_dig_bound(g)
    
  else
    if or(size(rep1)<>[1 4]) then
      error('Third argument must be a row vector with size 4')
    end
    rect=rep1
  end

  a=gca()
  a.isoview='on';
  a.data_bounds=[rect(1:2);rect(3:4)];
  
  g.directed=rep(2)==1
  NodeId=[];ArcId=[];
  if rep(4)==1 then NodeId=2,end
  if rep(5)==1 then NodeId=4,end
  if rep(8)==1 then NodeId=3,end
  
  if rep(6)==1 then ArcId=2,end
  if rep(7)==1 then ArcId=10,end
  if rep(9)==1 then ArcId=6,end
  if rep(10)==1 then ArcId=3,end
  if rep(11)==1 then ArcId=4,end
  if rep(12)==1 then ArcId=5,end
  if rep(13)==1 then ArcId=9,end
  
  
  EGdata=tlist(['egdata1','ArcId','NodeId'],ArcId,NodeId)
  ge_draw_graph(g)

endfunction
