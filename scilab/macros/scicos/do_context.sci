function [context, ok] = do_context(context)
// Copyright INRIA

if context==[] then context=' ',end ; //** if the variable is empty put a blank space 

//** open a dialog window ; dialog() it is a wrapper of "x_dialog"
rep = dialog([
              'You may enter here scilab instructions to define '   ;
              'symbolic parameters used in block definitions using' ;
              'Scilab instructions; comments are not allowed.'      ; //** now the comments ARE ALLOWED ! 
              ' '                                                   ;
              'These instructions are evaluated once confirmed, i.e.,you';
              'click on OK, by Eval and every time diagram is loaded. '], context) ;
//** "rep" is a colum vector of strings 
if rep==[] then //** in case of no entry  
  ok = %f
else
  context = rep //** if some text is typed //** 
  ok = %t
end

endfunction
