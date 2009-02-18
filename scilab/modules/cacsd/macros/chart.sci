// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) INRIA Serge Steer
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at    
// http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt

function chart(attenu,angl,flags)
  
  titre=_("Amplitude and phase contours of y/(1+y)")
  l10=log(10);
  ratio=%pi/180;
  defaultAngles = [1:10,20:10:160];
  //
  [lhs,rhs]=argn(0)
  
  select rhs
    
  case 3 then
    
  case 2 then,
    if type(angl)==15 then
	  // angl actually stands for flags
      flags=angl
      angl = defaultAngles;
    else
      // angl is the phase
      flags=[]
    end
    
  case 1 then
    if type(attenu)==15 then
      flags=attenu
      attenu=[-12 -8 -6 -5 -4 -3 -2 -1.4 -1 -.5 ,..
	      0.25 0.5 0.7 1 1.4 2 2.3 3 4 5 6 8 12];
    else
      flags=list()
    end
    angl = defaultAngles;
  else
    flags=list()
    attenu=[-12 -8 -6 -5 -4 -3 -2 -1.4 -1 -.5 ,..
	    0.25 0.5 0.7 1 1.4 2 2.3 3 4 5 6 8 12];
    angl = defaultAngles;
  end
  
  // convert angles to radian
  angl = -angl * ratio;
  
  c1=color('lightgrey');c2=c1
  select size(flags)
  case 0 then
    flags=list(0,-1,c1,c2)
  case 1 then
    flags=list(flags(1),-1,c1,c2)
  case 2 then
    flags=list(flags(1),flags(2),c1,c2)
  case 3 then
    flags(4)=c2
  end
  //
 
 
  if ~flags(1) then 
    clf()
    ax=gca();
    ax.data_bounds=[-360,-50;0,40];
    ax.axes_visible='on';
    ax.box='on';
    ax.tight_limits='on'
  else
    ax=gca();
  end
  ax.clip_state="clipgrf"
//  ax.data_bounds=[0,-50;360,40];

  phi_min=ax.data_bounds(1,1)
  phi_max=ax.data_bounds(2,1)
  k1=floor(phi_min/180)
  k2=ceil(phi_max/180)
  //
  drawlater()
  if flags(2) then xtitle(titre,_("phase(y) (degree)"),_("magnitude(y) (Db)")),end
  llrect=xstringl(0,0,'1')
  
  //isogain curves
  lambda=exp(l10*attenu/20)
  rayon=lambda./(lambda.*lambda-ones(lambda))
  centre=-lambda.*rayon
   //
   if attenu<>[]
     for i = 1:prod(size(attenu)),
       att=attenu(i);
       if att<0 then 
	 w=%eps:0.03:%pi;
       else 
	 w=-%pi:0.03:0;
       end;
       n=prod(size(w))
       rf=centre(i)*ones(w)+rayon(i)*exp(%i*w);
       phi=atan(imag(rf),real(rf))/ratio; //phi is in [-180 0]
       module=20*log(abs(rf))/l10;
       
       //use symetry and period to extend the curve on [k1*180 k2*180]
       p=[];m=[];
       S=[]
       for k=k1:k2-1
	 if pmodulo(k,2)==0 then
	   p=[p %nan k*180-phi($:-1:1)]
	   m=[m %nan module($:-1:1)]
	   if att>0 then 
	     xstring(p($),m($),string(att),0,0),
	     e=gce();
	     if ~flags(1) then e.clip_state='off';end
	     S=[e S]
	   end
	 else
	   p=[p %nan ((k+1)*180)+phi]
	   m=[m %nan module]
	   if att<0 then 
	     xstring(p($),m($),string(att),0,0),
	     e=gce();
	     if ~flags(1) then e.clip_state='off';end
	     S=[e S]
	   end
	 end
	 
       end
       xpoly(p,m)
       e=gce();e.foreground=flags(3);//e.line_style=3,
       if size(S,'*')>1 then S=glue(S),end
       S=glue([S,e]);S.user_data=att;
     end;
     glue(ax.children(size(attenu,'*'):-1:1))
   end
  //isophase curves
  if angl<>[] then
  eps=100*%eps;
  for teta=angl,
    if teta < -%pi/2 then
      last=teta-eps,
    else
      last=teta+eps,
    end;
    w=[-170*ratio:0.03:last last]
    n=prod(size(w));
    module=real(20*log((sin(w)*cos(teta)/sin(teta)-cos(w)))/l10)
    w=w/ratio
    p=[];m=[];
    for k=k1:k2-1
      if pmodulo(k,2)==0 then
	p=[p %nan k*180-w($:-1:1)]
	m=[m %nan module($:-1:1)]
      else
	p=[p %nan ((k+1)*180)+w]
	m=[m %nan module]
      end
    end
    xpoly(p,m)
    e=gce();e.foreground=flags(3);//e.line_style=3,
    e.user_data=teta
  end;
  glue(ax.children(size(angl,'*'):-1:1))
  end
  drawnow() ;
endfunction
