// -----------------------------------------------------------
// Allan CORNET
// INRIA 2005
// -----------------------------------------------------------
function bOK=IsAScalar(VarInput)
   bOK=%F;
   if ( (length(VarInput)==1) & (type(VarInput)==1) ) then
     bOK=%T
   end;
 endfunction
// -----------------------------------------------------------
 