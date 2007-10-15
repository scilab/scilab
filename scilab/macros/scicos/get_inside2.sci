function reg=get_inside2(SelectRegion)
// Copyright INRIA
  reg=list();
  [ox,oy,w,h,win]=SelectRegion(:)
  kc=find(win==windows(:,2))
  if kc==[] then
    message('This window is not an active palette')
    return
  elseif windows(kc,1)<0 then //click dans une palette
    kpal=-windows(kc,1)
    scs_m=palettes(kpal)
  elseif win==curwin then //click dans la fenetre courante
    scs_m=scs_m
  elseif slevel>1 then
    execstr('scs_m=scs_m_'+string(windows(kc,1)))
  else
    message('This window is not an active palette')
    return
  end
  [keep,del]=get_blocks_in_rect(scs_m,ox,oy,w,h)
  
  
  for bkeep=keep

    if typeof(scs_m.objs(bkeep))=='Block' then
      if or(scs_m.objs(bkeep).gui==['IN_f' 
		    'OUT_f'
		    'CLKINV_f'
		    'CLKIN_f'
		    'CLKOUTV_f'
		    'CLKOUT_f'
                    'INIMPL_f'
		    'OUTIMPL_f']) then
	message('Input/Output ports are not allowed in the region.')
	prt=[];rect=[];return
      end
    end
  end

  //preserve information on splitted links

  prt=splitted_links(scs_m,keep,del)

  [reg,DEL]=do_delete2(scs_m,del,%f)

  rect=[ox,oy-h,w,h]

  xset('window',wins)
  xset('alufunction',alu)

  nin=0
  nout=0
  ncin=0
  ncout=0
 
  //add input and output ports
  for k=1:size(prt,1)
    nreg=size(reg.objs)
    k1=prt(k,1); typ=prt(k,5);tp=prt(k,3)
    o1=reg.objs(k1) //block inside the region
    orient=o1.graphics.flip

    if tp==1 then //input port
      // build the link between block and port
      if typ>1 then //implicit regular link
	[x,y,vtyp]=getinputports(o1)
	from=[nreg+1,1,0] //added port
	to=prt(k,1:3)
      else	    //explicit regular link
	[x,y,vtyp]=getinputs(o1),
	from=[nreg+1,1,0] //added port
	to=prt(k,1:3)
      end

      if typ>0 then //input regular port
	x=x(prt(k,2))
	y=y(prt(k,2))
	nin=nin+1
	if typ==1 then sp=IN_f('define'),else sp=INIMPL_f('define'),end
	sz=20*sp.graphics.sz
	sp.graphics.sz=sz; //sz
	sp.graphics.flip=orient; //flip
	sp.graphics.exprs=string(nin); //expr
	sp.graphics.pout=nreg+2;//pout
	sp.model.ipar=nin; //port number

	o1.graphics.pin(prt(k,2))=nreg+2
	if orient then  //not flipped
	  sp.graphics.orig=[x-2*sz(1),y-sz(2)/2]; //orig
	  xl=[x-sz(1);x]
	  yl=[y;y]
	else // flipped
	  sp.graphics.orig=[x+sz(1),y-sz(2)/2]; //orig
	  xl=[x+sz(1);x]
	  yl=[y;y]
	end
	prt(k,2)=nin
      else //input event port
	p=prt(k,2)+size(find(vtyp==1),'*')
	x=x(p)
	y=y(p)
	ncin=ncin+1
	sp=CLKINV_f('define')
	sz=20*sp.graphics.sz
	sp.graphics.orig=[x-sz(1)/2,y+sz(2)]; //orig
	sp.graphics.sz=sz; //sz
	sp.graphics.exprs=string(ncin); //expr
	sp.graphics.peout=nreg+2;//peout
	sp.model.ipar=ncin; //port number
	
	o1.graphics.pein(prt(k,2))=nreg+2
	xl=[x;x]
	yl=[y+sz(2);y]
	prt(k,2)=ncin
      end
    else //  output port
      if typ>1 then //implicit regular link
	[x,y,vtyp]=getoutputports(o1)
	to=[nreg+1,1,1]
	from=prt(k,1:3)
      else //explicit regular link
	[x,y,vtyp]=getoutputs(o1)
	to=[nreg+1,1,1]
	from=prt(k,1:3)
      end
      if typ>0 then //output regular port
	x=x(prt(k,2))
	y=y(prt(k,2))
	nout=nout+1
	if typ==1 then sp=OUT_f('define'),else sp=OUTIMPL_f('define'),end
	sz=20*sp.graphics.sz
	sp.graphics.sz=sz; //sz
	sp.graphics.flip=orient; //flip
	sp.graphics.exprs=string(nout); //expr
	sp.graphics.pin=nreg+2;//pin
	sp.model.ipar=nout; //port number
	o1.graphics.pout(prt(k,2))=nreg+2
	sz=sp.graphics.sz
	if orient then  //not flipped 
	  sp.graphics.orig=[x+sz(1),y-sz(2)/2]; //orig
	  xl=[x;x+sz(1)]
	  yl=[y;y]
	else //flipped
	  sp.graphics.orig=[x-2*sz(1),y-sz(2)/2]; //orig
	  xl=[x;x-sz(1)]
	  yl=[y;y]
	end
	prt(k,2)=nout
      else //output event port
	p=prt(k,2)+size(find(vtyp==1),'*')
	x=x(p)
	y=y(p)
	ncout=ncout+1
	sp=CLKOUTV_f('define')
	sz=20*sp.graphics.sz
	sp.graphics.orig=[x-sz(1)/2,y-2*sz(2)]; //orig
	sp.graphics.sz=sz; //sz
	sp.graphics.exprs=string(ncout); //expr
	sp.graphics.pein=nreg+2;//pein
	sp.model.ipar=ncout; //port number
	o1.graphics.peout(prt(k,2))=nreg+2
	xl=[x;x]
	yl=[y;y-sz(2)]
	prt(k,2)=ncout
      end
    end
    lk=scicos_link(xx=xl,yy=yl,ct=[prt(k,4),typ],from=from,to=to)
    reg.objs(nreg+1)=sp
    reg.objs(nreg+2)=lk
    reg.objs(k1)=o1
  end

  reg=do_purge(reg)
endfunction
