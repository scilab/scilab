//
//Author : F.Leray
//Copyright INRIA
//

function [Color,CurColor]=setDefaultColor(CurColor)

// Adds a quadruple to the list of elementary plots to do

MAXCOL = 8;

ColorTab=[0         0    1.0000;
          0    0.5000         0;
     1.0000         0         0;
          0    0.7500    0.7500;
     0.7500         0    0.7500;
     0.7500    0.7500         0;
     0.2500    0.2500    0.2500];


CurColor = CurColor+1;

if(modulo(CurColor,MAXCOL)==0)
  CurColor=1;
else
  CurColor=modulo(CurColor,MAXCOL);
end

//disp('ColorTab(CurColor,:)=')
//disp(ColorTab(CurColor,:))
Color=addcolor(ColorTab(CurColor,:));


endfunction



