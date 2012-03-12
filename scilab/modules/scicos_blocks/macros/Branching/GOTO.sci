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

function [x,y,typ]=GOTO(job,arg1,arg2)
  x=[];y=[];typ=[]
  select job
   case 'plot' then
    //++ Save current pattern in 'pat' and set up a new one
    pat    = xget('pattern'); xset('pattern',default_color(0)); //++ pat = 34 (white); default_color(0) = 1
    orig   = arg1.graphics.orig       //++ position
    sz     = arg1.graphics.sz         //++ size
    orient = arg1.graphics.flip       //++ orientation
    tg     = arg1.graphics.exprs(1)
    forma  = ['[','{','',']','}','']
    tg     = forma(evstr(arg1.graphics.exprs(2)))+tg+forma(evstr(arg1.graphics.exprs(2))+3)
    gr_i   = arg1.graphics.gr_i
    
    if type(gr_i) == 15 & gr_i(2) <> 36 then
      [gr_i, coli] = gr_i(1:2)
    else
      coli = []
    end

    //++ Draw contour +  variable name & scope/visibility
    if orient then //++ standard orientation

      xx = orig(1) + [  0    1/8   1/4 ;
                       1/4   1/2    1  ;
                        1     1     1  ;
                        1    1/2   1/4 ;
                       1/4   1/8    0   ]'*sz(1)

      yy = orig(2) + [ 1/2   3/4    1  ;
                        1     1     1  ;
                        1    1/2    0  ;
                        0     0     0  ;
                        0    1/4   1/2 ]'*sz(2)

      x1 = 0 //++ used by xstringb to set up text position

    else //++ flipped orientation

      xx = orig(1) + [  0     0     0  ;
                        0    1/2   3/4 ;
                       3/4   7/8    1  ;
                        1    7/8   3/4 ;
                       3/4   1/2    0  ]'*sz(1)

      yy = orig(2) + [  0    1/2    1  ;
                        1     1     1  ;
                        1    3/4   1/2 ;
                       1/2   1/4    0  ;
                        0     0     0  ]'*sz(2)

      x1 = 1/4 //++ used by xstringb to set up text position
    end
    xpolys(xx,yy) //++ contour
    xfpoly(xx(1,:),yy(1,:)) //++ fill
    gh_e = gce()
    if coli <> []
      gh_e.fill_mode = "on"
      gh_e.background = coli
    end
    xstringb(orig(1)+x1*sz(1), orig(2), tg, (1-x1)*sz(1), sz(2)) //++ variable name & scope/visibility


    //++ Draw input and output ports
    xf   = 60
    yf   = 40
    nin  = 1
    nout = 0

    if orient then  //++ standard orientation

      // set port shape
      out = [ 0    -1/14
              1/7   0
              0     1/14
              0    -1/14]*diag([xf,yf])
      in =  [-1/7  -1/14
              0     0
             -1/7   1/14
             -1/7  -1/14]*diag([xf,yf])

      xset('pattern',default_color(1)) //++ default_color(1) = 1

      //++ Draw output ports
      dy = sz(2)/(nout+1)
      for k = 1:nout
        xfpoly(out(:,1) + ones(4,1)*(orig(1)+sz(1)),..
               out(:,2) + ones(4,1)*(orig(2)+sz(2)-dy*k),1)
      end

      //++ Draw input ports
      dy = sz(2)/(nin+1)
      for k = 1:nin
        xfpoly(in(:,1) + ones(4,1)*orig(1),..
               in(:,2) + ones(4,1)*(orig(2)+sz(2)-dy*k),1)
      end

    else //++ flipped orientation

      out = [ 0    -1/14
             -1/7   0
              0     1/14
              0    -1/14]*diag([xf,yf])
      in =  [ 1/7  -1/14
              0     0
              1/7   1/14
              1/7  -1/14]*diag([xf,yf])

      xset('pattern',default_color(1)) //++ default_color(1) = 1

      //++ Draw output ports
      dy = sz(2)/(nout+1)
      for k = 1:nout
        xfpoly(out(:,1) + ones(4,1)*orig(1)-1,..
               out(:,2) + ones(4,1)*(orig(2)+sz(2)-dy*k),1)
      end

      //++ Draw input ports
      dy = sz(2)/(nin+1)
      for k = 1:nin
        xfpoly(in(:,1) + ones(4,1)*(orig(1)+sz(1))+1,..
               in(:,2) + ones(4,1)*(orig(2)+sz(2)-dy*k),1)
      end
    end

    //++ Restore previously used pattern
    xset('pattern',pat)

    //** ------- Identification ---------------------------
    ident   = o.graphics.id
    gh_axes = gca() //** get the Axes proprieties

    // draw Identification
    if ident <> [] & ident <> ''  then

      //** Save font state
      axes_font_style = gh_axes.font_style
      axes_font_size  = gh_axes.font_size
      //** axes_font_color = gh_axes.font_color ; //** optional

      gh_axes.font_style = options.ID(1)(1)
      gh_axes.font_size  = options.ID(1)(2)
      //** font color not yet used
      rectangle = xstringl(orig(1), orig(2), ident)
      w = max(rectangle(3), sz(1))
      h = rectangle(4) * 1.3
      xstringb(orig(1) + sz(1) / 2 - w / 2, orig(2) - h , ident , w, h)

     //** Restore font state
     gh_axes.font_style = axes_font_style
     gh_axes.font_size  = axes_font_size
     //** gh_axes.font_color = axes_font_color //** optional

    end
    //** ----- Identification End -----------------------------

   case 'getinputs' then
    [x,y,typ]=standard_inputs(arg1)
   case 'getoutputs' then
    [x,y,typ]=standard_outputs(arg1)
   case 'getorigin' then
    [x,y]=standard_origin(arg1)
   case 'set' then
    x=arg1;
    graphics=arg1.graphics;exprs=graphics.exprs
    model=arg1.model;
    while %t do
      [ok,tag,tagvis,exprs]=scicos_getvalue('Set parameters',..
		['Tag';'Tag Visibility(1=Local 2=scoped 3= global)'],..
		    list('str',-1,'vec',1),exprs)
      if ~ok then break,end
      tagvis=int(tagvis)
      if ((tagvis<1)|(tagvis>3)) then
          message('Tag Visibility must be between 1 and 3');ok=%f;
      end
      if ok then
	 if ((model.ipar<>tagvis)|(model.opar<>list(tag))) then needcompile=4;y=needcompile,end
	 graphics.exprs=exprs;
	 model.opar=list(tag)
	 model.ipar=tagvis
	 x.model=model
	 x.graphics=graphics
	 arg1=x
	 break
      end
    end
   needcompile=resume(needcompile)
   case 'define' then
    model=scicos_model()
    model.sim='goto'
    model.in=-1
    model.in2=-2
    model.intyp=-1
    model.out=[]
    model.out2=[]
    model.outtyp=1
    model.ipar=int(1)
    model.opar=list('A')
    model.blocktype='c'
    model.dep_ut=[%f %f]

    exprs=['A';sci2exp(1)]

    gr_i='';
    x=standard_define([1.5 1.5],model,exprs,gr_i)
    x.graphics.id="Goto"
  end
endfunction

