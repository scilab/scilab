function [c,m,ms,lt,fail]=getColorNumber(ch) 

c=0;
m=[];
forceLine=0;
fail=0;
ms=1;

s=loadGlobalGraphicState();
s=s.simpleColorTable;

// Line type modifiers

if strindex(ch,'--')
   ch=strsubst(ch,'--','');
   forceLine=2;
elseif strindex(ch,'-.')
   ch=strsubst(ch,'-.','');
   forceLine=4;
elseif strindex(ch,':')
   ch=strsubst(ch,':','');
   m=0;
elseif strindex(ch,'-')
   ch=strsubst(ch,'-','');
   forceLine=1;
end   

//

for i=1:length(ch)

select part(ch,i)
  case 'y'
     c=s.y;
  case 'm'
     c=s.m;
  case 'c'
     c=s.c;
  case 'b'
     c=s.b;
  case 'g'
     c=s.g;
  case 'r'
     c=s.r;
  case 'w'
     c=s.w;
  case 'k'
     c=s.k;
  case '.'
     m=0;
  case '+'
     m=1;
  case 'x'
     m=2;
  case '*'
     m=3;
  case '^'
     m=6;
  case 'v'
     m=7;
  case 't'
     m=8;
  case 'd'
    m=5;
  case 'f'
    m=4;
  case 'o'
     m=9;
  case '1'
     ms=1;
  case '2'
     ms=2;
  case '3'
     ms=3;
  case '4'
     ms=4;
  case '5'
     ms=5;
  case '6'
     ms=6;
  case '7'
     ms=7;
  case '8'
     ms=8;
  case '9'
     ms=9;
  else
     fail=1;
end // select
end // for

if forceLine == 0
   if m == []
      lt = lineStyle;
   else
      lt = 0;
   end
else
   lt = forceLine;   
end

// end of getColorNumber
