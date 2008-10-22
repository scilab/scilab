function []=basparam()
//utility
//!
// Copyright INRIA
%else='else',%end='end';%elseif='elseif';%if='if';%for='for';
%select='select';%while='while';%case='case';%then='then'
quote=''''
logics=['==','<','>','<=','>=','<>']
//
//       symbol type_result parenthes_od_operands
ops     =['+','2','  ';
         '-','2',' r';
         '*','1','  ';
         '.*','1',' r';
         '*.','1','lr';
         '.*.','1','lr';
         '/','1',' r';
         './','1',' r';
         '/.','1','lr';
         './.','1','lr';
         '\','1','l ';
         '.\','1','l ';
         '\.','1','l ';
         '.\.','1','lr';
         '**','0',' r';
         '==','2','lr';
         '<','2','lr';
         '>','2','lr';
         '<=','2','lr';
         '>=','2','lr';
         '<>','2','lr';
         ':','0','  ';
         '[]','0','  ';
         'ins','0','  ';
         'ext','0','  ';
         quote,'0','  '
         '|','2',' ';
         '&','1',' '
         '~','2',' r'];
[logics,ops,quote,%else,%end,%elseif,%if,%for,%select,%while,%case,%then]=...
resume(logics,ops,quote,%else,%end,%elseif,%if,%for,%select,%while,%case,%then)
endfunction
