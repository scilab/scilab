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

function [x,y,typ]=TEXT_f(job,arg1,arg2)
    //** 22-23 Aug 2006: some carefull adjustements for the fonts
    //**                 inside the new graphics datastructure
    x=[];
    y=[];
    typ=[];

    select job

    case "set" then
        x = arg1 ;
        graphics = arg1.graphics ;
        orig  = graphics.orig  ;
        exprs = graphics.exprs ;
        model = arg1.model ;
        if size(exprs,"*")==1 then
            exprs = [exprs;"3";"1"]
        end // compatibility

        while %t do
            [ok,txt,font,siz,exprs] = scicos_getvalue("Set Text block parameters",..
            ["Text";"Font number";"Font size"], list("str",-1,"vec",1,"vec",1),exprs)

            if ~ok then
                break,
            end //**

            if font<=0|font>6 then
                message("Font number must be greater than 0 and less than 7")
                ok=%f
            end

            if siz<0 then
                message("Font size must be positive")
                ok=%f
            end

            if ok then
                graphics.exprs = exprs

                //** save the font
                //** oldfont = xget('font')
                gh_winpal = gca(); //** get the current Axes proprieties
                default_font_style = gh_winpal.font_style ;
                default_font_size  = gh_winpal.font_size  ;
                default_font_color = gh_winpal.font_color ;

                //** set the new font
                //** xset('font',font,siz)
                gh_winpal.font_style = font ;
                gh_winpal.font_size  = siz  ;


                //** store the box coordinate that contains the string
                r = xstringl(0,0,exprs(1),evstr(exprs(2)),evstr(exprs(3)));

                //** restore the old font
                //** xset('font',oldfont(1),oldfont(2));
                gh_winpal.font_style = default_font_style ;
                gh_winpal.font_size  = default_font_size  ;
                gh_winpal.font_color = default_font_color ;

                sz = r(3:4) ;
                graphics.sz = sz        ;
                x.graphics  = graphics  ;
                ipar        = [font;siz]
                model.rpar  = txt   ;
                model.ipar  = ipar  ;
                x.model     = model ;
                break
            end

        end //** of while


    case "define" then
        font = 2 ;
        siz  = 1 ;

        model = scicos_model()
        model.sim = "text"
        model.rpar= "Text"
        model.ipar=[font;siz]

        exprs = ["Text";string(font); string(siz)]
        graphics = scicos_graphics();
        graphics.orig = [0,0];
        graphics.sz =[2 1];
        graphics.exprs = exprs


        x = scicos_new(["Text","graphics","model","void","gui"],graphics,model," ","TEXT_f")

    end

endfunction
