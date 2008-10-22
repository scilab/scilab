
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2002-2008 - INRIA
//
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt

function g=gen_net(name,directed,v)
  [lhs,rhs]=argn(0)
  g=[]
  if rhs==0 then
    directed=x_choices('Is the graph directed?',list(list(' ',1,['Yes','No'])));
    if directed==[] then return, end
    if directed==2 then directed=0, end
    name=x_dialog('Name of the graph','foo');
    if name==[] then return, end
    txt=['Seed for random';'Number of nodes';'Number of sources';..
	 'Number of sinks';'Minimum cost';'Maximum cost';'Input supply';..
	 'Output supply';'Minimum capacity';'Maximum capacity';..
	 'Percentage of edges with costs';'Percentage of edges with capacities'];
    v=x_mdialog('Values for generating the network',txt,..
		['1';'4';'1';'1';'0';'10';'100';'100';'0';'100';'50';'50']);
    if v==[] then return, end
    v=evstr(v);
  else
    // name
    if type(name)<>10|prod(size(name))<>1 then
      error('""name"" must be a string')
    end  
    // directed
    if directed<>1&directed<>0 then
      error('""directed"" must be 0 or 1')
    end
    // v
    s=size(v)
    if s(1)<>1 then
      error('""v"" must be a row vector')
    end
    if s(2)<>12 then
      error('""v"" must have 12 elements')
    end 
  end

  inseed=v(1)  //seed for random
  n=v(2) //number of nodes
  nsorc=v(3) //number of sources
  nsink=v(4) //number of sinks
  mincst=v(5) //min cost
  maxcst=v(6) //max cost
  itsup=v(7) //input supply
  otsup=v(8) //output supply
  mincap=v(9) //min capacity
  maxcap=v(10) //max capacity
  bhicst=v(11) //percentage of edges with costs
  bcap=v(12) //percentage of nodes with capacity

  ns=n+nsorc+nsink;ns2=ns*2;ns4=ns*4;ns10=ns*10;ns12=ns*12;
  ns20=ns*20;ns30=ns*30;

  [ntype,ma,tail,head,x,y,cap,cost]=m6meshmesh(inseed,..
					       n,nsorc,nsink,mincst,maxcst,itsup,otsup,..
					       bhicst,bcap,mincap,maxcap,..
					       ns,ns2,ns4,ns10,ns12,ns20, ...
					       ns30)
  g=make_graph(name,directed,n,tail(1:ma),head(1:ma))
  g.nodes.graphics.type=ntype;
  g.nodes.graphics.x=x;
  g.nodes.graphics.y=y;
  g.nodes.graphics.name=string(1:n);
  g.edges.data=mlist(['edgedata','cost','capacity'],cost(1:ma),cap(1:ma));
endfunction
