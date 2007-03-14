function [sci_equiv]=get_unknown(varname,lhslist)
// Copyright INRIA
// Handle cases where varname appear in an expression while it is not known.

// nam may be:
// - a variable created in an other clause part
// - a M-file called without args
// - a variable created by an eval

// Check clause !!!

// Check if it is a Matlab function not converted yet
if or(varname==not_yet_converted()) then
  set_infos(msprintf(gettext("messages","m2sci_message_41"),varname),2)
  tmpvar=Variable(varname,Infer())
  sci_equiv=Funcall("mtlb",1,Rhs(tmpvar),lhslist)
else
  // Other cases: I am not able to determine what is nam
  set_infos(msprintf(gettext("messages","m2sci_message_42"),varname,varname,varname,varname),1)
  tmpvar=Variable(varname,Infer())
  sci_equiv=Funcall("mtlb",1,Rhs(tmpvar),lhslist)
end
endfunction
