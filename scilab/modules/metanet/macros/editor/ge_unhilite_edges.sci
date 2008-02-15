
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2008 - INRIA - Serge STEER <serge.steer@inria.fr>
//
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt

function ge_unhilite_edges(p,GraphList)
//Author : Serge Steer 2008
  f=gcf(); drawing=f.immediate_drawing;f.immediate_drawing='off'
  ge_axes_handle=gca();gindex=ge_axes_handle.user_data; hedges=gindex.edge;

  for k=1:size(p,'*')
    e=hedges(p(k)); 
    e.children(3).thickness=ge_get_edge_style(p(k));
  end
  f.immediate_drawing=drawing
  show_pixmap()

endfunction
