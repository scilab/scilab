
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2008 - INRIA - Serge STEER <serge.steer@inria.fr>
//
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt

function ge_record(fil)
//used to record the editor event in file for playback
  global ge_recording
  ge_recording=mopen(fil,'w')
  
  getvaluestd=getvalue
function [%ok,%1,%2,%3,%4,%5,...
          %6,%7,%8,%9,%10,...
          %11,%12,%13,%14,%15,...
          %16,%17,%18,%19,%20]=getvalue(%desc,%labels,%typ,%ini)
  lhs=argn(1)
  LHS=['%ok','%'+string(1:lhs-1)]
  execstr('['+strcat(LHS,',')+']=getvaluestd(%desc,%labels,%typ,%ini)')
  if  ge_recording<>[] then
    vals=[];for f=LHS,execstr('vals=[vals sci2exp('+f+',0)]');end
    mputl('['+strcat(LHS,',')+']=('+strcat(vals,',')+') //getvalue',ge_recording);
  end
endfunction

xgetmousestd=xgetmouse
function rep=xgetmouse(varargin)
  rep=xgetmousestd(varargin(:))
  mputl('rep='+sci2exp(rep,0)+'//xgetmouse',ge_recording);
endfunction

ge_eventhandlerstd=ge_eventhandler
function ge_eventhandler(win,x,y,ibut)
  if ibut==-1 then return,end //fast exit on mouse move
  LHS=['win','x','y','ibut']
  vals=[];for f=LHS,execstr('vals=[vals sci2exp('+f+',0)]');end
  mputl('['+strcat(LHS,',')+']=('+strcat(vals,',')+')//eventhandler',ge_recording);
  ge_eventhandlerstd(win,x,y,ibut)
endfunction


x_choicesstd=x_choices
function rep=x_choices(tit,titems)
  rep=x_choicesstd(tit,titems)
  mputl('rep='+sci2exp(rep,0)+'//x_choices',ge_recording);
endfunction

x_messagestd=x_message
function num=x_message(varargin)
  num=x_messagestd(varargin(:))
  mputl('num='+sci2exp(num,0)+'//x_message',ge_recording);
endfunction

x_dialogstd=x_dialog
function result=x_dialog(labels,valueini)  
  result=x_dialogstd(labels,valueini)  
  mputl('result='+sci2exp(result,0)+'//x_dialog',ge_recording);
endfunction

x_choosestd=x_choose
function num=x_choose(varargin)  
  num=x_choosestd(varargin(:))  
  mputl('num='+sci2exp(num,0)+'//x_choose',ge_recording);
endfunction

tk_savefilestd=tk_savefile
function   path=tk_savefile(varargin)  
  path=tk_savefilestd(varargin(:))  
  mputl('path='+sci2exp(path,0)+'//tk_savefile',ge_recording);
endfunction


edit_graph()
while or(winsid()==0),end
mclose(ge_recording)
clearglobal ge_recording
endfunction
