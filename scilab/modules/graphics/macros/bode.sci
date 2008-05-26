// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) INRIA
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at    
// http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt


function []=bode(sl,fmin,fmax,pas,comments)
//!
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


drawlater()
sciCurAxes=gca();
axes=sciCurAxes;
wrect=axes.axes_bounds;


//magnitude
axes.axes_bounds=[wrect(1)+0,wrect(2)+0,wrect(3)*1.0,wrect(4)*hx*0.95]
rect=[mini(frq),mini(d);maxi(frq),maxi(d)]
plot2d(frq',d') ;
if type(dom)==1 then
  plot2d(max(frq)*[1;1],axes.data_bounds(:,2));
end
axes.data_bounds = rect; axes.log_flags = "lnn" ;
axes.grid=[4 4];
xtitle('Magnitude ',' Hz','db');

//phase
axes=newaxes();
axes.axes_bounds=[wrect(1)+0,wrect(2)+wrect(4)*hx,wrect(3)*1.0,wrect(4)*hx*0.95];
rect=[mini(frq),mini(phi);maxi(frq),maxi(phi)]
plot2d(frq',phi' ) ;
if type(dom)==1 then
  plot2d(max(frq)*[1;1],axes.data_bounds(:,2));
end
axes.data_bounds = rect ;a.log_flags = "lnn" ;
axes.grid=[4 4];
xtitle('Phase ',' Hz','degrees');

// create legend
if mnc>0 then
  axes=newaxes()
  axes.axes_bounds=[wrect(1)+0,wrect(2)+wrect(4)*2*hx,wrect(3)*1.0,wrect(4)*0.1];
  axes.data_bounds=[0 0; 1 1];
  y0=0.7;dy=-1/2
  x0=0;dx=1/2
  count=0
  for k=1:mnc
    xsegs([x0;x0+0.08],[y0;y0],k)
    rect=xstringl(x0+0.1,y0,comments(k))
    xstring(x0+0.1,y0-rect(4)/3,comments(k))
    count=count+1
    y0=y0+dy
    if count==3 then x0=x0+dx;y0=0.7,end
  end
end
drawnow()
// return to the previous scale
set( "current_axes", sciCurAxes ) ;

endfunction
