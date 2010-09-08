// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) ???? - INRIA - Scilab
// 
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at    
// http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt

function [sci_equiv]=get_unknown(varname,lhslist)
// Handle cases where varname appear in an expression while it is not known.

// nam may be:
// - a variable created in an other clause part
// - a M-file called without args
// - a variable created by an eval

// Check clause !!!

// Check if it is a Matlab function not converted yet
if or(varname==not_yet_converted()) then
  set_infos(msprintf(gettext("Matlab function %s not yet converted."),varname),2)
  tmpvar=Variable(varname,Infer())
  sci_equiv=Funcall("mtlb",1,Rhs_tlist(tmpvar),lhslist)
else
  // Other cases: I am not able to determine what is nam
  set_infos(msprintf(gettext("mtlb(%s) can be replaced by %s() or %s whether %s is an M-file or not."),varname,varname,varname,varname),1)
  tmpvar=Variable(varname,Infer())
  sci_equiv=Funcall("mtlb",1,Rhs_tlist(tmpvar),lhslist)
end
endfunction
