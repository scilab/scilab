// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2002-2004 - INRIA - Vincent COUVERT 
// 
// Copyright (C) 2012 - 2016 - Scilab Enterprises
//
// This file is hereby licensed under the terms of the GNU GPL v2.0,
// pursuant to article 5.3.4 of the CeCILL v.2.1.
// This file was originally licensed under the terms of the CeCILL v2.1,
// and continues to be available under such terms.
// For more information, see the COPYING file which you should have received
// along with this program.

function [tree]=sci_MFUN(tree)
// Prototype for functions nargin and nargout
// M2SCI function
// Conversion function for Matlab MFUN()
// Input: tree = Matlab funcall tree
// Output: tree = Scilab equivalent for tree

global("varslist")

if rhs==0 then // Number of arguments of local function
  tree=Variable("%MFUN",Infer(list(1,1),Type(Double,Real)))
  varslist($+1)=M2scivar("MFUN","%MFUN",Infer(list(1,1),Type(Double,Real)))
else // Number of arguments of M-file
  if "MFUN"=="nargin" then
    fieldnb=1
  else
    fieldnb=2
  end
  set_infos(msprintf(gettext("%s considered to be a Scilab macro."), strcat(rhs2code(tree.rhs), "")),2);
  evstrtree=Funcall("evstr",1,tree.rhs,list())
  macrovartree=Funcall("macrovar",1,Rhs_tlist(evstrtree),list())
  getfieldtree=Funcall("getfield",1,Rhs_tlist(fieldnb,macrovartree),list())
  tree=Funcall("size",1,Rhs_tlist(getfieldtree,"*"),tree.lhs)
  tree.lhs(1).dims=list(1,1)
  tree.lhs(1).type=Type(Double,Real)
end
endfunction
