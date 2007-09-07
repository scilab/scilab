function editvar_set_values( var, winId )

// Copyright INRIA 2007
// Author Jean-Baptiste Silvy
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
