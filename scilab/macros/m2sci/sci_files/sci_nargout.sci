function [tree]=sci_nargout(tree)
// File generated from sci_PROTO15.g: PLEASE DO NOT EDIT !
// Copyright INRIA
// M2SCI function
// Conversion function for Matlab nargout()
// Input: tree = Matlab funcall tree
// Ouput: tree = Scilab equivalent for tree
// V.C.

global("varslist")

if rhs==0 then // Number of arguments of local function
  tree=Variable("%nargout",Infer(list(1,1),Type(Double,Real)))
  varslist($+1)=M2scivar("nargout","%nargout",Infer(list(1,1),Type(Double,Real)))
else // Number of arguments of M-file
  if "nargout"=="nargin" then
    fieldnb=1
  else
    fieldnb=2
  end
  set_infos(rhs2code(tree.rhs)+" considered to be a Scilab macro.",2);
  evstrtree=Funcall("evstr",1,tree.rhs,list())
  macrovartree=Funcall("macrovar",1,Rhs(evstrtree),list())
  getfieldtree=Funcall("getfield",1,Rhs(fieldnb,macrovartree),list())
  tree=Funcall("size",1,Rhs(getfieldtree,"*"),tree.lhs)
  tree.lhs(1).dims=list(1,1)
  tree.lhs(1).type=Type(Double,Real)
end
endfunction
