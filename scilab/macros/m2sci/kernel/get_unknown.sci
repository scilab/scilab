function [sci_equiv]=get_unknown(varname,lhslist)
// Copyright INRIA
// Handle cases where varname appear in an expression while it is not known.

// nam may be:
// - a variable created in an other clause part
// - a M-file called without args
// - a variable created by an eval

// Check clause !!!

// Other cases: I am not able to determine what is nam
set_infos("mtlb("+varname+") can be replaced by "+varname+"() or "+varname+" whether "+varname+" is an M-file or not",1)
tmpvar=Variable(varname,Infer())
sci_equiv=Funcall("mtlb",1,Rhs(tmpvar),lhslist)
endfunction
