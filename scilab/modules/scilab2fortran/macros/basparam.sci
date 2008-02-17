// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) ????-2008 - INRIA
//
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt


function []=basparam()
//utility
//!
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
