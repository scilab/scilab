// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) ???? - INRIA - Scilab
// 
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at    
// http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt

function r=mtlb_isa(OBJ,class)
select class
case "logical"
  r=type(OBJ)==4
case "char"
  r=type(OBJ)==10
case "numeric"
  r=or(type(OBJ)==[1,5,8])
case "int8"
  r=typeof(OBJ)=="int8"
case "uint8"
  r=typeof(OBJ)=="uint8"
case "int16"
  r=typeof(OBJ)=="int16"
case "uint16"
  r=typeof(OBJ)=="uint16"
case "int32"
  r=typeof(OBJ)=="int32"
case "uint32"
  r=typeof(OBJ)=="uint32"
case "single"
  r=type(OBJ)==1
case "double"
  r=type(OBJ)==1
case "cell"
  r=typeof(OBJ)=="ce" 
case "struct"
  r=typeof(OBJ)=="st" 
case "function_handle"
 r=type(OBJ)==13
case "sparse"
 r=type(OBJ)==5
case "lti"
   r=typeof(OBJ)=="state-space"
else
  error(msprintf(gettext("%s: Unhandled class ''%s''.\n"),"mtlb_isa",class));
end
endfunction
