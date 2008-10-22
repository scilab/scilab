
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2002-2008 - INRIA
//
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt

function g=make_graph(name,directed,n,tail,head)
  [lhs,rhs]=argn(0)
  if rhs<>5 then error(39), end
  // name
  if type(name)<>10|prod(size(name))<>1 then
    error('""name"" must be a string')
  end
  // directed
  if directed<>1&directed<>0 then
    error('""directed"" must be 0 or 1')
  end
  // node_number
  if prod(size(n))<>1|n<0
    error('""n"" must be a positive integer')
  end
  if size(tail,'*')<>size(head,'*') then
    error('""tail"" and ""head"" must have identical sizes')
  end
 
  // check tail
  ma=size(tail,'*')
  tail=matrix(tail,1,-1)
  head=matrix(head,1,-1)
  
  //CHECK TAIL AND HEAD
   if min(min(tail),min(head))<1| (n>0&max(max(tail),max(head))>n) then
      error('""tail"" and ""head"" do not represent a graph')
    end

    // tail and head
    if min(min(tail),min(head))<1| (n>0&max(max(tail),max(head))>n) then
      error('""tail"" and ""head"" do not represent a graph')
    end
  if n<>0  then
    nodename=string(1:n)
  else
    ij=[tail' head'];
    tri=[ij(:,1)' ij(:,2)'];[atri,indic]=sort(-tri);
    atri=-atri;
    n1=size(atri,2);
    atri1=atri-[0 atri(1:n1-1)];
    ij1=find(atri1<>0);
    ij2=find(atri1==0);
    uu=1:n1;
    nij2=size(ij2,2);vv2=1:nij2;uu(ij2)=uu(ij2)-vv2;
    nij1=size(ij1,2);vv1=[1:nij1];uu(ij1)=vv1;
    w=0*ones(1:n1);w(indic)=uu;
    tail=w(1:(ma));head=w((ma)+1:n1);
    n=nij1
    nodename=string(atri(ij1))
  end
  g=ge_new_graph();
  g.nodes.number=n
  g.directed=directed;
  g.edges.tail=matrix(tail,1,-1);
  g.edges.head=matrix(head,1,-1);
  //set default values
  g.nodes.graphics.name=matrix(nodename,1,-1);
  g.nodes.graphics.x=zeros(1,n);
  g.nodes.graphics.y=zeros(1,n);
  g.nodes.graphics.type=zeros(1,n);
  g.nodes.graphics.diam=zeros(1,n);
  g.nodes.graphics.border=zeros(1,n);
  g.nodes.graphics.colors=zeros(2,n);
  g.nodes.graphics.font=zeros(3,n);
  
  g.edges.graphics.name=string(1:ma);
  g.edges.graphics.width=zeros(1,ma);
  g.edges.graphics.foreground=zeros(1,ma);
  g.edges.graphics.font=zeros(3,ma);
  g.edges.graphics.profile_index=zeros(1,ma);
endfunction
