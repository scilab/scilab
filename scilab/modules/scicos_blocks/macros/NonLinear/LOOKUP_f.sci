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

function [x,y,typ]=LOOKUP_f(job,arg1,arg2)
x=[];y=[];typ=[];
select job
case 'plot' then
  standard_draw(arg1)
case 'getinputs' then
  [x,y,typ]=standard_inputs(arg1)
case 'getoutputs' then
  [x,y,typ]=standard_outputs(arg1)
case 'getorigin' then
  [x,y]=standard_origin(arg1)
case 'set' then
  x=arg1;
  graphics=arg1.graphics;exprs=graphics.exprs
  model=arg1.model;rpar=model.rpar;
  n=size(rpar,'*')/2
  xx=rpar(1:n);yy=rpar(n+1:2*n)
  while %t do
    [ln,fun]=where();  

    if ~or(fun == "do_eval") then // cas standard  
        [xx,yy,ok,gc] = edit_curv(xx,yy,'axy');
    else
      ok=%t
    end  // no need anymore to overload edit_curv in do_eval

    if ~ok then break,end
    n=size(xx,'*')
    if or(xx(2:n)-xx(1:n-1)<=0) then
      message('You have not defined a function')
      ok=%f
    end
    if ok then
      model.rpar=[xx(:);yy(:)]
      x.graphics=graphics;x.model=model
      break
    end
  end
case 'define' then
  model=scicos_model()
  model.sim='lookup'
  model.in=1
  model.out=1
  model.rpar=[-2;-1;1;2;-1;1;-1;1]
  model.blocktype='c'
  model.dep_ut=[%t %f]

  gr_i=['rpar=model.rpar;n=size(rpar,''*'')/2;';
    'thick=xget(''thickness'');xset(''thickness'',2);';
    'xx=rpar(1:n);yy=rpar(n+1:2*n);';
    'mnx=mini(xx);xx=xx-mnx*ones(xx);mxx=maxi(xx);';
    'xx=orig(1)+sz(1)*(1/10+(4/5)*xx/mxx);';
    'mnx=mini(yy);yy=yy-mnx*ones(yy);mxx=maxi(yy);';
    'yy=orig(2)+sz(2)*(1/10+(4/5)*yy/mxx);';
    'xpoly(xx,yy,''lines'');';
    'xset(''thickness'',thick);']
  x=standard_define([2 2],model,[],gr_i)
end
endfunction
