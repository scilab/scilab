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

function [x,y,typ]=BPLATFORM(job,arg1,arg2)
  x=[];y=[];typ=[]
  select job
   case 'plot' then
    standard_draw(arg1)
   case 'getinputs' then
    [x,y,typ]=standard_inputs(o)
   case 'getoutputs' then
    x=[];y=[];typ=[];
   case 'getorigin' then
    [x,y]=standard_origin(arg1)
   case 'set' then
    x=arg1;
    graphics=arg1.graphics;exprs=graphics.exprs
    model=arg1.model;dstate=model.dstate
    while %t do
      [ok,plen,csiz,phi,xmin,xmax,ymin,ymax,exprs]=scicos_getvalue(..
        'Set Scope parameters',..
        ['pendulum length';'cart size (square side)';'slope';
        'Xmin';'Xmax';  'Ymin'; 'Ymax'; ],..
        list('vec',1,'vec',1,'vec',1,'vec',1,'vec',1,'vec',1,'vec',1),exprs)
      if ~ok then break,end
      mess=[]
      if plen<=0|csiz<=0 then
        mess=[mess;'Pendulum lenght and cart size must be positive.';' ']
        ok=%f
      end
      if ymin>=ymax then
        mess=[mess;'Ymax must be greater than Ymin';' ']
        ok=%f
      end
      if xmin>=xmax then
        mess=[mess;'Xmax must be greater than Xmin';' ']
        ok=%f
      end
      if ~ok then
        message(mess)
      else
        rpar=[plen;csiz;phi;xmin;xmax;ymin;ymax]
        model.rpar=rpar;
        graphics.exprs=exprs;
        x.graphics=graphics;x.model=model
        break
      end
    end
   case 'define' then
    plen=2; csiz=2; phi=0;
    xmin=-5;xmax=5;ymin=0;ymax=15

    model=scicos_model()
    model.sim=list('bplatform2',5)
    model.in=[1;1]
    model.evtin=1
    model.dstate=0
    model.rpar=[plen;csiz;phi;xmin;xmax;ymin;ymax]
    model.blocktype='d'
    model.dep_ut=[%f %f]
    
    exprs=string(model.rpar)
    gr_i=['thick=xget(''thickness'');xset(''thickness'',2);Couleur=xget(''color'');';
	'xx=orig(1)+sz(1)*[3 7 7 3 3]/10;';
	'yy=orig(2)+sz(2)*[5 5 4 4 5 ]/10;';
	'xpoly(xx,yy,''lines'');';
	'xx=orig(1)+sz(1)*[5 4.5 5.5 4.5 5.5 4.5 5.5 4.5  5.5 4.5  5.5]/10;';
	'yy=orig(2)+sz(2)*[4 3.5 3.0 3.5 3.0 2.5 2.0 1.5  1.0 0.5 0]/10;';
	'xpoly(xx,yy,''lines'');';
	'xarc(orig(1)+sz(1)*0.4,orig(2)+sz(2)*0.8,sz(1)*0.2,sz(2)*0.2,0,360*64);';
	'xset(''thickness'',thick);']
    x=standard_define([2 2],model,exprs,gr_i)
  end
endfunction

