function [Color,Line,LineStyle,Marker,MarkerStyle,MarkerSize,fail]=getLineSpec(ch) 

Line = %T;
LineStyle=1;
Color=0;
MarkerStyle=[];
MarkerSize=1;
LineStyle=1;
fail=0;
ms=1;

//s=loadGlobalGraphicState();
//s=s.simpleColorTable;

//conversion to lower format
ch = convstr(ch);

// Line type modifiers

if strindex(ch,'--')
   ch=strsubst(ch,'--','');
   LineStyle=2;
elseif strindex(ch,'-.')
   ch=strsubst(ch,'-.','');
   LineStyle=4;
elseif strindex(ch,':')
   ch=strsubst(ch,':','');
   LineStyle=5;
elseif strindex(ch,'-')
   ch=strsubst(ch,'-','');
   LineStyle=1;
end   

//

disp("ch vaut:::::::: 1/2")
disp(ch)

//color management
//difficulty here since we have to allow every kind of writings:
//i.e:
//magenta can be set by: 'm' 'ma' 'mag' 'mage' 'magen' 'magent' or at least 'magenta'
//


YELLOW  = ['y' 'ye' 'yel' 'yell' 'yello' 'yellow'];
MAGENTA = ['m' 'ma' 'mag' 'mage' 'magen' 'magent' 'magenta'];
CYAN    = ['c' 'cy' 'cya' 'cyan'];
BLUE    = ['b' 'bl' 'blu' 'blue'];
GREEN   = ['g' 'gr' 'gre' 'gree' 'green'];
RED     = ['r' 're' 'red'];

BLA   =   ['k'      'bla' 'blac' 'black'];
WHI   =   ['w' 'wh' 'whi' 'whit' 'white'];

COULEURS = [YELLOW MAGENTA CYAN BLUE GREEN RED BLA WHI];

flag = %F;

for j=length(ch):-1:1

 if flag == %T
  break;
 end

 res = find(length(COULEURS)==j)

 for i=1:size(res,'*')
  if strindex(ch,COULEURS(res(i)))<>[] then
   if (res(i) >= 1) & (res(i) <= 6) then
    Color=color('yellow')
    ch=strsubst(ch,COULEURS(res(i)),'');
    flag = %T
    break;
   else if (res(i) >= 7) & (res(i) <= 13) then
     Color=color('magenta')
     ch=strsubst(ch,COULEURS(res(i)),'');
     flag = %T
     break;
   else if (res(i) >= 14) & (res(i) <= 17) then
     Color=color('cyan')
     ch=strsubst(ch,COULEURS(res(i)),'');
     flag = %T
     break;
   else if (res(i) >= 18) & (res(i) <= 21) then
     Color=color('blue')
     ch=strsubst(ch,COULEURS(res(i)),'');
     flag = %T
     break;
   else if (res(i) >= 22) & (res(i) <= 26) then
     Color=color('green')
     ch=strsubst(ch,COULEURS(res(i)),'');
     flag = %T
     break;
   else if (res(i) >= 27) & (res(i) <= 29) then
     Color=color('red')
     ch=strsubst(ch,COULEURS(res(i)),'');
     flag = %T
     break;
   else if (res(i) >= 30) & (res(i) <= 33) then
     Color=color('black')
     ch=strsubst(ch,COULEURS(res(i)),'');
     flag = %T
     break;
   else if (res(i) >= 34) & (res(i) <= 38) then
     Color=color('white')
     ch=strsubst(ch,COULEURS(res(i)),'');
     flag = %T
     break;
   end, end, end, end, end, end, end, end
  end
 end

 
end


disp("ch vaut:::::::: 2/2")
disp(ch)

// Special shapes

SQUARE    = ['s' 'sq' 'squ' 'squa' 'squar' 'square'];
DIAMOND   = ['d' 'di' 'dia' 'diam' 'diamo' 'diamon' 'diamond'];
PENTAGRAM = ['p' 'pe' 'pen' 'pent' 'penta' 'pentag' 'pentag' 'pentagr' 'pentagra' 'pentagram'];

SPESH = [SQUARE DIAMOND PENTAGRAM]

for j=length(ch):-1:1
 res = find(length(SPESH)==j)

 for i=1:size(res,'*')
  if strindex(ch,SPESH(res(i)))<>[]
   if (res(i) >= 1) & (res(i) <= 6)
    MarkerStyle=11;
    MarkerSize=3;
    ch=strsubst(ch,SPESH(res(i)),'');
    break;
   else if (res(i) >= 7) & (res(i) <= 13)
    MarkerStyle=5;
    MarkerSize=4;
    ch=strsubst(ch,SPESH(res(i)),'');
    break;
   else if (res(i) >= 14) & (res(i) <= 23)
    MarkerStyle=14;
    MarkerSize=6;
    ch=strsubst(ch,SPESH(res(i)),'');
    break;
   end, end, end
  end
 end

end


if(length(ch)>1) then
 disp("Error in LineSpec property : bad argument specified");
 return;
end

for i=1:length(ch)

select part(ch,i)
  case '.'
     MarkerStyle=0;
     MarkerSize=1;
  case '+'
     MarkerStyle=1;
     MarkerSize=1;
  case 'x'
     MarkerStyle=2;
     MarkerSize=1;
  case '*'
     MarkerStyle=10;
     MarkerSize=7;
  case '^'
     MarkerStyle=6;
     MarkerSize=1;
  case 'v'
     MarkerStyle=7;
     MarkerSize=1;
  case 't'
     MarkerStyle=8;
     MarkerSize=1;
  case 'f'
    MarkerStyle=4;
    MarkerSize=1;
  case 'o'
     MarkerStyle=9;
     MarkerSize=3;
  else
     fail=1;
end // select
end // for


if MarkerStyle == []
   Marker = %F;
else
   Marker = %T;
end

endfunction
// end of getLineSpec