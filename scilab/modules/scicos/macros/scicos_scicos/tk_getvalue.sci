//  Scicos
//
//  Copyright (C) INRIA - METALAU Project <scicos@inria.fr>
//                      - Alan Layec <alan.layec@inria.fr>
//
// This program is free software; you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation; either version 2 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program; if not, write to the Free Software
// Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
//
// See the file ../license.txt
//

//**
//** @OBSOLETE
//**
function [%ok,%1,%2,%3,%4,%5,...
          %6,%7,%8,%9,%10,...
          %11,%12,%13,%14,%15,...
          %16,%17,%18,%19,%20]=tk_getvalue(%desc,%labels,%typ,%ini)
  warnobsolete();
//  getvalues - %window dialog for data acquisition
//%Synta%
//  [%ok,%1,..,%11]=getvalue(desc,labels,typ,ini)
//%Parameters
//  desc    : column vector of strings, dialog general comment
//  labels  : n column vector of strings, labels(i) is the label of
//            the ith required value
//  typ     : list(typ1,dim1,..,typn,dimn)
//            typi : defines the type of the ith required value
//                   if may have the following values:
//                   'mat' : stands for matrix of scalars
//                   'col' : stands for column vector of scalars
//                   'row' : stands for row vector of scalars
//                   'vec' : stands for  vector of scalars
//                   'str' : stands for vector of strings
//                   'lis' : stands for list
//                   'pol' : stands for polynomials
//                   'r'   : stands for rational
//            dimi : defines the size of the ith required value
//                   it must be
//                    - an integer or a 2-vector of integers (-1 stands for
//                      arbitrary dimension)
//                    - an evaluatable character string
//  ini     : n column vector of strings, ini(i) gives the suggested
//            response for the ith required value
//  %ok      : boolean ,%t if %ok button pressed, %f if cancel button pressed
//  xi      : contains the ith required value if %ok==%t
//%Description
// getvalues macro encapsulate x_mdialog function with error checking,
// evaluation of numerical response, ...
//%Remarks
// All correct scilab syntax may be used as responses, for matrices
// and vectors getvalues automatically adds [ ] around the given response
// before numerical evaluation
//%Example
// labels=['magnitude';'frequency';'phase    '];
// [ampl,freq,ph]=getvalue('define sine signal',labels,..
//            list('vec',1,'vec',1,'vec',1),['0.85';'10^2';'%pi/3'])
//
//%See also
// x_mdialog, x_dialog
//!
// 17/01/07 -Alan- -%scicos_context behavior reviewed in accordance to context_evstr macro
//                 - pass int in field of type vec/mat/row/col (F. Nassif's Work)
//
// 05/02/07 -Alan- : update to %20 rhs parameters
//

[%lhs,%rhs]=argn(0)

%nn=prod(size(%labels))
if %lhs<>%nn+2&%lhs<>%nn+1 then error(41),end
if size(%typ)<>2*%nn then
  error('%typ : list(''type'',[sizes],...)')
end
%1=[];%2=[];%3=[];%4=[];%5=[];
%6=[];%7=[];%8=[];%9=[];%10=[];
%11=[];%12=[];%13=[];%14=[];%15=[];
%16=[];%17=[];%18=[];%19=[];%20=[];

if %rhs==3 then  %ini=emptystr(%nn,1),end
%ok=%t
while %t do
  %str=mdialog(%desc,%labels,%ini)
  if %str==[] then %ok=%f,%str=[];break,end
  //%str=%str1;
  for %kk=1:%nn
    %cod=ascii(%str(%kk))
    %spe=find(%cod==10)
    if %spe<>[] then
      %semi=ascii(';')
      %cod(%spe)=%semi*ones(%spe')
      %str(%kk)=ascii(%cod)
    end
  end

  [%vv_list,%ierr_vec]=context_evstr(%str,%scicos_context);

  %nok=0
  for %kk=1:%nn
    %vv=%vv_list(%kk)
    %ierr=%ierr_vec(%kk)
    select part(%typ(2*%kk-1),1:3)
    case 'mat'
      if %ierr<>0 then %nok=-%kk;break,end
      //29/12/06
      //the type of %vv is accepted if it is constant or integer
      if and(type(%vv)<>[1 8]) then %nok=-%kk,break,end
      %sz=%typ(2*%kk);if type(%sz)==10 then %sz=evstr(%sz),end
      [%mv,%nv]=size(%vv)
      %ssz=string(%sz(1))+' x '+string(%sz(2))
      if %mv*%nv==0 then
	if  %sz(1)>=0&%sz(2)>=0&%sz(1)*%sz(2)<>0 then %nok=%kk,break,end
      else
	if %sz(1)>=0 then if %mv<>%sz(1) then %nok=%kk,break,end,end
	if %sz(2)>=0 then if %nv<>%sz(2) then %nok=%kk,break,end,end
      end
    case 'vec'
      if %ierr<>0 then %nok=-%kk;break,end
      //17/01/07
      //the type of %vv is accepted if it is constant or integer
      if and(type(%vv)<>[1 8]) then %nok=-%kk,break,end
      %sz=%typ(2*%kk);if type(%sz)==10 then %sz=evstr(%sz),end
      %ssz=string(%sz(1))
      %nv=prod(size(%vv))
      if %sz(1)>=0 then if %nv<>%sz(1) then %nok=%kk,break,end,end
    case 'pol'
      if %ierr<>0 then %nok=-%kk;break,end
      if type(%vv)>2 then %nok=-%kk,break,end
      %sz=%typ(2*%kk);if type(%sz)==10 then %sz=evstr(%sz),end
      %ssz=string(%sz(1))
      %nv=prod(size(%vv))
      if %sz(1)>=0 then if %nv<>%sz(1) then %nok=%kk,break,end,end
    case 'row'
      if %ierr<>0 then %nok=-%kk;break,end
      //17/01/07
      //the type of %vv is accepted if it is constant or integer
      if and(type(%vv)<>[1 8]) then %nok=-%kk,break,end
      %sz=%typ(2*%kk);if type(%sz)==10 then %sz=evstr(%sz),end
      if %sz(1)<0 then
	%ssz='1 x *'
      else
	%ssz='1 x '+string(%sz(1))
      end
      [%mv,%nv]=size(%vv)
      if %mv<>1 then %nok=%kk,break,end,
      if %sz(1)>=0 then if %nv<>%sz(1) then %nok=%kk,break,end,end
    case 'col'
      if %ierr<>0 then %nok=-%kk;break,end
      //17/01/07
      //the type of %vv is accepted if it is constant or integer
      if and(type(%vv)<>[1 8]) then %nok=-%kk,break,end
      %sz=%typ(2*%kk);if type(%sz)==10 then %sz=evstr(%sz),end
      if %sz(1)<0 then
	%ssz='* x 1'
      else
	%ssz=string(%sz(1))+' x 1'
      end
      [%mv,%nv]=size(%vv)
      if %nv<>1 then %nok=%kk,break,end,
      if %sz(1)>=0 then if %mv<>%sz(1) then %nok=%kk,break,end,end
    case 'str'
      %sde=%str(%kk)
      %spe=find(ascii(%str(%kk))==10)
      %spe($+1)=length(%sde)+1
      clear %vv
      %vv=[];%kk1=1
      for %kkk=1:size(%spe,'*')
	%vv(%kkk,1)=part(%sde,%kk1:%spe(%kkk)-1)
	%kk1=%spe(%kkk)+1
      end
      %sz=%typ(2*%kk);if type(%sz)==10 then %sz=evstr(%sz),end
      %ssz=string(%sz(1))
      %nv=prod(size(%vv))
      if %sz(1)>=0 then if %nv<>%sz(1) then %nok=%kk,break,end,end
    case 'lis'
      if %ierr<>0 then %nok=-%kk;break,end
      if type(%vv)<>15& type(%vv)<>16& type(%vv)<>17 then %nok=-%kk,break,end
      %sz=%typ(2*%kk);if type(%sz)==10 then %sz=evstr(%sz),end
      %ssz=string(%sz(1))
      %nv=size(%vv)
      if %sz(1)>=0 then if %nv<>%sz(1) then %nok=%kk,break,end,end
    case 'r  '
      if %ierr<>0 then %nok=-%kk;break,end
      if type(%vv)<>16 then %nok=-%kk,break,end
      if typeof(%vv)<>'rational' then %nok=-%kk,break,end
      %sz=%typ(2*%kk);if type(%sz)==10 then %sz=evstr(%sz),end
      [%mv,%nv]=size(%vv(2))
      %ssz=string(%sz(1))+' x '+string(%sz(2))
      if %mv*%nv==0 then
	if  %sz(1)>=0&%sz(2)>=0&%sz(1)*%sz(2)<>0 then %nok=%kk,break,end
      else
	if %sz(1)>=0 then if %mv<>%sz(1) then %nok=%kk,break,end,end
	if %sz(2)>=0 then if %nv<>%sz(2) then %nok=%kk,break,end,end
      end
    else
      error('type non gere :'+%typ(2*%kk-1))
    end
    execstr('%'+string(%kk)+'=%vv')
    clear %vv
  end
  if %nok>0 then
    x_message(['answer given for '+%labels(%nok);
             'has invalid dimension: ';
             'waiting for dimension  '+%ssz])
    %ini=%str
  elseif %nok<0 then
    if %ierr==0 then
      x_message(['answer given for '+%labels(-%nok);
	'has incorrect type :'+ %typ(-2*%nok-1)])
    else
      x_message(['answer given for '+%labels(-%nok);
	'is incorrect:'+lasterror()])
    end
    %ini=%str
  else
    break
  end
end
if %lhs==%nn+2 then
  execstr('%'+string(%lhs-1)+'=%str')
end
endfunction

//**
//** @OBSOLETE
//**

function result=mdialog(titlex,items,init)
  warnobsolete()
if argn(2)<1 then
  titlex=['this is a demo';'this is a demo']
  items=['item 1';'item 2']
end
if argn(2)<3 then
  init=['init 1';'init 2']
end
titlex=sci2tcl(titlex);
for i=1:size(items,'*')
  items(i)=sci2tcl(items(i))
  init(i)=sci2tcl(init(i))
end
txt=create_txt(titlex,items,init);
result=[];
TCL_EvalStr(txt)
done=TCL_GetVar('done')
if done==string(1) then
 for i=1:size(items,'*')
   execstr('result(i)=TCL_GetVar(''x'+string(i)+''')')
 end
end
TCL_EvalStr('set numx [winfo x $w];set numy [winfo y $w];destroy $w')
endfunction

//**
//** @OBSOLETE
//**

function txt=create_txt(titlex,items,init)
warnobsolete()
//** Alan, 13/10/07 : patch to retrieve last of position
 //**                  of the window

 //** retrieve current postion of the last dialog box
 //** potential TCL global variables numx/numy
 if TCL_ExistVar('numx') then
   numx=TCL_GetVar('numx')
   numx_tt='set numx '+numx
 else
   numx_tt='set numx [winfo pointerx .]'
 end

 if TCL_ExistVar('numy') then
   numy=TCL_GetVar('numy')
   numy_tt='set numy '+numy
 else
   numy_tt='set numy [winfo pointery .]'
 end

txt=['set w .form'
     'catch {destroy $w}'
     'toplevel $w'
       numx_tt
       numy_tt
//      'set numx [winfo pointerx .]'
//      'set numy [winfo pointery .]'
     'wm geometry $w +$numx+$numy'
     'wm title $w '"Set Block properties'"'
     'wm iconname $w '"form'"'
     '#positionWindow $w'
     'label $w.msg  -wraplength 4i -justify left -text '"'+titlex+''"'
     'frame $w.buttons'
     'pack $w.buttons -side bottom -fill x -pady 2m'
     'button $w.buttons.dismiss -text Dismiss -command {set done 2}'
     'button $w.buttons.code -text OK -command {set done 1}'
     'pack $w.buttons.dismiss $w.buttons.code -side left -expand 1'];

for i=1:size(items,'*')
  txt=[txt
       'frame $w.f'+string(i)+' -bd 2'
       'entry $w.f'+string(i)+'.entry -relief sunken -width 40'
       'label $w.f'+string(i)+'.label'
       'pack $w.f'+string(i)+'.entry -side right'
       'pack $w.f'+string(i)+'.label -side left'];
end
for i=1:size(items,'*')
 txt=[txt
      '$w.f'+string(i)+'.label config -text '"'+items(i)+''"'];
end
for i=1:size(items,'*')
 txt=[txt
      '$w.f'+string(i)+'.entry insert 0 '"'+init(i)+''"'];
end

tt=''
for i=1:size(items,'*')
  tt=tt+'global x'+string(i)+';set x'+string(i)+' [$w.f'+string(i)+'.entry get];'
  //tt=tt+'ScilabEval '"result('+string(i)+')=x'+string(i)+''";'
end
txt=[txt;
     'proc done1 {w} {'+tt+'}']
tt=''
for i=1:size(items,'*')
  tt=tt+'$w.f'+string(i)+' '
end
txt=[txt;
     'pack $w.msg '+tt+'-side top -fill x'
     'focus $w.f1.entry'
     'set done 0'
     'bind $w <Return> {set done 1}'
     'bind $w <Destroy> {set done 2}'
     'tkwait variable done'
     'if {$done==1} {done1 $w}']
endfunction

