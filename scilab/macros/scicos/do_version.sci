function scs_m=do_version(scs_m,version)
// Copyright INRIA
//translate scicos data structure to new version
nx=size(scs_m);
if version<>'scicos2.2'&version<>'scicos2.3'&version<>'scicos2.3.1'&..
    version<>'scicos2.4' then
error('No version update defined to '+version+' version')
end

if version=='scicos2.2' then 
  scs_m=do_version231(do_version23(do_version22(scs_m)))
elseif version=='scicos2.3' then   
  scs_m=do_version231(do_version23(scs_m))
elseif version=='scicos2.3.1' then   
  scs_m=do_version231(scs_m)
end

if version<>'scicos2.5.1' then scs_m=do_version251(scs_m),end

function scs_m=do_version251(scs_m)
nx=size(scs_m)
obsolete=%f
for k=2:nx
  o=scs_m(k)
  if o(1)=='Block' then
    model=o(3)
    if model(1)=='super'|model(1)=='csuper' then
      model(8)=do_version251(model(8))
      o(3)=model
      scs_m(k)=o
    elseif o(5)=='SOM_f' then
      if and(model(8)==1) then
	model(8)=[]
	model(1)=list('plusblk',2) 
	scs_m(k)(3)=model
	scs_m(k)(5)='SUM_f'
      else
	scs_m(k)(3)(1)=list('sum',2) 
	obsolete=%t
      end
    elseif o(5)=='AFFICH_f' then  
      scs_m(k)(3)(7)=[0;-1;0;0;1;1]
      scs_m(k)(2)(9)=['xset(''font'',ipar(1),ipar(2))';
	  'str='' ''+part(''0'',ones(1,ipar(5)-ipar(6)-2))+''.''+part(''0'',ones(1,ipar(6)))'
	  'rr=xstringl(orig(1),orig(2),str)'
	  'xstring(orig(1)+max(0,(sz(1)-rr(3))/2),orig(2)+max(0,(sz(2)-rr(4))/2),str)']
    end
  elseif o(1)=="Link" then
    if size(o(2),'*')==1 then
      o(2)=o(2)*[1;1];o(3)=o(3)*[1;1];
      scs_m(k)=o
    end
  end
end
if obsolete then
  message(['Diagram contains obsolete signed blocks sum'
      'They are drawn in brown, they work as before but,'
      'please replace them with the new block sum'])
end  

function scs_m=do_version231(scs_m)

//2.3.1 to 2.4 version
if size(scs_m(1))<5 then scs_m(1)(5)=' ',end  //compatibility
if type(scs_m(1)(5))<>10 then scs_m(1)(5)=' ',end //compatibility
if size(scs_m(1))<6 then //compatibility
  wpar=scs_m(1)
  wpar(6)=list()
  wpar(7)=list(%t,[0.8 0.8 0.8])
  wpar(8)=[]
  wpar(9)=[]
  wpar(10)=[]
  scs_m(1)=wpar
end

if size(scs_m(1)(1),'*') <4 then scs_m(1)(1)=[scs_m(1)(1),0,0],end //compatibility


scs_m(1)(1)(2)=maxi(scs_m(1)(1)(2),450)
if size(scs_m(1))<6 then 
  options=default_options()
  doc=list() //documentation structure
  wpar=scs_m(1)
  wpar(6)=list()
  wpar(7)=options
  wpar(8)=[]
  wpar(9)=[]
  wpar(10)=doc
  scs_m(1)=wpar
end
wsiz=scs_m(1)(1)
if size(wsiz,'*')<6 then //compatibility
  if size(wsiz,'*')<4 then wsiz(3)=0;wsiz(4)=0;end
  wsiz(5)=wsiz(1);wsiz(6)=wsiz(2);
  scs_m(1)(1)=wsiz;
end
if type(scs_m(1)(7))==15 then //options 
  old_opt=scs_m(1)(7)
  options=default_options()
  options('3D')(1)=old_opt(1)
  options('Cmap')=old_opt(2)
  scs_m(1)(7)=options
end

nx=size(scs_m)
for k=2:nx
  o=scs_m(k)
  if o(1)=='Block' then
    model=o(3)
    if model(1)=='super'|model(1)=='csuper' then
      model(8)=do_version231(model(8))
      o(3)=model
      scs_m(k)=o
    elseif model(1)(1)=='ifthel' then
      model(1)=list('ifthel',-1)
      scs_m(k)(3)=model
    elseif model(1)(1)=='eselect' then
      model(1)=list('eselect',-1)
      scs_m(k)(3)=model  
    end
  end
end


function x_new=do_version23(scs_m)
//2.3 to 2.3.1
x_new=list()
x_new(1)=scs_m(1)
for k=2:nx
  o=scs_m(k)
  if o(1)=='Link' then
  elseif o(1)=='Block' then
    model=o(3)
    if model(1)=='super'|model(1)=='csuper' then
      model(8)=do_version(model(8))
      o(3)=model
      o=block2_version(o)
      scs_m(k)=o
    else
      o=block2_version(o)
    end
  end
  x_new(k)=o;
end


function o=block2_version(o)
if o(3)(6)<>[] then o(3)(12)(2)=%t;end

function o=block_version(o)
[graphics,model]=o(2:3)
for k=2:5, model(k)=ones(model(k),1),end
blocktype=o(5)

ok=%t
label=' '
gr_i=' '
select blocktype
case 'ABSBLK_f' then
  model(2)=-1
  model(3)=-1
  label=' '
  gr_i='xstringb(orig(1),orig(2),''abs'',sz(1),sz(2),''fill'')'
case 'ANIMXY_f' then
  [rpar,ipar]=model([8:9])
  model(9)=[model(9);0;[-1;-1];[-1;-1]]
  win=ipar(1);N=ipar(3);clrs=ipar(4);siz=ipar(5)
  xmin=rpar(1);xmax=rpar(2);ymin=rpar(3);ymax=rpar(4)
  label=[string(clrs);
      string(siz);
      string(win);
      '[]';
      '[]';
      string(xmin);
      string(xmax);
      string(ymin);
      string(ymax);
      string(N)]
  gr_i=['thick=xget(''thickness'');xset(''thickness'',2);';
      't=(0:0.3:2*%pi)'';';
      'xx=orig(1)+(1/5+(cos(2.2*t)+1)*3/10)*sz(1);';
      'yy=orig(2)+(1/4.3+(sin(t)+1)*3/10)*sz(2);';
      'xpoly(xx,yy,''lines'');'
      'xset(''thickness'',thick);']
case 'BOUND_f' then
  rpar=model(8);in=model(2);nin=sum(in)
  if nin<>1 then 
    write(%io(2),['Pb with BOUND_f block';
	'previously block has more than one input port';
	'It is better to change it with the new block version';
	'preceded by a multiplexer'])
  end
  thresh=rpar(1:nin),v=rpar(nin+1:2*nin)
  label=[strcat(sci2exp(thresh));
      strcat(sci2exp(v))]
  gr_i=['thick=xget(''thickness'');xset(''thickness'',2);';
      'xx=orig(1)+[1/5;1/2;1/2;1-1/5]*sz(1);';
      'yy=orig(2)+[1/2;1/2;1-1/5;1-1/5]*sz(2);';
      'xpoly(xx,yy,''lines'');';
      'xset(''thickness'',1);';
      'xpoly(orig(1)+[1/9;1/5]*sz(1),orig(2)+[1/2;1/2]*sz(2),''lines'');';
      'xpoly(orig(1)+[1/2;1-1/9]*sz(1),orig(2)+[1/2;1/2]*sz(2),''lines'');';
      'xpoly(orig(1)+[1/2;1/2]*sz(1),orig(2)+[1/9;1/2]*sz(2),''lines'');';
      'xpoly(orig(1)+[1/2;1/2]*sz(1),orig(2)+[1-1/5;1-1/9]*sz(2),''lines'');';
      'xset(''thickness'',thick);']
case 'CLINDUMMY_f' then
  label=[]
  gr_i=['xstringb(orig(1),orig(2),[''DUMMY'';''CLSS''],sz(1),sz(2),''fill'');']
case 'CLKIN_f' then
  prt=model(9);
  label=string(prt)
  gr_i=[]
case 'CLKOUT_f' then
  prt=model(9);
  label=string(prt)
  gr_i=[]
case 'CLKSPLIT_f' then
  label=[]
  gr_i=[]
case 'CLKSOM_f' then
  label=[]
  gr_i=[]
case 'CLOCK_f' then
  orig=o(2)(1)
  sz=o(2)(2)
  oo=o(3)(8)
  dt=0.1
  for ko=2:size(oo)
    if oo(ko)(3)(1)=='evtdly' then
      dt=oo(ko)(3)(8)
      break
    end
  end
  o=CLOCK_f('define')
  o(2)(1)=orig
  o(2)(2)=sz
  xx=o(3)(8)(3)
  xx(2)(4)=string([dt;dt])
  xx(3)(11)=dt
  xx(3)(8)=dt
  o(3)(8)(3)=xx
  model=o(3)
else
  ok=%f
end

if ok then
  graphics(4)=label
  graphics(9)=gr_i
  o(2)=graphics
  model(13)=' ';model(14)=list()
  o(3)=model
  o=replace_firing(o)
  return
end

ok=%t
select blocktype
case 'CLR_f' then
  ipar=model(9);model(9)=[]
  ns=size(model(6),'*');nin=1;nout=1;
  rpar=model(8);
  A=matrix(rpar(1:ns*ns),ns,ns);
  B=matrix(rpar(ns*ns+1:ns*(ns+nin)),ns,nin);
  C=matrix(rpar(ns*(ns+nin)+1:ns*(ns+nin+nout)),nout,ns);
  D=rpar(ns*(ns+nin+nout)+1);
  H=ss2tf(syslin('c',A,B,C,D));
  H=clean(H);
  if type(H)==16 then
    num=H(2);den=H(3)
  else
    num=H,den=1
  end
  label=[sci2exp(num);sci2exp(den)]
  gr_i=['xstringb(orig(1),orig(2),[''Num(s)'';''-----'';''Den(s)''],sz(1),sz(2),''fill'');']
case 'CLSS_f' then
  in=model(2);out=model(3)
  nin=sum(in)
  nout=sum(out)
  x0=model(6),
  rpar=model(8)
  ns=prod(size(x0))
  A=matrix(rpar(1:ns*ns),ns,ns)
  B=matrix(rpar(ns*ns+1:ns*(ns+nin)),ns,nin)
  C=matrix(rpar(ns*(ns+nin)+1:ns*(ns+nin+nout)),nout,ns)
  D=matrix(rpar(ns*(ns+nin+nout)+1:ns*(ns+nin+nout)+(nin*nout)),nout,nin)
  label=[  strcat(sci2exp(A));
      strcat(sci2exp(B));
      strcat(sci2exp(C));
      strcat(sci2exp(D));
      strcat(sci2exp(x0))]
  gr_i=['txt=[''xd=Ax+Bu'';''y=Cx+Du''];';
      'xstringb(orig(1),orig(2),txt,sz(1),sz(2),''fill'');']
  model(2)=nin;model(3)=nout
case 'CONST_f' then
  C=model(8);model(3)=sum(model(3))
  label=[strcat(sci2exp(C))]
  gr_i=['model=arg1(3);C=model(8);';
      'dx=sz(1)/5;dy=sz(2)/10;';
      'w=sz(1)-2*dx;h=sz(2)-2*dy;';
      'xstringb(orig(1)+dx,orig(2)+dy,string(C),w,h,''fill'');']
case 'COSBLK_f' then
  in=model(2);nin=sum(in)
  if nin<>1 then 
    write(%io(2),['Pb with COSBLK_f block';
	'previously block has more than one input port';
	'It is better to change it with the new block version';
	'preceded by a multiplexer and followed by a demultiplxer'])
  end
  label=' '
  gr_i=['xstringb(orig(1),orig(2),[''cos''],sz(1),sz(2),''fill'');']
case 'CURV_f' then
  label=[]
  gr_i=['model=arg1(3);rpar=model(8);ipar=model(9);n=ipar(1);';
      'thick=xget(''thickness'');xset(''thickness'',2);';
      'xx=rpar(1:n);yy=rpar(n+1:2*n);';
      'rect=rpar(2*n+1:2*n+4);';
      'mxx=rect(3)-rect(1);';
      'mxy=rect(4)-rect(2);';
      'xx=orig(1)+sz(1)*(1/10+(4/5)*((xx-rect(1))/mxx));';
      'yy=orig(2)+sz(2)*(1/10+(4/5)*((yy-rect(2))/mxy));';
      'xpoly(xx,yy,''lines'');';
      'xset(''thickness'',thick);']
case 'DELAY_f' then
  orig=o(2)(1)
  sz=o(2)(2)
  oo=o(3)(8)
  dt=[];z0=[]
  for ko=2:size(oo)
    if oo(ko)(3)(1)=='evtdly' then
      dt=oo(ko)(3)(8)
    elseif oo(ko)(3)(1)=='register' then
      z0=oo(ko)(3)(7)
    end
  end
  if dt==[]|z0==[] then
    write(%io(2), 'Pb with DELAY_f block dt or z0 not found')
    pause
  end
  o=DELAY_f('define')
  o(2)(1)=orig
  o(2)(2)=sz
  o(3)(8)(4)(2)(4)=sci2exp(z0)
  o(3)(8)(5)(2)(4)=sci2exp(dt)
  o(3)(8)(5)(3)(8)=dt
  o(3)(8)(5)(3)(11)=0
  o(3)(8)(4)(3)(7)=z0(:)
  model=o(3)
  label=[]
  gr_i=['b1=xstringl(0,0,''e'');';
      'b2=xstringl(0,0,''-Ts'');';
      'h=-b1(2)+maxi(0,sz(2)-0.5*b1(4)+b2(4))/2;';
      'w=maxi(0,sz(1)-b1(3)-b1(4))/2;';
      'xstring(orig(1)+w,orig(2)+h,''e'');';
      'xstring(orig(1)+w+b1(3)/2,orig(2)+h+b1(4)*0.5,''-Ts'');']
case 'DLRADAPT_f' then
  [dstate,rpar,ipar]=model(7:9)
  m=ipar(1);
  n=ipar(2)
  npt=ipar(3)
  p=rpar(1:npt)

  rn=matrix(rpar(npt+1:npt+m*npt)+%i*rpar(npt+m*npt+1:npt+2*m*npt),npt,m)
  rd=matrix(rpar(npt+2*m*npt+1:npt+(2*m+n)*npt)+..
      %i*rpar(npt+(2*m+n)*npt+1:npt+2*(m+n)*npt),npt,n)
  g=rpar(npt+2*(m+n)*npt+1:npt+2*(m+n)*npt+npt)
  last_u=dstate(1:m);last_y=dstate(m+1:m+n)
  label=[sci2exp(p);
      sci2exp(rn);
      sci2exp(rd);
      sci2exp(g);
      sci2exp(last_u);
      sci2exp(last_y)]
  gr_i=['txt=[''N(z,p)'';''-----'';''D(z,p)''];';
      'xstringb(orig(1),orig(2),txt,sz(1),sz(2),''fill'');']
case 'DLR_f' then
  model(9)=[];model(10)='d'
  ns=size(model(7),'*');nin=1;nout=1;
  rpar=model(8);
  A=matrix(rpar(1:ns*ns),ns,ns);
  B=matrix(rpar(ns*ns+1:ns*(ns+nin)),ns,nin);
  C=matrix(rpar(ns*(ns+nin)+1:ns*(ns+nin+nout)),nout,ns);
  D=rpar(ns*(ns+nin+nout)+1);
  H=ss2tf(syslin('d',A,B,C,D));
  H=clean(H);
  if type(H)==16 then
    num=H(2);den=H(3)
  else
    num=H,den=1
  end
  label=[sci2exp(num);sci2exp(den)]
  gr_i=['txt=[''Num(z)'';''-----'';''Den(z)''];';
      'xstringb(orig(1),orig(2),txt,sz(1),sz(2),''fill'');']
case 'DLSS_f' then
  in=model(2);out=model(3)
  nin=sum(in)
  nout=sum(out)
  x0=model(6),rpar=model(8)
  ns=prod(size(x0))
  A=matrix(rpar(1:ns*ns),ns,ns)
  B=matrix(rpar(ns*ns+1:ns*(ns+nin)),ns,nin)
  C=matrix(rpar(ns*(ns+nin)+1:ns*(ns+nin+nout)),nout,ns)
  D=matrix(rpar(ns*(ns+nin+nout)+1:ns*(ns+nin+nout)+(nin*nout)),nout,nin)

  label=[  strcat(sci2exp(A));
      strcat(sci2exp(B));
      strcat(sci2exp(C));
      strcat(sci2exp(D));
      strcat(sci2exp(x0))]
  gr_i=['txt=[''x+=Ax+Bu'';''y=Cx+Du''];';
      'xstringb(orig(1),orig(2),txt,sz(1),sz(2),''fill'');']
case 'EVENTSCOPE_f' then
  [nclock,rpar,ipar]=model([4 8 9])
  win=ipar(1);
  per=rpar(1);
  wdim=[-1;-1]
  wpos=[-1;-1]
  clrs=[1;3;5;7;9;11;13;15];
  model(9)=[win;1;clrs;wpos(:);wdim(:)]
  label=[sci2exp(nclock);
      strcat(sci2exp(clrs),' ');
      string(win);
      sci2exp([]);
      sci2exp([]);
      string(per)]
  gr_i=['thick=xget(''thickness'');xset(''thickness'',2);';
      'xrect(orig(1)+sz(1)/10,orig(2)+(1-1/10)*sz(2),sz(1)*8/10,sz(2)*8/10);';
      'xx=[orig(1)+sz(1)/5,orig(1)+sz(1)/5;';
	  'orig(1)+(1-1/5)*sz(1),orig(1)+sz(1)/5];';
      'yy=[orig(2)+sz(2)/5,orig(2)+sz(2)/5;';
	  'orig(2)+sz(2)/5,orig(2)+(1-1/5)*sz(2)];';
      'xarrows(xx,yy);';
      't=(0:0.3:2*%pi)'';';
      'xx=orig(1)+(1/5+3*t/(10*%pi))*sz(1);';
      'yy=orig(2)+(1/4.3+(sin(t)+1)*3/10)*sz(2);';
      'xpoly(xx,yy,''lines'');';
      'xset(''thickness'',thick);']
case 'EVTDLY_f' then
  dt=model(8);
  if model(11) then ff=0; else ff=-1; end
  model(11)=ff
  label=[string(dt);string(ff)]
  gr_i=['dt=model(8);';
      'txt=[''Delay'';string(dt)];';
      'xstringb(orig(1),orig(2),txt,sz(1),sz(2),''fill'');']
case 'EVTGEN_f' then
  tt=model(11);
  label=string(tt)
  gr_i=['xstringb(orig(1),orig(2),''Event'',sz(1),sz(2),''fill'');']
case 'FOR_f' then
  write(%io(2),'FOR block nor more exist')
  else
    ok=%f
  end
  if ok then
    graphics(4)=label
    graphics(9)=gr_i
    o(2)=graphics
    model(13)=' ';model(14)=list()
    o(3)=model
    o=replace_firing(o)
    return
  end
  ok=%t
  select blocktype
  case 'GAIN_f' then
    [in,out]=model(2:3)
    gain=matrix(model(8),out,in)
    label=[strcat(sci2exp(gain))]
    gr_i=['[nin,nout]=model(2:3);';
	'if nin*nout==1 then gain=string(model(8)),else gain=''Gain'',end';
	'dx=sz(1)/5;';
	'dy=sz(2)/10;';
	'xx=orig(1)+      [1 4 1 1]*dx;';
	'yy=orig(2)+sz(2)-[1 5 9 1]*dy;';
	'xpoly(xx,yy,''lines'');';
	'w=sz(1)-2*dx;h=sz(2)-2*dy;';
	'xstringb(orig(1)+dx,orig(2)+dy,gain,w,h,''fill'');']
  case 'GENERAL_f' then
    in=model(2);out=model(5)
    label=[strcat(sci2exp(in));strcat(sci2exp(out))]
    gr_i=['xstringb(orig(1),orig(2),''GENERAL'',sz(1),sz(2),''fill'');']
  case 'GENSIN_f' then
    rpar=model(8)
    label=[string(rpar(1));string(rpar(2));string(rpar(3))]
    gr_i=['txt=[''sinusoid'';''generator''];';
	'xstringb(orig(1),orig(2),txt,sz(1),sz(2),''fill'');']
  case 'GENSQR_f' then
    //rpar=model(8); ?
    Amplitude=model(7)
    label=string(Amplitude)
    gr_i=['txt=[''square wave'';''generator''];';
	'xstringb(orig(1),orig(2),txt,sz(1),sz(2),''fill'');']
  case 'HALT_f' then
    n=model(9);
    label=string(n)
    gr_i=['xstringb(orig(1),orig(2),''STOP'',sz(1),sz(2),''fill'');']
  case 'IFTHEL_f' then
    label=[]
    model(11)=[-1,-1]
    gr_i=['txt=[''If in>=0'';'' '';'' then    else''];';
	'xstringb(orig(1),orig(2),txt,sz(1),sz(2),''fill'');']
  case 'INTEGRAL_f' then
    x0=model(6);
    label=strcat(sci2exp(x0))
    gr_i=['xstringb(orig(1),orig(2),''  1/s  '',sz(1),sz(2),''fill'');']
  case 'INVBLK_f' then
    model(2)=-1;model(3)=-1
    label=' '
    gr_i=['xstringb(orig(1),orig(2),''1/u'',sz(1),sz(2),''fill'');']
  case 'IN_f' then
    prt=model(9);
    label=[string(prt)]
    model(3)=-1
    gr_i=[]
  case 'LOGBLK_f' then
    a=model(8)
    in=model(2)
    label=[string(a)]
    model(2)=-1;model(3)=-1
    gr_i=['xstringb(orig(1),orig(2),''log'',sz(1),sz(2),''fill'');']
  case 'LOOKUP_f' then
    model(10)='c'
    gr_i=['rpar=model(8);n=size(rpar,''*'')/2;';
	'thick=xget(''thickness'');xset(''thickness'',2);';
	'xx=rpar(1:n);yy=rpar(n+1:2*n);';
	'mnx=mini(xx);xx=xx-mnx*ones(xx);mxx=maxi(xx);';
	'xx=orig(1)+sz(1)*(1/10+(4/5)*xx/mxx);';
	'mnx=mini(yy);yy=yy-mnx*ones(yy);mxx=maxi(yy);';
	'yy=orig(2)+sz(2)*(1/10+(4/5)*yy/mxx);';
	'xpoly(xx,yy,''lines'');';
	'xset(''thickness'',thick);']
    label=[]
  case 'MAX_f' then
    in=model(2);nin=sum(in)
    if nin<>1 then 
      write(%io(2),['Pb with MAX_f block';
	  'previously block has more than one input port';
	  'It is better to change it with the new block version';
	  'preceded by a multiplexer'])
    end
    label=' '
    gr_i=['xstringb(orig(1),orig(2),''Max'',sz(1),sz(2),''fill'');']
  case 'MCLOCK_f' then
    label=[]
    gr_i=['txt=[''2freq clock'';''  f/n     f''];';
	'xstringb(orig(1),orig(2),txt,sz(1),sz(2),''fill'')']
  case 'MFCLCK_f' then
    dt=model(8)
    nn=model(9)  
    label=[string(dt);string(nn)]
    gr_i=['txt=[''M. freq'';''clock''];';
	'xstringb(orig(1),orig(2),txt,sz(1),sz(2),''fill'')']
  case 'MIN_f' then
    in=model(2);nin=sum(in)
    if nin<>1 then 
      write(%io(2),['Pb with MIN_f block';
	  'previously block has more than one input port';
	  'It is better to change it with the new block version';
	  'preceded by a multiplexer'])
    end
    label=' '
    gr_i=['xstringb(orig(1),orig(2),''MIN'',sz(1),sz(2),''fill'')']
  case 'NEGTOPOS_f' then
    label=[]
    gr_i=['xstringb(orig(1),orig(2),'' - to + '',sz(1),sz(2),''fill'');']
  case 'OUT_f' then
    prt=model(9);
    model(2)=-1
    label=[string(prt)]
    gr_i=[]
  case 'POSTONEG_f' then
    label=[]
    gr_i=['xstringb(orig(1),orig(2),'' + to - '',sz(1),sz(2),''fill'')']
  case 'POWBLK_f' then
    in=model(2);nin=sum(in)
    if nin<>1 then 
      write(%io(2),['Pb with MIN_f block';
	  'previously block has more than one input port';
	  'It is better to change it with the new block version';
	  'preceded by a multiplexer and followed by a demux'])
    end
    if model(8)<>[] then
      a=model(8)
    else
      a=model(9)
    end
    in=model(2)
    label=[string(a)]
    gr_i=['xstringb(orig(1),orig(2),''u^a'',sz(1),sz(2),''fill'');']
  case 'PROD_f' then
    label=[]
    gr_i=[]
    model(1)=list('prod',2)
  case 'QUANT_f' then
    rpar=model(8);ipar=model(9);
    pas=rpar(1)
    meth=ipar
    label=[string(pas);string(meth)]
    gr_i=['thick=xget(''thickness'');xset(''thickness'',2);';
	'xx=orig(1)+[1;2;2;3;3;4;4]/5*sz(1);';
	'yy= orig(2)+[1;1;2;2;3;3;4]/5*sz(2);';
	'xpoly(xx,yy,''lines'');';
	'xset(''thickness'',thick);']
  case 'RAND_f' then
    rpar=model(8);flag=model(9);
    out=model(3);nout=sum(out)
    if nout<>1 then 
      write(%io(2),['Pb with RAND_f block';
	  'previously block has more than one output port';
	  'It is better to change it with the new block version';
	  ' followed by a demux'])
    end

    a=rpar(1:nout)
    b=rpar(nout+1:2*nout)
    label=[string(flag);sci2exp(a(:));sci2exp(b(:))]
    gr_i=['txt=[''random'';''generator''];';
	'xstringb(orig(1),orig(2),txt,sz(1),sz(2),''fill'')']
  case 'REGISTER_f' then
    z0=model(7)
    label=strcat(string(z0),';')

    gr_i=['dly=model(8);';
	'txt=[''Shift'';''Register'';string(dly)];';
	'xstringb(orig(1),orig(2),txt,sz(1),sz(2),''fill'')']
  else
    ok=%f
  end
  if ok then
    graphics(4)=label
    graphics(9)=gr_i
    o(2)=graphics
    model(13)=' ';model(14)=list()
    o(3)=model
    o=replace_firing(o)
    return
  end
  ok=%t
  select blocktype
  case 'RFILE_f' then
    [out,state,ipar]=model([3 7 9])
    nout=sum(out)
    ievt=ipar(3);N=ipar(4);
    imask=5+ipar(1)+ipar(2)
    if ievt<>0 then tmask=ipar(imask),else tmask=[],end
    outmask=ipar(imask+ievt:imask+nout+ievt-1)
    lunit=state(2)
    ievt=ipar(3);N=ipar(4);
    imask=5+ipar(1)+ipar(2)
    if ievt<>0 then tmask=ipar(imask),else tmask=[],end
    outmask=ipar(imask+ievt:imask+nout+ievt-1)
    lfil=ipar(1)
    lfmt=ipar(2)
    if lfil>0 then fname=code2str(ipar(5:4+lfil)),else fname=' ',end
    if lfmt>0 then Fmt=code2str(ipar(5+lfil:4+lfil+lfmt)),else Fmt=' ',end
    label=[ sci2exp(tmask);
	sci2exp(outmask);
	fname;
	Fmt;
	string(N);
	sci2exp(out)]
    gr_i=['txt=[''read from'';''input file''];';
	'xstringb(orig(1),orig(2),txt,sz(1),sz(2),''fill'')']
  case 'SAT_f' then
    rpar=model(8)
    minp=rpar(1),maxp=rpar(2),pente=rpar(3)
    label=[string(minp);string(maxp);string(pente)]

    gr_i=['thick=xget(''thickness'');xset(''thickness'',2);';
	'xx=orig(1)+[4/5;1/2+1/5;1/2-1/5;1/5]*sz(1);';
	'yy=orig(2)+[1-1/5;1-1/5;1/5;1/5]*sz(2);';
	'xpoly(xx,yy,''lines'');';
	'xset(''thickness'',thick)']
  case 'SAWTOOTH_f' then
    model(10)='c'
    gr_i=['txt=[''sawtooth'';''generator''];';
	'xstringb(orig(1),orig(2),txt,sz(1),sz(2),''fill'');']
  case 'SCOPE_f' then
    [in,rpar,ipar]=model([2 8 9])
    in=model(2);nin=sum(in)
    if nin<>1 then 
      write(%io(2),['Pb with SCOPE_f block';
	  'previously block has more than one input port';
	  'It is better to change it with the new block version';
	  'preceded by a multiplexer'])
    end
    win=ipar(1);N=ipar(3);
    clrs=-ipar(4:nin+3)
    if size(clrs,'*')<8 then clrs(8)=1;end

    wdim=[-1;-1]
    wpos=[-1;-1]
    model(9)=[win;1;N;clrs;wpos(:);wdim(:)]
    dt=rpar(1);ymin=rpar(2);ymax=rpar(3);per=rpar(4)
    label=[strcat(string(clrs),' ');
	string(win);
	sci2exp([]);
	sci2exp([]);
	string(ymin);
	string(ymax);
	string(per);
	string(N)];
    
    gr_i=['thick=xget(''thickness'');xset(''thickness'',2);';
	'xrect(orig(1)+sz(1)/10,orig(2)+(1-1/10)*sz(2),sz(1)*8/10,sz(2)*8/10);';
	'xx=[orig(1)+sz(1)/5,orig(1)+sz(1)/5;';
	    'orig(1)+(1-1/5)*sz(1),orig(1)+sz(1)/5];';
	'yy=[orig(2)+sz(2)/5,orig(2)+sz(2)/5;';
	    'orig(2)+sz(2)/5,orig(2)+(1-1/5)*sz(2)];';
	'xarrows(xx,yy);';
	't=(0:0.3:2*%pi)'';';
	'xx=orig(1)+(1/5+3*t/(10*%pi))*sz(1);';
	'yy=orig(2)+(1/4.3+(sin(t)+1)*3/10)*sz(2);';
	'xpoly(xx,yy,''lines'');';
	'xset(''thickness'',thick)']
  case 'SCOPXY_f' then
    [rpar,ipar]=model(8:9)
    win=ipar(1);N=ipar(3);clrs=-ipar(4);siz=ipar(5)
    xmin=rpar(1);xmax=rpar(2);ymin=rpar(3);ymax=rpar(4)
    wdim=[-1;-1]
    wpos=[-1;-1]
    label=[sci2exp(clrs);
	sci2exp(siz);
	string(win);
	sci2exp([]);
	sci2exp([]);
	string(xmin);
	string(xmax);
	string(ymin);
	string(ymax);
	string(N)];
    model(9)=[win;1;N;clrs;siz;1;wpos(:);wdim(:)]
    gr_i=['thick=xget(''thickness'');xset(''thickness'',2);';
	't=(0:0.2:2*%pi)'';';
	'xx=orig(1)+(1/5+(cos(3*t)+1)*3/10)*sz(1);';
	'yy=orig(2)+(1/4.3+(sin(t+1)+1)*3/10)*sz(2);';
	'xpoly(xx,yy,''lines'');';
	'xset(''thickness'',thick)']
  case 'SELECT_f' then
    z0=model(7);nin=size(model(2),1);
    label=[string(nin);string(z0-1)]
    model(1)=list('selector',2)
    gr_i=['xstringb(orig(1),orig(2),''Selector'',sz(1),sz(2),''fill'');']
  case 'SINBLK_f' then
    in=model(2);nin=sum(in)
    if nin<>1 then 
      write(%io(2),['Pb with MIN_f block';
	  'previously block has more than one input port';
	  'It is better to change it with the new block version';
	  'preceded by a multiplexer and followed by a demux'])
    end
    label=' '
    gr_i=['xstringb(orig(1),orig(2),''sin'',sz(1),sz(2),''fill'')']
  case 'SOM_f' then
    sgn=model(8);
    label=sci2exp(sgn)
    model(1)=list('sum',2)
    gr_i=[]
  case 'SPLIT_f' then
    label=[]
    gr_i=[]
  case 'SUPER_f' then
    label=[]
    gr_i=['thick=xget(''thickness'');xset(''thickness'',2);';
	'xx=orig(1)+      [2 4 4]*(sz(1)/7);';
	'yy=orig(2)+sz(2)-[2 2 6]*(sz(2)/10);';
	'xrects([xx;yy;[sz(1)/7;sz(2)/5]*ones(1,3)]);';
	'xx=orig(1)+      [1 2 3 4 5 6 3.5 3.5 3.5 4 5 5.5 5.5 5.5]*sz(1)/7;';
	'yy=orig(2)+sz(2)-[3 3 3 3 3 3 3   7   7   7 7 7   7   3  ]*sz(2)/10;';
	'xsegs(xx,yy,0);';
	'xset(''thickness'',thick)']
  case 'TANBLK_f' then
    in=model(2);nin=sum(in)
    if nin<>1 then 
      write(%io(2),['Pb with TANBLK_f block';
	  'previously block has more than one input port';
	  'It is better to change it with the new block version';
	  'preceded by a multiplexer and followed by a demux'])
    end
    label=' '
    gr_i=['xstringb(orig(1),orig(2),''tan'',sz(1),sz(2),''fill'');']
  case 'TCLSS_f' then
    in=model(2);nin=sum(in)
    out=model(3);nout=sum(out)
    x0=model(6),rpar=model(8)
    
    ns=prod(size(x0));nin=nin-ns
    A=matrix(rpar(1:ns*ns),ns,ns)
    B=matrix(rpar(ns*ns+1:ns*(ns+nin)),ns,nin)
    C=matrix(rpar(ns*(ns+nin)+1:ns*(ns+nin+nout)),nout,ns)
    D=matrix(rpar(ns*(ns+nin+nout)+1:ns*(ns+nin+nout)+(nin*nout)),nout,nin)
    nin1=nin;nout1=nout

    label=[strcat(sci2exp(A));
	strcat(sci2exp(B));
	strcat(sci2exp(C));
	strcat(sci2exp(D));
	strcat(sci2exp(x0))]
    gr_i=['txt=[''Jump'';''(A,B,C,D)''];';
	'xstringb(orig(1),orig(2),txt,sz(1),sz(2),''fill'')']
  else
    ok=%f
  end
  if ok then
    graphics(4)=label
    graphics(9)=gr_i
    o(2)=graphics
    model(13)=' '
    model(14)=list()
    o(3)=model
    o=replace_firing(o)
    return
  end  
  ok=%t
  select blocktype
  case 'TEXT_f' then
    ipar=model(9)
    font=ipar(1);siz=ipar(2)
    label=[graphics(4);string(font);string(siz)]
    gr_i=[]
  case 'TIME_f' then
    label=[]
    gr_i=['wd=xget(''wdim'').*[1.016,1.12];';
	'thick=xget(''thickness'');xset(''thickness'',2);';
	'p=wd(2)/wd(1);p=1;';
	'rx=sz(1)*p/2;ry=sz(2)/2;';
	'xarc(orig(1)+0.05*sz(1),orig(2)+0.95*sz(2),0.9*sz(1)*p,0.9*sz(2),0,360*64);';
	'xset(''thickness'',1);';
	'xx=[orig(1)+rx    orig(1)+rx;';
	    'orig(1)+rx    orig(1)+rx+0.6*rx*cos(%pi/6)];';
	'yy=[orig(2)+ry    orig(2)+ry ;';
	    '	  orig(2)+1.8*ry  orig(2)+ry+0.6*ry*sin(%pi/6)];';
	'xsegs(xx,yy,0);';
	'xset(''thickness'',thick);']
  case 'TRASH_f' then
    in=model(2);nin=sum(in)
    if nin<>1 then 
      write(%io(2),['Pb with MIN_f block';
	  'previously block has more than one input port';
	  'It is better to change it with the new block version';
	  'preceded by a multiplexer'])
    end
    label=' '
    gr_i=['xstringb(orig(1),orig(2),''Trash'',sz(1),sz(2),''fill'')']
  case 'WFILE_f' then
    state=model(7)
    [in,ipar]=model([2  9])
    N=ipar(3);
    lunit=state(2)
    N=ipar(4)
    lfil=ipar(1)
    lfmt=ipar(2)
    if lfil>0 then fname=code2str(ipar(5:4+lfil)),else fname=' ',end
    if lfmt>0 then Fmt=code2str(ipar(5+lfil:4+lfil+lfmt)),else Fmt=' ',end
    label=[sci2exp(in);
	fname;
	Fmt;
	string(N)]
    
    gr_i=['txt=[''write to'';''output file''];';
	'xstringb(orig(1),orig(2),txt,sz(1),sz(2),''fill'')']
  case 'ZCROSS_f' then
    in=model(2)
    label=strcat(sci2exp(in))
    gr_i=['xstringb(orig(1),orig(2),''Zcross'',sz(1),sz(2),''fill'');']
  case 'func_block' then
    write(%io(2),['PB with func_block';
	'version change is not implemented'])
    label=[]
    gr_i=['xstringb(orig(1),orig(2),''Func'',sz(1),sz(2),''fill'');']
  case 'm_sin' then
    rpar=model(8);gain=rpar(1);phase=rpar(2)
    label=[string(gain);string(phase)]
    gr_i=[]
  case 'sci_block' then
    write(%io(2),['PB with sci_block';
	'version change is not implemented'])
    label=[]
    gr_i=['xstringb(orig(1),orig(2),''Sci_Block'',sz(1),sz(2),''fill'');']
  case 'scifunc_block' then
    write(%io(2),['PB with scifunc_block';
	'version change is not implemented'])
    i1=model(2);o1=model(3);ci1=model(4);co1=model(5);x1=model(6);
    z1=model(7);auto1=model(11);type_1=model(10);
    tt=model(9),rpar=model(8)
    label=list([sci2exp(i1);sci2exp(o1);sci2exp(ci1);sci2exp(co1);
	strcat(sci2exp(x1));strcat(sci2exp(z1));type_1;
	strcat(sci2exp(rpar));sci2exp(auto1)],tt)
    
    gr_i=['xstringb(orig(1),orig(2),''Scifunc'',sz(1),sz(2),''fill'');']
  case 'standard_block' then
    write(%io(2),['PB with standard_block';
	'version change is not implemented'])
    label=[]
    gr_i=[]
  else
    disp(blocktype+' unknown, parameters values may be lost, please check')
    label=[]
    gr_i=[]
  end
  graphics(4)=label
  graphics(9)=gr_i
  o(2)=graphics
  model(13)=' '
  model(14)=list()
  o(3)=model
  o=replace_firing(o)

function o=replace_firing(o)
firing=o(3)(11)
cout=o(3)(5)
if firing==%f|firing==0 then 
  o(3)(11)=-ones(cout)
  disp('firing changed from %f to '+sci2exp(o(3)(11))+' in '+o(3)(1)(1))
elseif firing==%t|firing==1  then 
  o(3)(11)=0*cout
  disp('firing changed from %t to '+sci2exp(o(3)(11))+' in '+o(3)(1)(1))  
elseif firing==[] then
  o(3)(11)=[]
else
  write(%io(2),'Problem with block '+o(3)(1)(1))
  pause
end

function x_new=do_version22(scs_m)
x_new=list()
wpar=scs_m(1)
wsiz=wpar(1)

if size(wsiz,'*')>=4 then x_new=x,return,end

wpar(3)(4)=wpar(4)+1
x_new(1)=wpar



for k=2:nx
  o=scs_m(k)
  if o(1)=='Link' then
  elseif o(1)=='Block' then
    model=o(3)
    if model(1)=='super'|model(1)=='csuper' then
      model(8)=do_version(model(8))
      o(3)=model
      o=block_version(o)
      scs_m(k)=o
    else
      o=block_version(o)
    end
  elseif o(1)=='Text' then
    o=block_version(o)
  end
  x_new(k)=o;
end

