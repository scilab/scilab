function [ok,tt]=MODCOM(funam,tt,vinp,vout,vparam,vparamv,vpprop,prev_exprs)
//
 [dirF,nameF,extF]=fileparts(funam);

  //the new head
 class_txt_new=build_classhead(funam,vinp,vout,vparam,vparamv,vpprop)
 //the previous head
 class_txt_prev=build_prev_classhead(prev_exprs)

 if (tt==[]) then
   tete5='equation';

   tete6=['      // exemple'];
   tete7='      //der(x)=x-x*y;';
   tete8='      //der(y)+2*y=x*y;';
   tete9='end '+nameF+';';
   textmp=[class_txt_new;tete5;tete6;tete7;tete8;tete9];
 else
   modif=%f;
   for i=1:size(tt,'*')
     if strindex(stripblanks(tt(i)),'equation')<>[] then
        //Alan, 07/10/07
        //tt(1:i-1) : the current head
        if and(tt(1:i-1)==class_txt_prev) then
           textmp=[class_txt_new;tt(i:$)]
           modif=%t
           break
        end
     end
   end
   if ~modif then textmp=tt, end;
 end

 while %t
   txt=textmp;
   if (extF=='' | (extF=='.mo' & fileinfo(funam)==[])) then
    [txt]=x_dialog(['Function definition in Modelica';
         'Here is a skeleton of the functions which you should edit'],textmp);
   end
   if txt<>[] then // not a  Cancel button
     tt=txt
     tarpath=pathconvert(TMPDIR+'/Modelica/',%f,%t);
     //saving in the filename
     if (extF=='')  then
       funam=tarpath+nameF+'.mo';
       mputl(tt,funam);
     elseif fileinfo(funam)==[] then
       mputl(tt,funam);
     end
     compilerpath=pathconvert(SCI+'/bin/',%f,%t);
     if MSDOS then
       compilerpath=compilerpath+'modelicac.exe';
     else
       compilerpath=compilerpath+'modelicac';
     end

     if execstr('unix_s(compilerpath+'' -c ''+..
                 funam+'' -o ''+tarpath+nameF+''.moc'')','errcatch')<>0 then
       x_message(["An error has occured during the compilation";
                  "of the modelica block. Please check."])
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

//build_classhead : build the head of the modelica function
function class_txt=build_classhead(funam,vinp,vout,vparam,vparamv,vpprop)

  [dirF,nameF,extF]=fileparts(funam);

  ni=size(vinp,'r');
  no=size(vout,'r');
  np=size(vparam,'r');

  tete1=['class '+nameF]
  //------------------------------------------
  if np<>0 then
    tete1b= '      //parameters';
    for i=1:np
      //param
      if vpprop(i)==0 then
         head='      parameter Real '
         cm=''
      //continuous state
      elseif vpprop(i)==1 then
         head='      Real           '
         cm='continuous state, '
      //fixed continuous state
      elseif vpprop(i)==2 then
         head='      Real           '
         cm='fixed continuous state, '
      end

      if size(vparamv(i),'*')==1 then
        tete1b=[tete1b;
                head+..
                 vparam(i)+'; //'+..
                  cm+'init value '+..
                   string(vparamv(i))];
      else
        tete1b=[tete1b;
                head+..
                 vparam(i)+'['+...
                 string(size(vparamv(i),'*'))+']; //'+..
                  cm+'init values '+..
                   sci2exp(vparamv(i))];
      end
    end
  else
    tete1b=[];
  end
  //---------------------------------------
  //------------------------------------------
  if ni<>0 then
    tete2= '      Real ';
    for i=1:ni
      tete2=tete2+vinp(i);
      if (i==ni) then  tete2=tete2+';';else  tete2=tete2+',';end
    end
    //tete2=tete2+'  //input variables ';
    tete2=['      //input variables';
           tete2];
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
    //tete3=tete3+'  //output variables';
    tete3=['      //output variables';
           tete3];
  else
   tete3=[];
  end
   //-----------------------------------------

  tete4= ['';' //     Real x(start=1), y(start=2);']

  class_txt=[tete1;tete1b;tete2;tete3;tete4]
endfunction

//build_prev_classhead : get the head of the previous modelica function
function class_txt=build_prev_classhead(exprs)
  class_txt=[]

  funam=exprs.nameF
  vinp=evstr(exprs.in)
  vout=evstr(exprs.out)
  vparam=evstr(exprs.param)
  vparamv=exprs.paramv
  vpprop=evstr(exprs.pprop)

  class_txt=build_classhead(funam,vinp,vout,vparam,vparamv,vpprop)

endfunction