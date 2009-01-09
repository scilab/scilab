
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2002-2008 - INRIA - Serge STEER <serge.steer@inria.fr>
//
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt

function [ok,GraphList]=ge_do_new()
  if EGdata.Edited then
    if messagebox(['Current graph is modified'
		  'Do you really want to erase it'],"modal","question",['yes','no'])==2 then
      ok=%f
      graphlist=[]
      return
    end
  end
  ok=%t
  GraphList=ge_new_graph()
endfunction

