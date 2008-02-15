
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2008 - INRIA - Serge STEER <serge.steer@inria.fr>
//
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt

function ge_play(fil)
//used to record the editor event in file for playback
  global ge_recording
  global evth
  global lcount
  evth=''
  ge_recording=mopen(fil,'r')
  lcount=0
  
function [%ok,%1,%2,%3,%4,%5,...
          %6,%7,%8,%9,%10,...
          %11,%12,%13,%14,%15,...
          %16,%17,%18,%19,%20]=getvalue(%desc,%labels,%typ,%ini)
  execstr(mgetl(ge_recording,1))
  global lcount;lcount=lcount+1
endfunction

function rep=xgetmouse(varargin)
  execstr(mgetl(ge_recording,1))
  global lcount;lcount=lcount+1
endfunction
function seteventhandler(s)
  global evth
  evth=s
endfunction

function rep=x_choices(tit,titems)
  execstr(mgetl(ge_recording,1))
  global lcount;lcount=lcount+1
endfunction

function num=x_message(varargin)
  execstr(mgetl(ge_recording,1))
  global lcount;lcount=lcount+1
endfunction

function result=x_dialog(labels,valueini)  
  execstr(mgetl(ge_recording,1))
  global lcount;lcount=lcount+1
endfunction

function num=x_choose(varargin)  
  execstr(mgetl(ge_recording,1))    
  global lcount;lcount=lcount+1
endfunction

function   path=tk_savefile(varargin)  
  execstr(mgetl(ge_recording,1))    
  global lcount;lcount=lcount+1
endfunction

edit_graph()
fin=%f
global lcount;
while %t
  t=mgetl(ge_recording,1); lcount=lcount+1
  //mprintf("%s\n",t)
  execstr(t)
  if fin then pause,break,end
  ge_eventhandler(win,x,y,ibut)
  //halt()
end
mclose (ge_recording)
clearglobal ge_recording
endfunction
