function [ok,tt]=MODCOM(funam,tt,vinp,vout)
//
[dirF,nameF,extF]=fileparts(funam);

ni=size(vinp,'r');
no=size(vout,'r');

if (tt==[]) then
  tete1=['class '+nameF]
  //------------------------------------------
  if ni<>0 then 
    tete2= '      Real '; 
    for i=1:ni
      tete2=tete2+vinp(i);
      if (i==ni) then  tete2=tete2+';';else  tete2=tete2+',';end
    end
    tete2=tete2+'  //input (known variables) ';
  else
    tete2=[];
  end
  //---------------------------------------
  if no<>0 then 
    tete3= '      Real '
    for i=1:no
      tete3=tete3+vout(i);
      if (i==no) then  tete3=tete3+';';else  tete3=tete3+',';end
    end
    tete3=tete3+'  //output variables';
  else
    tete3=[];
  end
  //-----------------------------------------
  tete4= ' //     Real x(start=1), y(start=2);'
  tete5='equation';
  
  tete6=['      // exemple'];
  //  for i=1:no
  //    tete6=[tete6;'      '+vo(i)+'=x;'];
  //  end
  //  tete6=tete6+'  //you can modify the outputs';
  //  tete6=[tete6;'//--------------------------'];
  
  tete7='      //der(x)=x-x*y;';
  tete8='      //der(y)+2*y=x*y;';
tete9='end '+nameF+';';
textmp=[tete1;tete2;tete3;tete4;tete5;tete6;tete7;tete8;tete9];
else
  textmp=tt;
end

while %t
  txt=textmp;
  if (extF=='' | (extF=='.mo' & fileinfo(funam)==[])) then
   [txt]=x_dialog(['Function definition in Modelica';
        'Here is a skeleton of the functions which you shoud edit'],textmp);
  end
  if txt<>[] then // not a  Cancel button
    tt=txt
    tarpath=pathconvert(TMPDIR+'/Modelica/',%f,%t);
    if (extF=='')  then
      funam=tarpath+nameF+'.mo';
      mputl(tt,funam);
    elseif fileinfo(funam)==[] then
      mputl(tt,funam);//saving in the filename
    end
    compilerpath=pathconvert(SCI+'/bin/',%f,%t);
    if MSDOS then
      compilerpath=compilerpath+'modelicac.exe';
    else
      compilerpath=compilerpath+'modelicac';
    end
    
    if execstr('unix_s(compilerpath+'' -c ''+funam+'' -o ''+tarpath+nameF+''.moc'')','errcatch')<>0 then
      textmp=txt;
      ok=%f;
    else
      tt=txt;
      ok=%t;
      break
    end
  else
    ok=%f; // cancel bouton
    break
  end
end
endfunction

