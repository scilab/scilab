
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2002-2008 - INRIA - Serge STEER <serge.steer@inria.fr>
//
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt

function [ok,GraphList,edited,fname]=ge_do_load(fname)
  GraphList=[];ok=%t
  if edited then
    ok=messagebox(['Current graph is modified';'Do you really want to erase it'],"modal","question",['yes','no']);
    if ok==2 then ok=%f,return,end
  end

  if argn(2)<1 then fname=uigetfile("*.graph*"),end
  if fname==emptystr() then ok=%f,return,end// Canceled by user
  [GraphList,edited,msg]=load_graph(fname)
  if msg<>[] then //an error occured
    messagebox(msg,"modal")
    ok=%f
    return
  end
  [path,name,ext]=splitfilepath(fname)
  GraphList.name=name;
  ok=%t

endfunction

