function [scs_m,fct]=do_addnew(scs_m)
//add a new block (defined by its GUI function to a palette
//!
// Copyright INRIA
fct=[]
[ok,name]=getvalue('Get block GUI function name',..
    ['Name'],list('str',1),emptystr())

if ~ok then return,end
to_get=%f
if exists(name)==0 then 
  to_get=%t
else
  execstr('tp=type('+name+')')
  to_get=tp<>11&tp<>13 
end
if to_get then // try to get it
  path=name+'.sci'
  path=xgetfile(path)
  if length(path)<=0 then return,end
  [u,err]=file('open',path,'old','formatted')
  if err<>0 then
    message(path+' file, Not found')
    return
  end
  errcatch(-1,'continue')
  getf(u)
  errcatch(-1)
  file('close',u)
  if iserror(-1) then
    errclear(-1)
    message(name+' erroneous function see message in scilab window')
    return
  end
  fct=path
end

//define the block
errcatch(-1,'continue')
execstr('blk='+name+'(''define'')')
errcatch(-1)
if iserror(-1) then
  message('Error in GUI function see message in scilab window')
  errclear(-1)
  fct=[]
  return
end
xinfo('Choose block position in the window')
rep(3)=-1
blk(2)(2)=20*blk(2)(2);
[xy,sz]=blk(2)(1:2)
// clear block
// draw block shape
  dr=driver()
  if dr=='Rec' then driver('X11'),end

xrect(%xc,%yc+sz(2),sz(1),sz(2))
if pixmap then xset('wshow'),end
while rep(3)==-1 , //move loop
  // clear block shape
  xrect(%xc,%yc+sz(2),sz(1),sz(2))
  if pixmap then xset('wshow'),end
  // get new position
  rep=xgetmouse(0)
  %xc=rep(1);%yc=rep(2)
  xy=[%xc,%yc];
  // draw block shape
  xrect(%xc,%yc+sz(2),sz(1),sz(2))
  if pixmap then xset('wshow'),end
end
xinfo(' ')
// clear  block shape
xrect(%xc,%yc+sz(2),sz(1),sz(2))
if pixmap then xset('wshow'),end
// update and draw block
blk(2)(1)=xy
driver(dr)

drawobj(blk)
if pixmap then xset('wshow'),end

scs_m($+1)=blk



