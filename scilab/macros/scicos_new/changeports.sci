function scs_m=changeports(scs_m,path,o_n)
// Move  change number of ports of block with path path and modify connected links if any
//!
//look at connected links
// Copyright INRIA
  connected=[];dx=[];dy=[]
  o=scs_m(path)
  k=path($)
  [nin_n,nout_n,ncin_n,ncout_n]=(o_n.model.in,o_n.model.out,o_n.model.evtin,o_n.model.evtout)
  [sz,orient,ip,op,cip,cop]=(o.graphics.sz,o.graphics.flip,o.graphics.pin,o.graphics.pout,..
			     o.graphics.pein,o.graphics.peout)
  [nin,nout,ncin,ncout]=(o_n.model.in,o_n.model.out,o_n.model.evtin,o_n.model.evtout)
  //standard inputs
  nip=size(ip,'*')
  nipn=size(nin_n,'*')
  ipn=ip
  if nip>nipn then
    //number of input ports decreased
    if or(ip(nipn+1:$)>0) then
      message('Connected ports cannot be suppressed')
      return
    end
    kc=find(ip>0)
    ipn=ip(1:nipn)
  elseif nip<nipn then
    //number of input ports increased 
    if ip<>[] then kc=find(ip>0),else kc=[];end
    ipn=[ip;zeros(nipn-nip,1)]
  end
  if  nip<>nipn then
    connected=[connected ip(kc)']
    dy=[dy,(-sz(2)/(nipn+1)+sz(2)/(nip+1))*kc]; //new-old
    dx=[dx,0*kc];
  end

  //standard outputs
  nop=size(op,'*')
  nopn=size(nout_n,'*')
  opn=op
  if nop>nopn then
    //number of output ports decreased
    if or(op(nopn+1:$)>0) then
      message('Connected ports cannot be suppressed')
      return
    end
    kc=find(op>0)
    opn=op(1:nopn)
  elseif nop<nopn then
    //number of input ports increased 
    if op<>[] then kc=find(op>0),else kc=[];end
    opn=[op;zeros(nopn-nop,1)]
  end
  if nop<>nopn then
    connected=[connected op(kc)']
    dy=[dy,(-sz(2)/(nopn+1)+sz(2)/(nop+1))*kc]; //new-old
    dx=[dx,0*kc];
  end


  //event inputs
  ncip=size(cip,'*')
  ncipn=size(ncin_n,'*')
  cipn=cip
  if ncip>ncipn then
    //number of input ports decreased
    if or(cip(ncipn+1:$)>0) then
      message('Connected ports cannot be suppressed')
      return
    end
    kc=find(cip>0)
    cipn=cip(1:ncipn)
  elseif ncip<ncipn then
    //number of input ports increased 
    if cip<>[] then kc=find(cip>0),else kc=[];end
    cipn=[cip;zeros(ncipn-ncip,1)]
  end
  if ncip<>ncipn then
    connected=[connected cip(kc)']
    dx=[dx,(sz(1)/(ncipn+1)-sz(1)/(ncip+1))*kc]
    dy=[dy,0*kc]
  end

  //event outputs 
  ncop=size(cop,'*')
  ncopn=size(ncout_n,'*')
  copn=cop
  if ncop>ncopn then
    //number of output ports decreased
    if or(cop(ncopn+1:$)>0) then
      message('Connected ports cannot be suppressed')
      return
    end
    kc=find(cop>0)
    copn=cop(1:ncopn)
  elseif ncop<ncopn then
    //number of input ports increased 
    if cop<>[] then kc=find(cop>0),else kc=[];end
    copn=[cop;zeros(ncopn-ncop,1)]
  end
  if ncop<>ncopn then
    connected=[connected,cop(kc)']
    dx=[dx, (sz(1)/(ncopn+1)-sz(1)/(ncop+1))*kc]
    dy=[dy,0*kc]
  end
  // update  block
  o_n.graphics.pin=ipn
  o_n.graphics.pout=opn
  o_n.graphics.pein=cipn
  o_n.graphics.peout=copn
  o_n.model.in=nin_n
  o_n.model.out=nout_n
  o_n.model.evtin=ncin_n
  o_n.model.evtout=ncout_n
  //*********************************
  if nip<>nipn|nop<>nopn|ncip<>ncipn|ncop<>ncopn then
    // build movable segments for all connected links
    //===============================================
    xx=[];yy=[];ii=[];clr=[];mx=[];my=[]

    for i1=1:size(connected,'*')
      i=connected(i1)
      oi=scs_m.objs(i)
      //[xl,yl,ct,from,to]=oi([2,3,7:9])
      [xl,yl,ct,from,to]=(oi.xx,oi.yy,oi.ct,oi.from,oi.to)
      clr=[clr ct(1)]
      nl=prod(size(xl))
      if from(1)==k then
	ii=[ii i]
	// build movable segments for this link
	if nl>=4 then
	  x1=xl(1:4)
	  y1=yl(1:4)
	elseif nl==3 then 
	  // 3 points link add one point at the begining
	  x1=xl([1 1:3])
	  y1=yl([1 1:3])
	elseif xl(1)==xl(2)|yl(1)==yl(2) then 
	  // vertical or horizontal   2 points link add a point in the middle
	  x1=[xl(1);xl(1)+(xl(2)-xl(1))/2;xl(1)+(xl(2)-xl(1))/2;xl(2)]
	  y1=[yl(1);yl(1)+(yl(2)-yl(1))/2;yl(1)+(yl(2)-yl(1))/2;yl(2)]
	else
	  // oblique 2 points link add 2 points in the middle
	  x1=[xl(1);xl(1)+(xl(2)-xl(1))/2;xl(1)+(xl(2)-xl(1))/2;xl(2)]
	  y1=[yl(1);yl(1);yl(2);yl(2)]
	end
	//set allowed (x or y) move for each points of build movable segments
	if nl==3 then
	  if xl(1)==xl(2) then 
	    mx=[mx,[1;1;1;0]]
	    my=[my,[1;1;0;0]]
	  else
	    mx=[mx,[1;1;0;0]]
	    my=[my,[1;1;1;0]]
	  end
	else
	  if xl(1)==xl(2) then
	    mx=[mx,[1;1;0;0]]
	    my=[my,[1;1;1;0]]
	  else
	    mx=[mx,[1;0;0;0]]
	    my=[my,[1;1;0;0]]
	  end
	end
	mx(:,$)=mx(:,$)*dx(i1)
	my(:,$)=my(:,$)*dy(i1)
	xx=[xx x1];yy=[yy y1]  //store  movable segments for this link
      elseif to(1)==k then
	ii=[ii -i]
	// build movable segments
	if nl>=4 then
	  x1=xl(nl:-1:nl-3)
	  y1=yl(nl:-1:nl-3)
	elseif nl==3 then 
	  // 3 points link add one point at the end
	  sel=[nl:-1:nl-2,nl-2]
	  x1=xl([nl nl:-1:nl-2])
	  y1=yl([nl nl:-1:nl-2])
	elseif xl(1)==xl(2)|yl(1)==yl(2) then 
	  // vertical or horizontal 2 points link add a point in the middle
	  xm=xl(2)+(xl(1)-xl(2))/2
	  x1= [xl(2);xm;xm;xl(1)]
	  ym=yl(2)+(yl(1)-yl(2))/2;
	  y1= [yl(2);ym;ym;yl(1)]
	else
	  // oblique 2 points link add 2 points in the middle
	  xm=xl(2)+(xl(1)-xl(2))/2
	  x1=[xl(2);xm;xm;xl(1)]
	  y1=[yl(2);yl(2);yl(1);yl(1)]
	end
	if nl==3 then
	  if x1(2)==x1(3) then 
	    mx=[mx,[1;1;1;0]]
	    my=[my,[1;1;0;0]]
	  else
	    mx=[mx,[1;1;0;0]]
	    my=[my,[1;1;1;0]]
	  end
	else
	  if x1(1)==x1(2) then
	    mx=[mx,[1;1;0;0]]
	    my=[my,[1;1;1;0]]
	  else
	    mx=[mx,[1;0;0;0]]
	    my=[my,[1;1;0;0]]
	  end
	end
	mx(:,$)=mx(:,$)*dx(i1)
	my(:,$)=my(:,$)*dy(i1)
	xx=[xx x1];yy=[yy y1] 
      end
    end
    [mxx,nxx]=size(xx)
    if connected<>[] then // move connected links  
      
      // erase moving part of links
      xpolys(xx,yy,clr)
      // draw moving part of links
      xx=xx+mx
      yy=yy+my
      xpolys(xx,yy,clr)
      //udate moved links in scicos structure
      for i=1:prod(size(ii))
	oi=scs_m.objs(abs(ii(i)))
	// xl=oi(2);yl=oi(3);nl=prod(size(xl))
	xl=oi.xx;yl=oi.yy;nl=prod(size(xl))
	if ii(i)>0 then
	  if nl>=4 then
	    xl(1:4)=xx(:,i)
	    yl(1:4)=yy(:,i)
	  elseif nl==3 then
	    xl=xx(2:4,i)
	    yl=yy(2:4,i)
	  else
	    xl=xx(:,i)
	    yl=yy(:,i)
	  end
	else
	  if nl>=4 then
	    xl(nl-3:nl)=xx(4:-1:1,i)
	    yl(nl-3:nl)=yy(4:-1:1,i)
	  elseif nl==3 then
	    xl=xx(4:-1:2,i)
	    yl=yy(4:-1:2,i)
	  else
	    xl=xx(4:-1:1,i)
	    yl=yy(4:-1:1,i)
	  end
	end
	nl=prod(size(xl))
	//eliminate double points
	kz=find((xl(2:nl)-xl(1:nl-1))^2+(yl(2:nl)-yl(1:nl-1))^2==0)
	xl(kz)=[];yl(kz)=[]
	//store
	oi.xx=xl;oi.yy=yl;
	scs_m.objs(abs(ii(i)))=oi;
      end
    end

  end
  // redraw block
  drawobj(o) //clear old block
  drawobj(o_n)// draw new block

  if pixmap then xset('wshow'),end
  // update block in scicos structure
  scs_m.objs(k)=o_n
endfunction
