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
// Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301, USA.
//
// See the file ../license.txt
//
function [x,y,typ]=VirtualCLK0(job,arg1,arg2)
// Copyright INRIA
x=[];y=[];typ=[]
select job
case 'plot' then
  orig=arg1.graphics.orig;sz=arg1.graphics.sz;orient=arg1.graphics.flip;
  pat=xget('pattern');xset('pattern',default_color(-1))
  thick=xget('thickness');xset('thickness',2)
  xx=[0:.01:1];
  yy=(1/4-(xx-1/2)^2)^(1/2)+1/2;
  x=(orig(1))*ones(1,101)+sz(1)*xx;
  y=(orig(2))*ones(1,101)+sz(2)*yy;
  xset('thickness',2);
  xpolys(x',y',5*ones(101,1));
  xx=[1:-.01:.01 0];
  yy=-(1/4-(xx-1/2)^2)^(1/2)+1/2;
  x=(orig(1))*ones(1,101)+sz(1)*xx;
  y=(orig(2))*ones(1,101)+sz(2)*yy;
  xpolys(x',y',5*ones(101,1));
  xset('thickness',1);
  xstringb(orig(1),orig(2),'CLK0',sz(1),sz(2),"fill")
  x=orig(1)*ones(1,2)+sz(1)*[1/2 1/2];
  y=(orig(2))*ones(1,2)+sz(2)*[1/2 15/16];
  xpolys(x',y',2*ones(2,1));
  x=(orig(1))*ones(1,2)+sz(1)*[1/2 1/2+(3*2^(1/2))/16];
  y=(orig(2))*ones(1,2)+sz(2)*[1/2 1/2+(3*2^(1/2))/16];
  xpolys(x',y',2*ones(2,1));
  xset('thickness',thick)
  xset('pattern',pat)
  xf=40
  yf=60
  nin=1;
  if ~orient then
    in=[-1/14   0
	0       1/7
	1/14    0
	-1/14   0]*diag([xf,yf])
    dy=sz(1)/(nin+1)
    xset('pattern',5)
    for k=1:nin
      xfpoly(in(:,1)+ones(4,1)*(orig(1)+sz(1)-dy*k),..
	  in(:,2)+ones(4,1)*(orig(2)-yf/7),5)
    end
  else
    in=[-1/14   0
	0       -1/7
	1/14    0
	-1/14   0]*diag([xf,yf])
    dy=sz(1)/(nin+1)
    xset('pattern',5)
    for k=1:nin
      xfpoly(in(:,1)+ones(4,1)*(orig(1)+sz(1)-dy*k),..
	  in(:,2)+ones(4,1)*(orig(2)+sz(2)+yf/7),5)
    end
  end
  
  x=[];y=[]
case 'getinputs' then
  orig=arg1.graphics.orig;
  sz=arg1.graphics.sz;
  if arg1.graphics.flip then
    x=orig(1)+sz(1)/2
    y=orig(2)+sz(2)
  else
    x=orig(1)+sz(1)/2
    y=orig(2)
  end
  typ=-ones(x) //undefined type
case 'getoutputs' then
  x=[];y=[];typ=[]
case 'getorigin' then
  [x,y]=standard_origin(arg1)
case 'set' then
  x=arg1;
case 'define' then
  model=scicos_model()
  model.sim='vrtclk0'
  model.evtin=1
  model.opar=list()
  model.ipar=[]
  model.blocktype='d'
  model.firing=-1
  model.dep_ut=[%f %f]

  exprs=[]
  x=standard_define([2 2],model,exprs,' ')
end
endfunction
