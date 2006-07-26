function nyquist(sl,fmin,fmax,pas,comments)
// Nyquist plot
//!
// Copyright INRIA
[lhs,rhs]=argn(0);
sltyp='x';
pas_def='auto' //valeur du pas par defaut
//---------------------
//xbasc()
ilf=0
typ=type(sl)
//-compat next line added for list/tlist compatibility
if typ==15 then typ=16,end
select typ
case 16 then  // sl,fmin,fmax [,pas] [,comments]
  typ=sl(1);typ=typ(1);
  if typ<>'lss'&typ<>'r' then
    error(97,1)
  end
  sltyp=sl('dt')
  select rhs
  case 1 then //sl
   comments=' '
   [frq,repf,splitf]=repfreq(sl,'sym');sl=[] 
  case 2 then // sl,frq
   comments=' '
   [frq,repf,splitf]=repfreq(sl,fmin);fmin=[];sl=[]
  case 3 , //sl,frq,comments ou sl,fmin,fmax
   if type(fmax)==1 then
      comments=' '
      [frq,repf,splitf]=repfreq(sl,fmin,fmax,pas_def),sl=[]
   else
      comments=fmax
      [frq,repf,splitf]=repfreq(sl,fmin);fmin=[];sl=[]
   end
  case 4 ,
    if type(pas)==1 then 
      comments=' ',
    else 
      comments=pas;pas=pas_def
    end,
    [frq,repf,splitf]=repfreq(sl,fmin,fmax,pas)
  case 5 then,
    [frq,repf,splitf]=repfreq(sl,fmin,fmax,pas)
  else 
    error('invalid call: sys,fmin,fmax [,pas] [,com]')
  end;
case 1 then //frq,db,phi [,comments] ou frq, repf [,comments]
  select rhs
  case 2 , //frq,repf
    comments=' '
    repf=fmin;fmin=[]
  case 3 then
    if type(fmax)==1 then
      comments=' '//frq db phi
      repf=exp(log(10)*fmin/20 + %pi*%i/180*fmax);
      fmin=[]; fmax=[]  
    else
      repf=fmin;fmin=[]
      comments=fmax
     end;
   case 4 then 
     repf=exp(log(10)*fmin/20 + %pi*%i/180*fmax);
     comments=pas;
     fmin=[];fmax=[]
   else 
     error('invalid call: frq,db,phi,[com] or frq,repf,[com]')
   end;
   frq=sl;sl=[];[mn,n]=size(frq);
   splitf=1
   if mn<>1 then
      ilf=1;//un vecteur de frequences par reponse
   else
      ilf=0;//un seul vecteur de frequence
   end;
else 
   error('Nyquist: invalid call')
end;

[mn,n]=size(repf)
//
if comments==' ' then
  comments(mn)=' ';
  mnc=1
else
  mnc=mn+1
end
//
//trace d
repi=imag(repf);repf=real(repf)
//
mnx=mini(repf);
mny=mini(repi);
mxx=maxi(repf);
mxy=maxi(repi);
// computing bounds of graphic window
dx=(mxx-mnx)/30;dy=(mxy-mny)/30
rect=[mnx-dx,mny-dy,mxx+dx,mxy+dy]

// drawing the curves 
splitf($+1)=n+1;
for ksplit=1:size(splitf,'*')-1
  sel=splitf(ksplit):splitf(ksplit+1)-1
  if mnc==1 then
    plot2d(repf(:,sel)',repi(:,sel)',(1:mn),"051",' ',rect);
  else
    plot2d(repf(:,sel)',repi(:,sel)',(1:mn),"151",strcat(comments,'@'),rect);
  end
end
[r1,rect]=xgetech()
// drawing the grid 
alu=xget('alufunction');xset('alufunction',6)
xgrid(4);
xset('alufunction',alu)
// setting the current mark 
xgeti=xget("mark");
xset("mark",2,xgeti(2));
// clipping on (graphic box)
xset("clipgrf");

kk=1;p0=[repf(:,kk) repi(:,kk)];ks=1;d=0;
dx=rect(3)-rect(1)
dy=rect(4)-rect(2)
dx2=dx^2;dy2=dy^2

// collection significant frequencies along the curve 
//-------------------------------------------------------
Ic=mini(cumsum(sqrt(((repf(:,1:$-1)-repf(:,2:$)).^2)/dx2+((repi(:,1:$-1)-repi(:,2:$)).^2)/dy2),2),'r');
kk=1
L=0
DIc=0.2
while %t
  ksup=find(Ic-L>DIc)
  if ksup==[] then break,end
  kk1=mini(ksup)
  L=Ic(kk1)
  Ic(1:kk1)=[]
  kk=kk+kk1
  
  if mini(abs(frq(:,ks($))-frq(:,kk))./abs(frq(:,kk)))>0.001 then
    if mini(sqrt(((repf(:,ks)-repf(:,kk)*ones(ks)).^2)/dx2+..
        ((repi(:,ks)-repi(:,kk)*ones(ks)).^2)/dy2)) >DIc then
      ks=[ks kk]
      d=0
    end
  end
end  
if ks($)~=n then    
if mini(((repf(:,ks(1))-repf(:,n))^2)/dx2+((repi(:,ks(1))-repi(:,n))^2)/dy2)>0.01
  ks=[ks n]
end
end
// display of parametrization (frequencies along the curve)
//-------------------------------------------------------
kf=1
frmt=format();
mrksiz=0.015*(rect(3)-rect(1))

for k=1:mn,
  for kks=ks
    if abs(frq(kf,kks))>9999|abs(frq(kf,kks))<0.001 then
      format('e',6)
    else
      format('v',6)
    end
    xstring(repf(k,kks),repi(k,kks),string(frq(kf,kks)),0);
  end
  if size(ks,'*') >1 then
    if ks($)<size(repf,2) then
      last=$;
    else
      last=$-1;
    end
    dx=repf(k,ks(1:last)+1)-repf(k,ks(1:last));
    dy=repi(k,ks(1:last)+1)-repi(k,ks(1:last));
    dd=150*sqrt((dx/(rect(3)-rect(1))).^2+(dy/(rect(4)-rect(2))).^2);
    if dd>0 then
      dx=dx./dd;dy=dy./dd;

      xarrows([repf(k,ks(1:last));repf(k,ks(1:last))+dx],..
          [repi(k,ks(1:last));repi(k,ks(1:last))+dy],mrksiz)
    end
  end
//  xpoly(repf(k,ks),repi(k,ks),'marks',0);
  kf=kf+ilf
end;
vv=['v','e'];format(vv(frmt(1)),frmt(2))


xset("mark",xgeti(1),xgeti(2));
// axes with 0 
[w,rect]=xgetech()
xpoly([rect(1),rect(3)],[0,0],'lines');
xpoly([0,0],[rect(2),rect(4)],'lines');
//Optional unit circle 
//t=(0:0.1:2*%pi)';
//plot2d(sin(t),cos(t),[(mn+1) -mnc],'100','Unit Circle')

// clipping off 
xclip();

if sltyp=='c' then
  xtitle('Nyquist plot ','Re(h(2i*pi*f))','Im(h(2i*pi*f))');
elseif sltyp=='x' then 
    xtitle('Nyquist plot ','Re','Im');
else   
  xtitle('Nyquist plot ',['Re(h(exp(';'2i*pi*f*dt)))'],'Im(h(exp(2i*pi*f*dt)))');
end
endfunction
