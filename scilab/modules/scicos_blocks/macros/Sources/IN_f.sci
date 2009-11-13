//  Scicos
//
//  Copyright (C) INRIA - METALAU Project <scicos@inria.fr>
//
// This program is free software; you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation; either version 2 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program; if not, write to the Free Software
// Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
//
// See the file ../license.txt
//

function [x,y,typ]=IN_f(job,arg1,arg2)
x=[];y=[];typ=[]
select job
case 'plot' then

  orig=arg1.graphics.orig;
  sz=arg1.graphics.sz;
  orient=arg1.graphics.flip;

  prt=arg1.model.ipar
  pat=xget('pattern');xset('pattern',default_color(1))
  thick=xget('thickness');xset('thickness',2)
  if orient then
    x=orig(1)+sz(1)*[-1/6;-1/6;1/1.5;1;1/1.5]
    y=orig(2)+sz(2)*[0;1;1; 1/2;0]
    xo=orig(1);yo=orig(2)
  else
    x=orig(1)+sz(1)*[0;1/3;7/6;7/6;1/3]
    y=orig(2)+sz(2)*[1/2;1;1;0;0]
    xo=orig(1)+sz(1)/3;yo=orig(2)
  end
  gr_i=arg1.graphics.gr_i;
  if type(gr_i)==15 then 
    coli=gr_i(2);pcoli=xget('pattern')
    xfpolys(x,y,coli);
    xset('pattern',coli)
    xstringb(xo,yo,string(prt),sz(1)/1.5,sz(2))
    xset('pattern',pcoli)
    xstringb(xo,yo,string(prt),sz(1)/1.5,sz(2))
  else
    xstringb(xo,yo,string(prt),sz(1)/1.5,sz(2))
    xpoly(x,y,'lines',1)
  end
  xset('thickness',thick)
  xset('pattern',pat)
  
  ident = arg1.graphics.id

  if ident <> [] then
    font=xget('font')
    xset('font', options.ID(1)(1), options.ID(1)(2))
    rectangle = xstringl(orig(1)+3/2*sz(1), orig(2), ident)
    w = rectangle(3)
    h = rectangle(4)
    xstringb(orig(1) - sz(1) /2 - w, orig(2) + sz(2) * 0.5 , ident , w, h)
    xset('font', font(1), font(2))
  end
  x=[];y=[]
case 'getinputs' then
  x=[];y=[];typ=[]
  case 'getoutputs' then
  orig=arg1.graphics.orig;
  sz=arg1.graphics.sz;
  orient=arg1.graphics.flip;

  if orient then
    x=orig(1)+sz(1)
    y=orig(2)+sz(2)/2
  else
    x=orig(1)
    y=orig(2)+sz(2)/2
  end
  typ=ones(x)
case 'getorigin' then
  [x,y]=standard_origin(arg1)
case 'set' then
  x=arg1;
  graphics=arg1.graphics;
  model=arg1.model;
  exprs=graphics.exprs;
  
  if size(exprs,'*')==2 then exprs=exprs(1),end //compatibility
  while %t do
    [ok,prt,exprs]=scicos_getvalue('Set Input block parameters',..
	'Port number',list('vec',1),exprs)
    if ~ok then break,end
    prt=int(prt)
    if prt<=0 then
      message('Port number must be a positive integer')
    else
      if model.ipar<>prt then needcompile=4;y=needcompile,end
      model.ipar=prt
      model.firing=[];model.out=-1//compatibility
      graphics.exprs=exprs
      x.graphics=graphics
      x.model=model
      break
    end
  end
case 'define' then
  in=-1
  prt=1
  model=scicos_model()
  model.sim='input'
  model.out=-1
  model.out2=-2
  model.outtyp=-1
  model.ipar=prt
  model.blocktype='c'
  model.dep_ut=[%f %f]

  exprs=sci2exp(prt)
  gr_i=' '
  x=standard_define([1 1],model,exprs,gr_i)
end
endfunction
