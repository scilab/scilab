
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2008 - INRIA - Serge STEER <serge.steer@inria.fr>
//
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt

function [ok,GraphList]=ge_do_add_node_data(GraphList)
  desc=['Add a  new node data field';
	'  enter the field name'
	'  and the default value for this field']
  def=['','']

  data=GraphList.nodes.data
  F=getfield(1,data)
  ok=%f
  while ~ok 
    res=x_mdialog(desc,['Name','Default Value'],def)
    if res==[] then return,end
    ok=%t
    name=stripblanks(res(1))
    if execstr(name+'=1','errcatch')<>0 then
      x_message('Invalid Field name')
      ok=%f,
    end
    if ok&or(F(2:$)==name) then
      x_message('This field name is already defined')
      ok=%f,
    end
    if ok then
      ierr=execstr('default='+res(2),'errcatch')
      if ierr<>0 then 
	x_message('Answer for ""Default Value"" cannot be evaluated'),
	ok=%f
      end
    end
  end
  n=size(GraphList.nodes)
  F(1,$+1)=name
  setfield(1,F,data)
  if size(default,2)>1 then
    d=list()
    for k=1:n,d(k)=default,end
    data(name)=d;
  else
    if n>0 then
      data(name)=default(:,ones(1,n)),
    else
      data(name)=[]
    end
  end
  GraphList.nodes.data=data;
endfunction
