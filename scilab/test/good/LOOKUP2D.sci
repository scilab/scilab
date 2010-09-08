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
function [x,y,typ]=LOOKUP2D(job,arg1,arg2)
// Copyright INRIA
  
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
    
    x=arg1
    model=arg1.model
    graphics=arg1.graphics
    exprs=graphics.exprs
    ok=%f;
    SaveExit=%f
    while %t do

      Ask_again=%f
      [ok,xx,yy,zz,Method,graf,exprs]=scicos_getvalue('2D Lookup table parameters',['Row index input values';'Column index input values';'Table data';'Lookup method(1..5)';'Launch graphic window(y/n)?'],list('vec',-1,'vec',-1,'matrix',[-1,-1],'vec',1,'str',1),exprs)
      // 1 : Interpolation-extrapolation (Bilinear)
      // 2 : Interpolation_endvalues
      // 3 : use input nearest
      // 4 : use input below
      // 5 : use input above
      // 6 : Interpolation-extrapolation (linear)

      if  ~ok then break;end    
      mtd=int(Method); if mtd<1 then mtd=1;end; if mtd>6 then mtd=6;end;    
      if graf<>'y' & graf<>'Y' then  graf='n'; end
      exprs(5)='n';// exprs.graf='n'
      exprs(4)=sci2exp(mtd);// pour le cas methode>7 | method<0

      METHOD=getmethod(mtd);
      if ~Ask_again then 
	xx=xx(:);yy=yy(:);
	[nx,mx]=size(xx); [ny,my]=size(yy);[nz,mz]=size(zz);
	if ((nx<=1)|(ny<=1)) then, x_message('input row/column data size should be greater than one');  Ask_again=%t;end
	if ~((nx==nz)&(ny==mz)) then, x_message('incompatible size of x and y');  Ask_again=%t;end
	[ok]=test_increasing(xx);if (~ok) then  x_message('Row input values must be monotonically increasing');Ask_again=%t;end
	[ok]=test_increasing(yy);if (~ok) then  x_message('Column input values must be monotonically increasing');Ask_again=%t;end
      end
      if ~Ask_again then 
	if (graf=='Y' | graf=='y') then 
	  gh=gcf();
	  curwin=gh.figure_id;
	  save_curwin=curwin;
	  gh2=scf();
	  curwin=max(winsid())+1; 
	  plot3d(xx,yy,zz,35,45,'X@Y@Z',[5,2,4]) ;
	  curwin=save_curwin;
	  gh.figure_id=curwin;
	end
	
	model.rpar=[xx(:);yy(:);zz(:)]
	model.ipar=[nx;ny;mtd]
	graphics.exprs=exprs;
	x.model=model       
	x.graphics=graphics
	break
      end
    end 
    //======================================================================
   case 'define' then  
    model=scicos_model();
    xx=[1:4]
    yy=[1:3]
    zz=[4,5,6;16,19,20;10,18,23;6, 3,-1];
    Method=1; 
    Graf='n'
    
    Nx=length(xx);  
    Ny=length(yy);  
    model.sim=list('lookup2d',4)
    model.in=[1;1]  
    model.out=1 
    
    model.rpar=[xx(:);yy(:);zz(:)]
    model.ipar=[Nx;Ny;Method]
    model.blocktype='c'
    model.dep_ut=[%t %f]
    exprs=list(strcat(sci2exp(xx)),strcat(sci2exp(yy)),strcat(sci2exp(zz)),sci2exp(Method),Graf)
    gr_i=['txt=[''   Lookup'';''   table''];';
	  'xstringb(orig(1),orig(2),txt,sz(1),sz(2),''fill'');';
	  'txt=''r'';';'xstringb(orig(1)+.01*sz(1), orig(2)+.5*sz(1), txt, sz(1)/6,sz(2)/6,''fill'');'
	  'txt=''c'';';'xstringb(orig(1)+.01*sz(1), orig(2)+.1*sz(1), txt, sz(1)/6,sz(2)/6,''fill'');'

	 ]    
    
    x=standard_define([2.5 2],model,exprs,gr_i)
  end
endfunction

function [ok]=test_increasing(xx)
  ok=%f
  [nx,mx]=size(xx);// adjusting the x and y size

  for i=1:mx
    if (xx(i)<>xx(i)) then 
      xinfo('x contains no data in x('+string(i)+')'); 
      return;
    end
   
  end
  
   for i=1:mx-1
    if (xx(i)>xx(i+1)) then 
      return;
    end   
  end

  ok=%t
endfunction
//============================
function METHOD=getmethod(order)
  select order
   case 1 then, METHOD='Interpolation-extrapolation(biliniear)'
   case 2 then, METHOD='Interpolation_endvalues'
   case 3 then, METHOD='use input nearest'
   case 4 then, METHOD='use input below'
   case 5 then, METHOD='use input above'
   case 6 then, METHOD='Interpolation-extrapolation'
  end
endfunction
//=========================================================
