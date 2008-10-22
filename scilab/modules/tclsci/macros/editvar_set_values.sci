// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2007 - INRIA - Jean-Baptiste SILVY
// 
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at    
// http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt


function editvar_set_values( var, winId )
// For editvar, send the values of the variable var (size nbRow x nbCol) to TCL
// in order to display them in window winId

//
// This is a slow manner to move the data ...
// Anyone have a faster method than TCL_SetVar
// I would like set some tcl variable like varname(pos_i,pos_j)
//

var = string(var) ;

[nbRow,nbCol] = size(var) ;

for j = 1:nbCol,
  for i = 1: nbRow,
    varname="sciGUITable(win,"+winId+",data,"+string(i)+","+string(j)+")";
    TCL_SetVar(varname,var(i,j));
  end
end

endfunction
