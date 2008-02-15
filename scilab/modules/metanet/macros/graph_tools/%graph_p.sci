
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2002-2008 - INRIA - Serge STEER <serge.steer@inria.fr>
//
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt

function %graph_p(g)
//Overload graph display
  yn=['no','yes']
  txt=['graph name      : '+g.name
       'version         : '+g.version
       'oriented        : '+yn(g.directed+1)
       'number of nodes : '+string(node_number(g))
       'number of edges : '+string(edge_number(g))]

  mprintf("  %s\n",txt)
endfunction
