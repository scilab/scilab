// Copyright INRIA
mode(-1)
//
// (setbpt delbpt).
deff('[]=lig()',[
'shlig(2),';
'if 1>2 then,';
'  shlig(4),';
',';
'  shlig(6)     ,'; // break-point
',';
'end,shlig(8)   ,'; // break-point
'shlig(9),';
'for k=1:3,';
'  k;shlig(11)  ,'; // break-point 
'end,shlig(12)  ,'; // break-point
',';
'for l=1:10,';
'  if l==5 then break,end,';
'  l,';
'end,shlig(17)  ,'; // break-point
'shlig(18),';
'select 2,';
'case 1 then,';
',';
'case 2 then,';
'  shlig(23)   ,'; // break-point
'case 3 then,';
'1,';
'else,';
'2,';
'end,shlig(28) ,'; // break-point
'shlig(29),';
'select 0,';
'case 1 then,';
'1,';
'case 2 then,';
'2,';
'case 3 then,';
'3,';
'else,';
'  shlig(38)  ,'; // break-point
'end,shlig(39),'; // break-point
'shlig(40)    ,'; // break-point
'i=0,';
'while i<3,';
'i=i+1,shlig(43) ,'; // break-point 
'end,shlig(44) ,'; //break-point
'i=0;,';
'while i<10,';
'i=i+1,if i==2 then break,end,shlig(47),';
'end,shlig(48) ,' //break-point
],'n')
deff('[]=shlig(n)','write(%io(2),''ligne ''+string(n))')

//definition des points d'arrets
setbpt('lig',6)
setbpt('lig',8)
setbpt('lig',11)
setbpt('lig',12)
setbpt('lig',17)
setbpt('lig',23)
setbpt('lig',28)
setbpt('lig',38)
setbpt('lig',39)
setbpt('lig',40)
setbpt('lig',43)
setbpt('lig',44)
setbpt('lig',48)
write(%io(2),[
'user should check the line number'])
write(%io(2),[' ';'MACRO NOT COMPILED';'-----------------';' '])
lig()
resume
resume
resume
resume
resume
resume
resume
resume
resume
resume
resume
resume
resume
resume
resume
resume

comp(lig)
write(%io(2),[' ';'COMPILED MACRO';'-----------------';' '])
lig()
resume
resume
resume
resume
resume
resume
resume
resume
resume
resume
resume
resume
resume
resume
resume
resume
delbpt('lig')
clear lig shlig

