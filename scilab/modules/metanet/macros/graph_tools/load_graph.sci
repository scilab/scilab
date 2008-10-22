
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2002-2008 - INRIA - Serge STEER <serge.steer@inria.fr>
//
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt

function [g,modified,msg]=load_graph(path)
//Loads a graph data structrure from a file.

  with_error=argn(1)<>3
  g=[];modified=%f,msg=[];
  // path
  if type(path)<>10|prod(size(path))<>1 then
    msg='Argument must be a string'
    if with_error then error(msg),else return,end
  end
 
  
  //test for old ascii file
  [u,ierr]=mopen(path,'rb')
  if ierr<>0 then 
    msg='file '+path+' cannot be opened'
    if with_error then error(msg),else return,end
  end
  test=mget(10,'c',u);
  mclose(u)
  old=and(test==ascii('GRAPH TYPE'))
  if old then //old formatted graph file
    ierr=execstr('g=load_old_graph(path)','errcatch')
    if(ierr<>0) then
      msg=lasterror()
      if with_error then error(msg),else return,end
    end
  else //new binary version
    [nams,typs]=listvarinfile(path)  
    if typs==[]|and(typs<>16) then
      msg='given file contains no graph data structure'
      if with_error then error(msg),else return,end
    end
    if size(typs,'*')>1 then
      msg='given file contains more than one variable'
      if with_error then error(msg),else return,end
    end
    ierr=execstr('load(path)','errcatch')
    if(ierr<>0) then
      msg=lasterror()
      if with_error then error(msg),else return,end
    end
    execstr('g='+nams)
    if exists('g','local')==0|typeof(g)<>'graph' then
      msg='Loaded file contains no graph data structure'
      if with_error then error(msg),else return,end
    end
  end
  //update the graph list according to the current version
  [g,edited]=update_graph(g)
endfunction

function g=load_old_graph(path)
  g=m6loadg(pathconvert(path,%f,%t))
  g=glist(g(:))
  g($+1)=[]; g($+1)=[]
endfunction
