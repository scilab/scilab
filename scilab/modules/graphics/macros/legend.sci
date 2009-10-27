// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) INRIA
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt

function c=legend(varargin)

  if size(varargin)<1 then
    error(msprintf(gettext("%s: Wrong number of input argument(s): At least %d expected.\n"), "legend", 1));
  end

options_names=["in_upper_right";"in_upper_left";"in_lower_left";
	       "in_lower_right";"by_coordinates";"out_upper_right";
	       "out_upper_left";"out_lower_left";"out_lower_right";
	       "upper_caption";"lower_caption"];
options_codes=[1;2;3;
	       4;5;-1;
	       -2;-3;-4;
	       -5;-6];
  f        = gcf();
  vis      = f.immediate_drawing;
  narg     = size(varargin);
  k0       = 1;
  H        = [];
  leg      = [];
  Acur     = gca();
  A        = Acur;
  opt      = 1;
  with_box = %T;


  while type(varargin(k0))==9 then //a handle that could be an Axes, Agreg. or Polyline handle.
    tmpH=varargin(k0)
    if tmpH.type=='Axes' then
      A = tmpH;
    end
    H=[H;getvalidchildren(tmpH)'];
    k0 = k0+1;
  end

  if H==[] then
    //walk subtree to get all proper children
    H=getvalidchildren(A)
  end

  //get all labels
  for k=k0:size(varargin)
    if type(varargin(k))<>10 then break,end
    vk=varargin(k)
    leg=[leg;vk(:)]
  end
  nleg=size(leg,'*')

  k0=k
  if k0<=narg&type(varargin(k0))==1 then opt=varargin(k0);k0=k0+1,end
  if k0<=narg&type(varargin(k0))==4 then with_box=varargin(k0);k0=k0+1,end

  //upper left coordinates
  if size(opt,'*')>1 then
    bnds=get(gca(),'axes_bounds');
    as=get(gcf(),'axes_size');
    [x1,y1,rect]=xchange(opt(1),opt(2),'f2i')
    x1=x1-as(1)*bnds(1)
    y1=y1-as(2)*bnds(2)
    pos=[x1/(as(1)*bnds(3)), y1/(as(2)*bnds(4))]
    opt=5
  elseif opt==5 then
    pos=[]
  elseif opt<-6|opt>4 then
    error('opt can take value in -6:4')
  end

  kopt=find(options_codes==opt)
  if kopt==[] then error('invalid positionning option'),end
  
  // the number of labels might be lower than the number of polylines
  nbLeg = min(size(H, '*'), size(leg, '*'));
  H = H(1:nbLeg);
  leg = leg(1:nbLeg);
  
  
  drawlater()
  c=captions(H,leg)
  if with_box then c.line_mode='on',else c.line_mode='off',end
  c.legend_location=options_names(kopt)
  if opt==5 then
    if pos<>[] then
      c.position=pos;
    else
      drawnow()
      bnds=get(gca(),'axes_bounds');
      as=get(gcf(),'axes_size');
      while %t
	rep=xgetmouse()
	if rep(3)<> -1 then break,end
	[x1,y1,rect]=xchange(rep(1),rep(2),'f2i')
	x1=x1-as(1)*bnds(1)
	y1=y1-as(2)*bnds(2)
	pos=[x1/(as(1)*bnds(3)), y1/(as(2)*bnds(4))]
	c.position=pos;
      end
    end
  end
  drawnow()
endfunction

function h=getvalidchildren(A)
  h=[]
  for k=1:size(A,'*')
    a=A(k)
    select a.type
    case "Polyline" then
      h=[h;a]
     case 'Axes'
      ax=a.children
      h=[h;getvalidchildren(ax($:-1:1))]
    case 'Compound'
      for k=size(a.children,'*'):-1:1
	h=[h;getvalidchildren(a.children(k))]

      end
    end
  end
endfunction
