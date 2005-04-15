function [scs_m,fct]=do_addnew(scs_m)
//add a new block (defined by its GUI function to a palette
//!
// Copyright INRIA
  fct=[]
  [ok,name]=getvalue('Get block GUI function name',..
		     ['Name'],list('str',1),emptystr())

  if ~ok then return,end
  name=stripblanks(name);
  if name==emptystr() then message('No block name specified');return,end
  to_get=%f
  if exists(name)==0 then 
    to_get=%t
  else
    execstr('tp=type('+name+')')
    to_get=tp<>11&tp<>13 
  end
  if to_get then // try to get it
    path=name+'.sci'
    path=getfile("*.sci",path,title="File containing function: "+name)

    if length(path)<=0 then return,end
    [u,err]=file('open',path,'old','formatted')
    if err<>0 then
      message(path+' file, Not found')
      return
    end
    if execstr('getf(u)','errcatch')<>0 then
      file('close',u)
      message([name+' erroneous function:';lasterror()])
      return
    end 
    file('close',u)
    if ~exists(name) then message(name+' is not defined in this file'),return,end
    fct=path
  else
    fct=emptystr()
  end

  //define the block
  ierror=execstr('blk='+name+'(''define'')','errcatch')
  if ierror <>0 & ierror <>4 then
    message(['Error in GUI function';lasterror()] )
    fct=[]
    return
  end
  if ierror==4 then
    irr=message(['Error in GUI function--The error was:';
		lasterror();'It could be an old GUI';
	     'Should I try to translate (no guarantee)?'],['yes','no'])
    if irr==2 then fct=[];return
    else
      funcprot_val=funcprot()
      funcprot(0)
      standard_define=standard_define_old;
      funcprot(funcprot_val)
      ierror=execstr('blk='+name+'(''define'')','errcatch')
      if ierror <>0 then
	message("Translation did not work, sorry" )
	fct=[]
	return
      end
      do_version=do_version;//load do_version and its subfunctions
      ierror=execstr('blk=up_to_date(blk)','errcatch');
      if ierror <>0 then
	message("Translation did not work, sorry" )
	fct=[]
	return
      end
    end
  end
  
  xinfo('Choose block position in the window')
  rep(3)=-1
  blk.graphics.sz=20*blk.graphics.sz;
  [xy,sz]=(blk.graphics.orig,blk.graphics.sz)
  // clear block
  // draw block shape
  dr=driver()
  if dr=='Rec' then driver('X11'),end
  %xc=%pt(1);%yc=%pt(2);
  xrect(%xc+0,%yc+sz(2),sz(1),sz(2))
  if pixmap then xset('wshow'),end
  while rep(3)==-1 , //move loop
    // get new position
    rep=xgetmouse(0)
    if rep(3)==-100 then //active window has been closed
      driver(dr);
      [%win,Cmenu]=resume(curwin,'Quit')
    end
    // clear block shape
    xrect(%xc+0,%yc+sz(2),sz(1),sz(2))
    if pixmap then xset('wshow'),end
 
    %xc=rep(1);%yc=rep(2)
    xy=[%xc,%yc];
    // draw block shape
    xrect(%xc,%yc+sz(2),sz(1),sz(2))
    if pixmap then xset('wshow'),end
  end
  if xget('window')<>curwin then
    xrect(%xc,%yc+sz(2),sz(1),sz(2))
    if pixmap then xset('wshow'),end
    //active window has been closed
    [%win,Cmenu]=resume(curwin,'Quit')
  end
  
  xinfo(' ')
  // clear  block shape
  xrect(%xc,%yc+sz(2),sz(1),sz(2))
  if pixmap then xset('wshow'),end
  // update and draw block
  blk.graphics.orig=xy
  driver(dr)

  drawobj(blk)
  if pixmap then xset('wshow'),end

  scs_m.objs($+1)=blk
endfunction


function objsi =up_to_date(o)
  if size(o(2)) > 8 then
    if type(o(2)(9))==15 then 
      gr_io=o(2)(9)(1);
      if o(2)(9)(2)<>[] then
	back_col=o(2)(9)(2);,
      end
    else
      gr_io=o(2)(9);
      back_col=8
    end
    gr_i=convert_gri(o(5),gr_io);
    if gr_i==[] then gr_i=gr_io;, end
  elseif size(o(2)) < 9 then
    gr_i=[];
    back_col=8
  end
  gr_i=list(gr_i,back_col)
  
  mdl=o(3);
  if size(o(3))<=12 then 
    mdl(13)=''; mdl(14)=[] ; mdl(15)='';
  elseif size(o(3))<=13 then 
    mdl(14)=[] ; mdl(15)='';
  elseif size(o(3))<=14 then 
    mdl(15)='';
  end
  
  if mdl(1)(1)=='super'|mdl(1)(1)=='csuper' then
    if type(mdl(8))==15 then
      mdl(8)=do_version27(mdl(8))
    end
  end
  
  graphics=scicos_graphics(orig=o(2)(1),sz=o(2)(2),flip=o(2)(3),..
			   exprs=o(2)(4),pin=o(2)(5),pout=o(2)(6),..
			   pein=o(2)(7),peout=o(2)(8),gr_i=gr_i,..
			   id=mdl(15)) 	       
  
  
  model=scicos_model(sim=mdl(1),in=mdl(2),out=mdl(3),evtin=mdl(4),..
		     evtout=mdl(5),state=mdl(6),dstate=mdl(7),..
		     rpar=mdl(8),ipar=mdl(9),blocktype=mdl(10),..
		     firing=mdl(11),dep_ut=mdl(12),label=mdl(13))
  
  
  objsi=scicos_block(graphics=graphics,model=model,gui=o(5),..
		     doc=mdl(14))
  if objsi.gui=='ESELECT_f' then objsi.model.sim(2)=-2,end
endfunction

function o=standard_define_old(sz,model,label,gr_i)
//initialize graphic part of the block data structure
// Copyright INRIA
[lhs,rhs]=argn(0)
if rhs<4 then gr_i=[],end
[nin,nout,ncin,ncout]=model(2:5)
nin=size(nin,1);if nin>0 then pin(nin,1)=0,else pin=[],end
nout=size(nout,1);if nout>0 then pout(nout,1)=0,else pout=[],end
ncin=size(ncin,1);if ncin>0 then pcin(ncin,1)=0,else pcin=[],end
ncout=size(ncout,1);if ncout>0 then pcout(ncout,1)=0,else pcout=[],end
graphics=list([0,0],sz,%t,label,pin,pout,pcin,pcout,gr_i)
if model(1)(1)=='super' then
   o=list('Block',graphics,model,' ','SUPER_f')
else
  [ln,mc]=where()
  o=list('Block',graphics,model,' ',mc(2))
end
endfunction

