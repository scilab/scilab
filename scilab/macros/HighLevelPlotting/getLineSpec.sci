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


//color management
//difficulty here since we have to allow evry kind of writings:
//i.e:
//magenta can be set by: 'm' 'ma' 'mag' 'mage' 'magen' 'magent' or at least 'magenta'
//


YELLOW  = ['y' 'ye' 'yel' 'yell' 'yello' 'yellow'];
MAGENTA = ['m' 'ma' 'mag' 'mage' 'magen' 'magent' 'magenta'];
CYAN    = ['c' 'cy' 'cya' 'cyan'];
BLUE    = ['b' 'bl' 'blu' 'blue'];
GREEN   = ['g' 'gr' 'gre' 'gree' 'green'];
RED     = ['r' 're' 'red'];
BLA   = ['k'];
WHI   = ['w'];

for j=1:length(ch)

 for i=length(ch):-1:1

  colour = part(ch,[j:i]);

  if find(or(colour==YELLOW)) <> []
   Color=color('yellow')
   ch=strsubst(ch,colour,'');
   break;
  elseif find(or(colour==MAGENTA)) <> []
    Color=color('magenta')
    ch=strsubst(ch,colour,'');
    break;
  elseif find(or(colour==CYAN)) <> []
    Color=color('cyan')
    ch=strsubst(ch,colour,'');  
    break;
  elseif find(or(colour==BLUE)) <> []
    Color=color('blue')
    ch=strsubst(ch,colour,'');  
    break;
  elseif find(or(colour==GREEN)) <> []
    Color=color('green')
    ch=strsubst(ch,colour,'');
    break;
  elseif find(or(colour==RED)) <> []
    Color=color('red')
    ch=strsubst(ch,colour,'');
    break;
  elseif find(or(colour==BLA)) <> []
    Color=color('black')
    ch=strsubst(ch,colour,'');
    break;
  elseif find(or(colour==WHI)) <> []
    Color=color('white')
    ch=strsubst(ch,colour,'');
    break;
  end

 end
end

SQUARE = ['s' 'sq' 'squ' 'squa' 'squar' 'square'];
DIAMOND =['d' 'di' 'dia' 'diam' 'diamo' 'diamon' 'diamond'];


for j=1:length(ch)

 for i=length(ch):-1:1

  mark = part(ch,[j:i]);

  if find(or(mark==SQUARE)) <> []
   MarkerStyle=12;
   MarkerSize=4;
   ch=strsubst(ch,mark,'');
   break;
  elseif find(or(mark==DIAMOND)) <> []
   MarkerStyle=13;
   MarkerSize=4;
   ch=strsubst(ch,mark,'');
   break;
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
     MarkerStyle=11;
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
     MarkerStyle=10;
     MarkerSize=4;
  else
     fail=1;
end // select
end // for


if MarkerStyle == []
   Marker = %F;
   Line = %T;
else
   Line = %F;
   Marker = %T;
end

// end of getLineSpec