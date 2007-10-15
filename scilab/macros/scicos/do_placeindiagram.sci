function scs_m=do_placeindiagram(scs_m,blk)
// Copyright INRIA
//  blk.graphics.sz = 20*blk.graphics.sz;
  %xc = 100; %yc = 100; //** default start position
   xy =[%xc,%yc] ;
  blk.graphics.orig = xy

  gh_blk=drawobj(blk); //** draw the block (in the buffer) - using the corresponding Interface Function
                       //** Scilab Language - of the specific block (blk) and get back the graphic handler
		       //** to handle the block as a single entity

//**-----------------------------------------------------------------
//** ---> main loop that move the empty box until you click
  rep(3)=-1 ;
  while rep(3)==-1 , //move loop

    // get new position
    rep = xgetmouse(0,[%t,%t])

    //** Protection from window closing
    if rep(3)==-100 then //active window has been closed
      [%win,Cmenu] = resume(curwin,'Quit')
    end

    xm=rep(1) ; ym = rep(2) ;
    dx = xm - %xc ; dy = ym - %yc ;
    drawlater();
    move (gh_blk , [dx dy]);
    draw(gh_blk.parent);
    show_pixmap();

    %xc = xm ;%yc = ym ; //** position update

  end //** ---> of the while loop
//**----------------------------------------------------------------------
  //** window closing protection
  if xget('window') <> curwin then
    //active window has been closed
    [%win,Cmenu]=resume(curwin,'Quit')
  end

  xinfo(' ')

  xy = [%xc,%yc];
  blk.graphics.orig = xy ; //** update object position in the data strucure

  scs_m_save = scs_m,nc_save=needcompile ;
   
  scs_m.objs($+1) = blk ; //** add the object to the data structure

  needcompile = 4     
  [scs_m_save, nc_save, enable_undo, edited] = resume(scs_m_save,nc_save,%t,%t)

endfunction

//**---------------------------------------------------------------------------------------------------

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


