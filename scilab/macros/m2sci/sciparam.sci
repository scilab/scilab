function sciparam()
// Copyright INRIA
%else='else',%end='end';%elseif='elseif';%if='if';%for='for';
%select='select';%while='while';%case='case';%then='then'
quote=''''
logics=['==','<','>','<=','>=','<>']
//symbole : symbole associe a un numero d'operation
//type_du resultat : 0:facteur,1:terme,2:expression
//parenthesage : indique s'il faut parentheser les operandes de meme type
//               que type_du_resultat
//
//       symbole type_du_resultat parenthesage_des_operandes
ops     =['+',   'a';
         '-',    's';
         '*',    'm';
         '.*',   'x';
         '*.',   'u';
         '.*.',  'k';
         '/',    'r';
         './',   'd';
         '/.',   'v';
         './.',  'y';
         '\',    'l';
         '.\',   'q';
         '\.',   'w';
         '.\.',  'z';
         '**',   'p';
         '==', 'log';
         '<', 'log';
         '>', 'log';
         '<=', 'log';
         '>=', 'log';
         '~=', 'log';
         ':',    'imp';
         '[]',   'c';
         'ins',  'i';
         'ext',  'e';
         quote,  't';
         '[]',   'c';
         '|',    'g';
         '&',    'h';
         '~',    '5';
         '.^',   'j';
         '.'+quote '0';
         '[]',   'c']         
     
same=['error','input','clc','home','fprintf','sprintf','emptystr']

[logics,same,ops,%else,%end,%elseif,%if,%for,%select,%while,%case,%then]=...
resume(logics,same,ops,%else,%end,%elseif,%if,%for,%select,%while,%case,%then)
 



