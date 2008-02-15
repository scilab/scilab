
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2002-2008 - INRIA
//
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt

function g1=line_graph(g)
  [lhs,rhs]=argn(0)
  if rhs<>1 then error(39), end
  tta=g.edges.tail;hhe=g.edges.head;
  ta=[tta hhe];he=[hhe tta];
  ma=prod(size(g.edges.tail));
  ta1=[];he1=[];
  for i=1:(2*ma),
    ist=ta(i);iar=he(i);
    ij=find(ta==ist);ij=ij-int((ij-0.1)/ma)*ma;ij=ij(find(ij>i));
    jk=find(he==iar);jk=jk-int((jk-0.1)/ma)*ma;jk=jk(find(jk>i));
    ke=[ij jk];ka=i*ones(ke);
    ta1=[ta1 ka];he1=[he1 ke];
  end;
  g1=make_graph('foo',0,ma,ta1,he1);
  xi=g.nodes.graphics.x;yi=g.nodes.graphics.y;
  if (xi<>[]) then
    x1i=0.5*(xi(tta)+xi(hhe));
    g1.nodes.graphics.x=x1i;
  end
  if (yi<>[]) then
    y1i=0.5*(yi(tta)+yi(hhe));
    g1.nodes.graphics.y=y1i;
  end
endfunction
