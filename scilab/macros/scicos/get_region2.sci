function [reg,rect,prt]=get_region2(xc,yc,win)
// Copyright INRIA
ok=%t
alu=xget('alufunction')
wins=curwin
xset('window',win)
xset('alufunction',6)
reg=list();rect=[]
kc=find(win==windows(:,2))
if kc==[] then
  message('This window is not an active palette')
  return
elseif windows(kc,1)<0 then //click dans une palette
  kpal=-windows(kc,1)
  scs_m=palettes(kpal)
elseif win==curwin then //click dans la fenetre courante
  scs_m=scs_m
elseif pal_mode&win==lastwin then 
  scs_m=scs_m_s
elseif slevel>1 then
  execstr('scs_m=scs_m_'+string(windows(kc,1)))
else
  message('This window is not an active palette')
  return
end

[ox,oy,w,h,ok]=get_rectangle(xc,yc)
if ~ok then prt=[];rect=[];return;end
[keep,del]=get_blocks_in_rect(scs_m,ox,oy,w,h)
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
  nreg=size(reg)
  k1=prt(k,2)
  o1=reg(k1)
  orient=o1(2)(3)
  if prt(k,1)==1 then //input port
    [x,y,vtyp]=getinputs(o1)
    typ=prt(k,4)
    from=[nreg+1,1]
    to=[prt(k,2),prt(k,3)]
    if typ==1 then //input regular port
      x=x(prt(k,3))
      y=y(prt(k,3))
      nin=nin+1
      sp=IN_f('define')
      sz=20*sp(2)(2)
      sp(2)(2)=sz; //sz
      sp(2)(3)=orient; //flip
      sp(2)(4)=string(nin); //expr
      sp(2)(6)=nreg+2;//pout
      sp(3)(9)=nin; //port number
      o1(2)(5)(prt(k,3))=nreg+2
      if orient then  //not flipped
	sp(2)(1)=[x-2*sz(1),y-sz(2)/2]; //orig
	xl=[x-sz(1);x]
	yl=[y;y]
      else // flipped
	sp(2)(1)=[x+sz(1),y-sz(2)/2]; //orig
	xl=[x+sz(1);x]
	yl=[y;y]
      end
      prt(k,1:2)=[1,nin]
    else //input event port
      p=prt(k,3)+size(find(vtyp==1),'*')
      x=x(p)
      y=y(p)
      ncin=ncin+1
      sp=CLKINV_f('define')
      sz=20*sp(2)(2)
      sp(2)(1)=[x-sz(1)/2,y+sz(2)]; //orig
      sp(2)(2)=sz; //sz
      sp(2)(4)=string(ncin); //expr
      sp(2)(8)=nreg+2;//peout
      sp(3)(9)=ncin; //port number
      o1(2)(7)(prt(k,3))=nreg+2
      xl=[x;x]
      yl=[y+sz(2);y]
      prt(k,1:2)=[3,ncin]
    end
  else //output port
    [x,y,vtyp]=getoutputs(o1)
    typ=prt(k,4)
    if typ==1 then //output regular port
      x=x(prt(k,3))
      y=y(prt(k,3))
      nout=nout+1
      sp=OUT_f('define')
      sz=20*sp(2)(2)
      sp(2)(2)=sz; //sz
      sp(2)(3)=orient; //flip
      sp(2)(4)=string(nout); //expr
      sp(2)(5)=nreg+2;//pin
      sp(3)(9)=nout; //port number
      o1(2)(6)(prt(k,3))=nreg+2
      if orient then  //not flipped 
	sp(2)(1)=[x+sz(1),y-sz(2)/2]; //orig
	xl=[x;x+sz(1)]
	yl=[y;y]
      else //flipped
	sp(2)(1)=[x-2*sz(1),y-sz(2)/2]; //orig
	xl=[x;x-sz(1)]
	yl=[y;y]
      end
      to=[nreg+1,1]
      from=[prt(k,2),prt(k,3)]
      prt(k,1:2)=[2,nout]
    else //output event port
      p=prt(k,3)+size(find(vtyp==1),'*')
      x=x(p)
      y=y(p)
      ncout=ncout+1
      sp=CLKOUTV_f('define')
      sz=20*sp(2)(2)
      sp(2)(1)=[x-sz(1)/2,y-2*sz(2)]; //orig
      sp(2)(2)=sz; //sz
      sp(2)(4)=string(ncout); //expr
      sp(2)(7)=nreg+2;//pein
      sp(3)(9)=ncout; //port number
      o1(2)(8)(prt(k,3))=nreg+2
      to=[nreg+1,1]
      from=[prt(k,2),prt(k,3)]
      xl=[x;x]
      yl=[y;y-sz(2)]
      prt(k,1:2)=[4,ncout]
    end
  end
  lk=list('Link',xl,yl,'drawlink',' ',[0,0],[prt(k,5),typ],from,to)
  reg(nreg+1)=sp
  reg(nreg+2)=lk
  reg(k1)=o1
end
reg=do_purge(reg)

