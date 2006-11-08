function []=gainplot(sl,fmin,fmax,pas,comments)
//!
// Copyright INRIA
[lhs,rhs]=argn(0);
dom='c';
//---------------------
pas_def='auto' // default
//
noxtitle=%f;
ilf=0
flag=type(sl);
if flag==15 then flag=16;end
select flag
case 16 then  // sl,fmin,fmax [,pas] [,comments]
  typ=sl(1);typ=typ(1);
  if typ<>'lss'&typ<>'r' then
    error(97,1)
  end
  sl1=sl(1);
  if sl1(1)=='r' then dom=sl(4),else dom=sl(7),end
  if dom==[] then error(96,1),end
  if dom=='d' then dom=1;end
  select rhs
  case 1 then //sl
   comments=' '
   fmin_default=1.d-3;
   fmax_default=1.d3;

   if dom=='c' then fmax_default=1.d3; else fmax_default=1/(2*dom),end
   [frq,repf]=repfreq(sl,fmin_default,fmax_default);sl=[] 
   [d,phi]=dbphi(repf);
  case 2 then // sl,frq
   comments=' '
   if min(fmin)<=0 then
     error('bode: requires strictly positive frequency vector')
   end   
   [frq,repf]=repfreq(sl,fmin);fmin=[];sl=[]
   [d,phi]=dbphi(repf);
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
      comments=pas;pas=pas_def
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
noxtitle=%t;
  select rhs
  case 2 , //frq,repf
    comments=' '
    [phi,d]=phasemag(fmin),fmin=[]
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
     error('inputs:frq,db,phi,[com] or frq,repf,[com]')
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
   error('gainplot: invalid plot')
end;
[mn,n]=size(phi)
//
//Captions
if comments==' ' then
   comments(mn)=' ';
   mnc=0
  strf='051'
else
   mnc=mn
  strf='151'
end;

rect=[mini(frq),mini(d),maxi(frq),maxi(d)]
isNewStyle = ( get("figure_style") == "new") ;

if ~isNewStyle then
  plot2d1("oln",mini(frq),mini(d),0,'051',' ',rect);
end


xgrid(4)
if ilf==0 then
  plot2d1("oln",frq',d',[1,3:mn+1],strf,strcat(comments,'@'),rect);
else
  plot2d1("gln",frq',d',[1,3:mn+1],strf,strcat(comments,'@'),rect);
end

if isNewStyle then
  axes = gca() ;
  rect=[mini(frq),mini(d);maxi(frq),maxi(d)]
  axes.data_bounds = rect ;
  a.log_flags = "lnn" ;
end

if ~noxtitle then
xtitle(' ','Hz','db');
end
endfunction
