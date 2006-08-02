function [tree]=sci_MFUN(tree)
// Prototype for functions nargin and nargout
// Copyright INRIA
// M2SCI function
// Conversion function for Matlab MFUN()
// Input: tree = Matlab funcall tree
// Ouput: tree = Scilab equivalent for tree
// V.C.

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
  set_infos(rhs2code(tree.rhs)+" considered to be a Scilab macro.",2);
  evstrtree=Funcall("evstr",1,tree.rhs,list())
  macrovartree=Funcall("macrovar",1,Rhs(evstrtree),list())
  getfieldtree=Funcall("getfield",1,Rhs(fieldnb,macrovartree),list())
  tree=Funcall("size",1,Rhs(getfieldtree,"*"),tree.lhs)
  tree.lhs(1).dims=list(1,1)
  tree.lhs(1).type=Type(Double,Real)
end
endfunction
