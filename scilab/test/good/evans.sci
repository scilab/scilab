// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) INRIA
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at    
// http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt


function evans(n,d,kmax)
// Seuil maxi et mini (relatifs) de discretisation en espace
// Copyright INRIA
  
  smax=0.002;smin=smax/3;
  nptmax=2000 //nbre maxi de pt de discretisation en k
  
  //analyse de la liste d'appel
  
  [lhs,rhs]=argn(0)
  
  if rhs <= 0 then   // demo
    n=real(poly([0.1-%i 0.1+%i,-10],'s'));
    d=real(poly([-1 -2 -%i %i],'s'));
    evans(n,d,80);
    return
  end

  select typeof(n)
  case 'constant'  then
    if rhs==2 then kmax=0,end
  case 'polynomial'  then
    if rhs==2 then kmax=0,end
    //-compat next case retained for list/tlist compatibility
  case 'rational' then
    if rhs==2 then kmax=d,else kmax=0,end
    [n,d]=n(2:3)
  case 'state-space' then
    if rhs==2 then kmax=d,else kmax=0,end
    n=ss2tf(n);
    [n,d]=n(2:3);n=clean(n);d=clean(d);
  else 
     error(msprintf(gettext("%s: Wrong type for input argument #%d: A linear dynamical system or a polynomial expected.\n"),"evans",1));
  end
  if prod(size(n))<>1 then
    error(msprintf(gettext("%s: Wrong value for input argument #%d: Single input, single output system expected.\n"),"evans",1));
  end
  if kmax<=0 then
    nm=min([degree(n),degree(d)])
    fact=norm(coeff(d),2)/norm(coeff(n),2)
    kmax=round(500*fact),
  end
  //
  //calcul de la discretisation en k et racines associees
  nroots=roots(n);racines=roots(d);
  if nroots==[] then
    nrm=max([norm(racines,1),norm(roots(d+kmax*n),1)])
  else
    nrm=max([norm(racines,1),norm(nroots,1),norm(roots(d+kmax*n),1)])
  end
  md=degree(d)
  //
  ord=1:md;kk=0;nr=1;k=0;pas=0.99;fin='no';


  while fin=='no' then
    k=k+pas
    r=roots(d+k*n);r=r(ord)
    dist=max(abs(racines(:,nr)-r))/nrm
    //
    point='nok'
    if dist <smax then //pas correct
      point='ok'
    else //pas trop grand ou ordre incorrect
	 // on cherche l'ordre qui minimise la distance
	 ix=1:md
	 ord1=[]
	 for ky=1:md
	   yy=r(ky)
	   mn=10*dist*nrm
	   for kx=1:md
	     if ix(kx)>0 then
	       if  abs(yy-racines(kx,nr)) < mn then
		 mn=abs(yy-racines(kx,nr))
		 kmn=kx
	       end
	     end
	   end
	   ix(kmn)=0
	   ord1=[ord1 kmn]
	 end
	 r(ord1)=r
	 dist=max(abs(racines(:,nr)-r))/nrm
	 if dist <smax then
	   point='ok',
	   ord(ord1)=ord
	 else
	   k=k-pas,pas=pas/2.5
	 end
    end
    if dist<smin then
      //pas trop petit
      pas=2*pas;
    end
    if point=='ok' then
      racines=[racines,r];kk=[kk,k];nr=nr+1
      if k>kmax then fin='kmax',end
      if nr>nptmax then fin='nptmax',end
    end
  end
  //draw the axis
  x1 =[nroots;matrix(racines,md*nr,1)];
  xmin=min(real(x1));xmax=max(real(x1))
  ymin=min(imag(x1));ymax=max(imag(x1))
  dx=abs(xmax-xmin)*0.05
  dy=abs(ymax-ymin)*0.05
  if dx<1d-10, dx=0.01,end
  if dy<1d-10, dy=0.01,end
  legs=[],lstyle=[];lhandle=[]
  rect=[xmin-dx;ymin-dy;xmax+dx;ymax+dy];
  fig=gcf();
  immediate_drawing = fig.immediate_drawing;
  fig.immediate_drawing = 'off';
  a=gca()
  a.data_bounds=[rect(1) rect(2);rect(3) rect(4)]
  if nroots<>[] then 
    xpoly(real(nroots),imag(nroots))
    e=gce();e.line_mode='off';e.mark_mode='on';
    e.mark_size_unit="point";e.mark_size=7;e.mark_style=5;
    legs=[legs; _("open loop zeroes")]
    lhandle=[lhandle; e];
  end
  if racines<>[] then 
    xpoly(real(racines(:,1)),imag(racines(:,1)))
    e=gce();e.line_mode='off';e.mark_mode='on';
    e.mark_size_unit="point";e.mark_size=7;e.mark_style=2;
    legs=[legs;_("open loop poles")]
    lhandle=[lhandle; e];
  end

  dx=max(abs(xmax-xmin),abs(ymax-ymin));
  //plot the zeros locations


  //computes and draw the asymptotic lines
  m=degree(n);q=md-m
  if q>0 then
    la=0:q-1;
    so=(sum(racines(:,1))-sum(nroots))/q
    i1=real(so);i2=imag(so);
    if prod(size(la))<>1 then
      ang1=%pi/q*(ones(la)+2*la)
      x1=dx*cos(ang1),y1=dx*sin(ang1)
    else
      x1=0,y1=0,
    end
    if md==2,
      if coeff(d,md)<0 then
	x1=0*ones(2),y1=0*ones(2)
      end,
    end;
    if max(k)>0 then
      xpoly(i1,i2);
      e=gce();
      legs=[legs;_("asymptotic directions")]
      lhandle=[lhandle; e];
      axes = gca();
      axes.clip_state = "clipgrf";
      for i=1:q,xsegs([i1,x1(i)+i1],[i2,y1(i)+i2]),end,
      
      axes.clip_state = "off";
    end
  end;

  //lieu de evans
  [n1,n2]=size(racines);

  plot2d(real(racines)',imag(racines)',style=2+(1:n2));
  legend(lhandle,legs);
  xtitle(_("Evans root locus"),_("Real axis"),_("Imaginary axis"));
  fig.immediate_drawing = immediate_drawing;

  if fin=='nptmax' then
    warning(msprintf(gettext("%s: Curve truncated to the first %d discretization points.\n"),"evans",nptmax))
  end
endfunction
