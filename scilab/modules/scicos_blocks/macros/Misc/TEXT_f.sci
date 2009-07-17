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

function [x,y,typ]=TEXT_f(job,arg1,arg2)
// Copyright INRIA
//** 22-23 Aug 2006: some carefull adjustements for the fonts
//**                 inside the new graphics datastructure  
x=[]; y=[]; typ=[];

select job

case 'plot' then //normal  position
  graphics = arg1.graphics; 
  model    = arg1.model;
  
  if model.rpar==[] then
      model.rpar=graphics.exprs(1)
  end //compatibility

  if size(model.ipar,'*')==2 then
      model.ipar=[model.ipar; -1]
  end //compatibility
  
 
  
  //@@ compute bbox
  rect = stringbox(model.rpar, graphics.orig(1), graphics.orig(2),0,model.ipar(1),model.ipar(2));

  w=(rect(1,3)-rect(1,2)) * %zoom;
  h=(rect(2,2)-rect(2,4)) * %zoom/1.4 * 1.2;
  
  //@@ fill txt_index in a box
  xstringb(rect(1,1),rect(2,1), model.rpar, w, h,'fill') ;
  es=gce();
  es.font_style=model.ipar(1) ; 
  es.font_size  = model.ipar(2) ;
  es.font_foreground= model.ipar(3) ;
   
case 'getinputs' then

case 'getoutputs' then

case 'getorigin' then
  [x,y] = standard_origin(arg1)

case 'set' then
  x = arg1 ;
  graphics = arg1.graphics ;
  orig  = graphics.orig  ;
  exprs = graphics.exprs ;
  model = arg1.model ;
  if size(exprs,'*')==1 then
       exprs = [exprs;'3';'1']
  end // compatibility
  if size(exprs,'*')==3 then
       exprs = [exprs;'-1']
  end // compatibility
  
  while %t do
    [ok,txt,font,siz,fore,exprs] = getvalue('Set Text block parameters',..
	['Text';'Font number';'Font size';'Font foreground'], list('str',-1,'vec',1,'vec',1,'vec',1),exprs)
    
    if ~ok then break,end //** 
    
    if font<=0|font>9 then
      message('Font number must be greater than 0 and less than 10')
      ok=%f
    end
    
    if siz<0 then
      message('Font size must be positive')
      ok=%f
    end
    
    if ok then
      graphics.exprs = exprs
    
      
      //** store the box coordinate that contains the string
      r = xstringl(0,0,exprs(1),font,siz);
      
      sz = r(3:4) ; 
      graphics.sz = sz        ;
      x.graphics  = graphics  ;
      ipar        = [font;siz;fore]
      model.rpar  = txt   ;
      model.ipar  = ipar  ;
      x.model     = model ;
      break
    end
    
  end //** of while 
   
  
case 'define' then
  font = 2 ;
  siz  = 1 ;
 
  model = scicos_model()
  model.sim = 'text'
  model.rpar= 'Text'
  model.ipar=[font;siz;-1]

  exprs = ['Text';string(font); string(siz);'-1']
  graphics = scicos_graphics();
  graphics.orig = [0,0];
  graphics.sz =[2 1];
  graphics.exprs = exprs


  x = mlist(['Text','graphics','model','void','gui'],graphics,model,' ','TEXT_f')

end

endfunction
