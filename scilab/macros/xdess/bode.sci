function []=bode(sl,fmin,fmax,pas,comments)
//!
// Copyright INRIA
[lhs,rhs]=argn(0);
dom='c';
//---------------------
nyq_frq=[];l10=log(10);
pas_def='auto' // default
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
  if typ=='lss' then
    if sl(7)==[] then error('Undefined time domain (sl(7))');end
  end
  if typ=='r' then
    if sl(4)==[] then error('Undefined time domain (sl(4))');end
  end
  dom=sl('dt')
  if dom==[]|dom==0 then error(96,1),end
  if dom=='d' then dom=1;end
  
  select rhs
  case 1 then //sl
   comments=' '
   fmin_default=1.d-3;
   fmax_default=1.d3;

   if dom=='c' then fmax_default=1.d3; else fmax_default=1/(2*dom),end
   [frq,repf]=repfreq(sl,fmin_default,fmax_default);
   [d,phi]=dbphi(repf);
   sl=[] 
  case 2 then // sl,frq
   comments=' '
   if min(fmin)<=0 then
     error('bode: requires strictly positive frequency vector')
   end
   [frq,repf]=repfreq(sl,fmin);
   [d,phi]=dbphi(repf);
   fmin=[];sl=[]
  case 3 , //sl,frq,comments ou sl,fmin,fmax
   if type(fmax)==1 then
      comments=' '
      if fmin<=0 then
         error('bode: requires strictly positive frequency range')
       end
      [frq,repf]=repfreq(sl,fmin,fmax,pas_def),sl=[]
      [d,phi]=dbphi(repf);
   else
      comments=fmax
      if min(fmin)<=0 then
	error('bode: requires strictly positive frequency vector')
      end      
      if type(dom)==1 then nyq_frq=1/2/dom;end
      if find(fmin>nyq_frq)~=[] then 
	warning('There are frequencies beyond Nyquist f!');
      end
      [frq,repf]=repfreq(sl,fmin);fmin=[];sl=[]
      [d,phi]=dbphi(repf);
   end
  case 4 ,
    if type(pas)==1 then 
      comments=' ',
    else 
      comments=pas;pas=pas_def;
    end,
    if min(fmin)<=0 then
      error('bode: requires strictly positive frequency vector')
    end
    [frq,repf]=repfreq(sl,fmin,fmax,pas)
    [d,phi]=dbphi(repf);
  case 5 then,
    if min(fmin)<=0 then
      error('bode: requires strictly positive frequency vector')
    end
    [frq,repf]=repfreq(sl,fmin,fmax,pas)
    [d,phi]=dbphi(repf);
  else 
    error('Invalid call: sys,fmin,fmax [,pas] [,com]')
  end;

case 1 then //frq,db,phi [,comments] ou frq, repf [,comments]
  select rhs
  case 2 , //frq,repf
    comments=' '
    [phi,d]=phasemag(fmin);fmin=[]
  case 3 then
    if type(fmax)==1 then
      comments=' '//frq db phi
      d=fmin,fmin=[]
      phi=fmax,fmax=[]
    else
      [phi,d]=phasemag(fmin);fmin=[]
      comments=fmax
     end;
   case 4 then 
     comments=pas;d=fmin;fmin=[];phi=fmax;fmax=[]
   else 
     error('Invalid call: frq,db,phi,[com] or frq,repf,[com]')
   end;
   frq=sl;sl=[];[mn,n]=size(frq);
   if min(frq)<=0 then
     error('bode: requires strictly positive frequencies')
   end
   if mn<>1 then
      ilf=1;//un vecteur de frequences par reponse
   else
      ilf=0;//un seul vecteur de frequence
   end;
else 
   error('Bode: invalid call')
end;
[mn,n]=size(phi)
//
//Captions
if comments==' ' then
   comments(mn)=' ';
   mnc=0
   hx=0.48
   else
   mnc=mn
   hx=0.43
end;

[wrect,frect]=xgetech();
//magnitude
xsetech(wrect=[wrect(1)+0,wrect(2)+0,wrect(3)*1.0,wrect(4)*hx*0.95]);
rect=[mini(frq),mini(d),maxi(frq),maxi(d)]
// just to fix the scales for xgrid
plot2d1("oln",mini(frq),mini(d),0,"051"," ",rect);
// xgrid first 
xgrid(4);
// now the curves 
plot2d1("oln",frq',d',[1:mn],"000"," ",rect);
if type(dom)==1 then
  [xx1,xx2]=xgetech();
  val= xx2([2;4])';
  plot2d1("oln",max(frq)*[1;1],val,5,"000"," ",rect);
end
xtitle('Magnitude ',' Hz','db');

//phase
xsetech(wrect=[wrect(1)+0,wrect(2)+wrect(4)*hx,wrect(3)*1.0,wrect(4)*hx*0.95]);
rect=[mini(frq),mini(phi),maxi(frq),maxi(phi)]
// just to fix the scales for xgrid
plot2d1("oln",mini(frq),mini(phi),0,"051"," ",rect);
xgrid(4);
//  now the curves
plot2d1("oln",frq',phi',[1:mn],"000");
if type(dom)==1 then
  [xx1,xx2]=xgetech();
  val= xx2([2;4])';
  plot2d1("oln",max(frq)*[1;1],val,5,"000");
end
xtitle('Phase ',' Hz','degrees');
if mnc>0 then
  xsetech([wrect(1)+0,wrect(2)+wrect(4)*2*hx,wrect(3)*1.0,wrect(4)*0.1],[0 0 1 1]);
  dash=xget('dashes')
  y0=0.7;dy=-1/2
  x0=0;dx=1/2
  count=0
  for k=1:mnc
    xset('dashes',k)
    xsegs([x0;x0+0.08],[y0;y0])
    rect=xstringl(x0+0.1,y0,comments(k))
    xset('dashes',dash(1));
    xstring(x0+0.1,y0-rect(4)/3,comments(k))
    count=count+1
    y0=y0+dy
    if count==3 then x0=x0+dx;y0=0.7,end
  end
  xset('dashes',dash(1))
end
xsetech(wrect,frect);
endfunction
