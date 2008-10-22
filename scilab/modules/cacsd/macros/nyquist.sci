// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) INRIA, Serge Steer
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at    
// http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt

function nyquist(varargin)
// Nyquist plot
//!
  rhs=size(varargin)
  if type(varargin($))==10 then
    comments=varargin($),rhs=rhs-1;
  else
    comments=[];
  end
  fname="nyquist";//for error messages
  fmax=[];  
  if or(typeof(varargin(1))==['state-space' 'rational']) then 
    //sys,fmin,fmax [,pas] or sys,frq
    refdim=1 //for error message
    sltyp=varargin(1).dt
    if rhs==1 then
      [frq,repf,splitf]=repfreq(varargin(1),1d-3,1d3)
    elseif rhs==2 then //sys,frq
      if size(varargin(2),2)<2 then
	error(msprintf(_("%s: Wrong size for input argument #%d: A row vector with length>%d expected.\n"),..
		       fname,2,1))
      end
      [frq,repf]=repfreq(varargin(1:rhs))
    elseif or(rhs==(3:4)) then //sys,fmin,fmax [,pas]
      [frq,repf,splitf]=repfreq(varargin(1:rhs))
    else
      error(msprintf(_("%s: Wrong number of input arguments: %d to %d expected.\n"),fname,1,5))
    end
  elseif  type(varargin(1))==1 then 
    //frq,db,phi [,comments] or frq, repf [,comments]
    refdim=2
    sltyp='x';splitf=[];splitf=1
    select rhs
    case 2 then //frq,repf
      frq=varargin(1);
      repf=varargin(2)
      if size(frq,2)<2 then
	error(msprintf(_("%s: Wrong size for input argument #%d: A row vector with length>%d expected.\n"),..
		       fname,1,1))
      end
      if size(frq,2)<>size(varargin(2),2) then
	error(msprintf(_("%s: Incompatible input arguments #%d and #%d: Same column dimensions expected.\n"),..
			 fname,1,2))
      end
 
    case 3 then  //frq,db,phi
      frq=varargin(1);
      if size(frq,2)<>size(varargin(2),2) then
	error(msprintf(_("%s: Incompatible input arguments #%d and #%d: Same column dimensions expected.\n"),..
			 fname,1,2))
      end
      if size(frq,2)<>size(varargin(3),2) then
	error(msprintf(_("%s: Incompatible input arguments #%d and #%d: Same column dimensions expected.\n"),..
			 fname,1,3))
      end
      repf=exp(log(10)*varargin(2)/20 + %pi*%i/180*varargin(3));
      
    else 
       error(msprintf(_("%s: Wrong number of input arguments: %d to %d expected.\n"),fname,2,4))
    end
  else
    error(msprintf(_("%s: Wrong type for input argument #%d: Linear dynamical system or row vector of floats expected.\n"),fname,1))
  end;
  if size(frq,1)==1 then
    ilf=0
  else
    ilf=1
  end
  
  [mn,n]=size(repf)
  if and(size(comments,'*')<>[0 mn]) then
    error(msprintf(_("%s: Incompatible input arguments #%d and #%d: Same number of elements expected.\n"),...
		   fname,refdim,rhs+1))
  end
  //

  repi=imag(repf);repf=real(repf)

  // computing bounds of graphic window
  mnx=mini(repf);
  mny=mini(repi);
  mxx=maxi(repf);
  mxy=maxi(repi);

  dx=(mxx-mnx)/30;dy=(mxy-mny)/30
  rect=[mnx-dx,mny-dy;mxx+dx,mxy+dy]

  drawlater()
  ax=gca();
  ax.data_bounds=rect
  ax.axes_visible='on';
  ax.grid=color('lightgrey')*ones(1,3)
  // drawing the curves 
  splitf($+1)=n+1;
  
  ksplit=1;sel=splitf(ksplit):splitf(ksplit+1)-1;
  R=[repf(:,sel)];  I=[repi(:,sel)];
  for ksplit=2:size(splitf,'*')-1
    sel=splitf(ksplit):splitf(ksplit+1)-1
    R=[R %nan(ones(mn,1)) repf(:,sel)];
    I=[I %nan(ones(mn,1)) repi(:,sel)];
  end
  Curves=[]
  for k=1:mn
     xpoly(R(k,:),I(k,:)) 
     e=gce();e.foreground=k;
     Curves=[Curves e];
  end
  clear R I

  kk=1;p0=[repf(:,kk) repi(:,kk)];ks=1;d=0;
  dx=rect(2,1)-rect(1,1)
  dy=rect(2,2)-rect(1,2)
  dx2=dx^2;dy2=dy^2

  // collect significant frequencies along the curve 
  //-------------------------------------------------------
  Ic=min(cumsum(sqrt((diff(repf,1,'c').^2)/dx2+ (diff(repi,1,'c').^2)/dy2),2),'r');
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
  frmt=format();//preserve current format
  if ks($)<size(repf,2) then last=$;else last=$-1;end
  for k=1:mn,
    L=[]
    for kks=ks
      if abs(frq(kf,kks))>9999|abs(frq(kf,kks))<0.001 then
	format('e',6)
      else
	format('v',6)
      end
      xstring(repf(k,kks),repi(k,kks),string(frq(kf,kks)),0);
      L=[L gce()]
    end
    A=[]
    
    if size(ks,'*')>1 then
      dr=repf(k,ks(1:last)+1)-repf(k,ks(1:last));
      di=repi(k,ks(1:last)+1)-repi(k,ks(1:last));
      dd=150*sqrt((dr/dx).^2+(di/dy).^2);
      if dd>0 then
        // we should use xarrows or xsegs here.
        // However their displayed arrow size depends
        // on the data bounds and we want to avoid this
        xpolys([repf(k,ks(1:last));repf(k,ks(1:last))+dr./dd],..
		[repi(k,ks(1:last));repi(k,ks(1:last))+di./dd])
	//xarrows([repf(k,ks(1:last));repf(k,ks(1:last))+dr./dd],..
	//	[repi(k,ks(1:last));repi(k,ks(1:last))+di./dd],1.5)
	A=gce();
        A.children.arrow_size_factor = 1.5;
        A.children.polyline_style = 4;
      end
    end
    kf=kf+ilf
    glue([Curves(k) glue([L A])])

  end;

  if comments<>[] then
    legend(Curves,comments)
  end

  if sltyp=='c' then
    xtitle(_("Nyquist plot"),'Re(h(2i*pi*f))','Im(h(2i*pi*f))');
  elseif sltyp=='x' then 
    xtitle(_("Nyquist plot"),'Re','Im');
  else   
    xtitle(_("Nyquist plot"),'Re(h(exp(2i*pi*f*dt)))','Im(h(exp(2i*pi*f*dt)))');
  end
  drawnow()
    //reset format   
  vv=['v','e'];format(vv(frmt(1)),frmt(2))
endfunction
