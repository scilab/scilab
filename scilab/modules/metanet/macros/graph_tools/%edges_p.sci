
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2002-2008 - INRIA
//
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt

function %edges_p(edges)
//Overloads edge  data structure display
//Author: Serge Steer; Copyright INRIA
 
  w='';
  if edges.graphics.name==[] then 
     na=size(edges.tail,2)
    edges.graphics.name=string(1:na),
  end
   
  w=format_struct(edges.graphics,'name','names')+..
    format_struct(edges,['tail','head'])+..
    format_struct(edges.data)+'|'

  mprintf("  %s\n",w)
endfunction

