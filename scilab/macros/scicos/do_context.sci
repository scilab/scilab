function [context,ok]=do_context(context)
// Copyright INRIA
if context==[] then context=' ',end
rep=dialog([
    'You may enter here scilab instructions to define ';
    'symbolic parameters used in block definitions using';
    'Scilab instructions; comments are not allowed.';
    ' ';
    'These instructions are evaluated once confirmed, i.e.,you';
    'click on OK, by Eval and every time diagram is loaded. '],context)
if rep==[] then 
  ok=%f
else
  context=rep
  ok=%t
end

