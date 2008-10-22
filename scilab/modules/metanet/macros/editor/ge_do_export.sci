
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2002-2008 - INRIA - Serge STEER <serge.steer@inria.fr>
//
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt

function ge_do_export(GraphList)

// export the contents of the editor to a regular graphic window for
// plotting and exports
//!
  ge_disablemenus()
  ge_axes_handle=gca();
  gindex=ge_axes_handle.user_data;
  hedges=gindex.edge;hnodes=gindex.node;

  old=gcf()
  scf(max(winsid()+1))
  
  nn=size(hnodes,'*');
  if nn>0 then
    D=zeros(nn,4);
    for k=1:size(hnodes,'*')
      e=hnodes(k).children(2)
      if get(e,'type')=='Compound' then
	d=e.children(2).data(1:4);//circle
	d1=e.children(1).data;//polyline
	mn=min(d1,'r');
	mx=max(d1,'r');
	D(k,:)=[min(d(1),mn(1)), max(d(1)+d(3),mx(1)), min(d(2)-d(4),mn(2)), max(d(2),mx(2))];
      else
	d=e.data(1:4);
	D(k,:)=[d(1) d(1)+d(3) d(2)-d(4) d(2)]
      end
    end
    rect=[min(D(:,[1 3]),'r');max(D(:,[2 4]),'r')]
    // sink nodes
  else
    rect=[0,0;600,400];
  end

  drawlater()
  ax=gca();ax.data_bounds=rect
  ax.margins=[0,0,0,0];ax.isoview='on';
  objs=ge_axes_handle.children
  newax=gca()
  //the following code could be rewritten
  //copy(ge_axes_handle.children, newax)
  //when Scilab will be able to clone arrays and compounds
  for k=1:size(objs,'*')
    ok=objs($+1-k).children
    m=size(ok,'*')
    for j=1:m
      ok(j)=copy(ok(j), newax)
    end
    objs($+1-k)=glue(ok)
  end
  drawnow()
  scf(old)
  ge_enablemenus()
endfunction
