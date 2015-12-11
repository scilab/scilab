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

function [x,y,typ]=BOUNCEXY(job,arg1,arg2)
    //Scicos 2D animated visualization block
    x=[];
    y=[];
    typ=[];
    select job
    case "set" then
        x=arg1;
        graphics=arg1.graphics;
        exprs=graphics.exprs
        model=arg1.model;
        dstate=model.dstate
        while %t do
            [ok,clrs,siz,win,imode,xmin,xmax,ymin,ymax,exprs]=scicos_getvalue(..
            "Set Scope parameters",..
            ["colors";
            "radii";
            "window number (-1 for automatic)";
            "animation mode (0,1)";
            "Xmin";
            "Xmax";
            "Ymin";
            "Ymax";
            ],..
            list("vec",-1,"vec",-1,"vec",1,"vec",1,"vec",1,"vec",1,"vec",1,"vec",1),exprs)
            if ~ok then
                break,
            end //user cancel modification

            mess=[]
            if size(clrs,"*")<>size(siz,"*") then
                mess=[mess;"colors and radii must have equal size (number of balls)";" "]
                ok=%f
            end
            if win<-1 then
                mess=[mess;"Window number cannot be inferior than -1";" "]
                ok=%f
            end
            if ymin>=ymax then
                mess=[mess;"Ymax must be greater than Ymin";" "]
                ok=%f
            end
            if xmin>=xmax then
                mess=[mess;"Xmax must be greater than Xmin";" "]
                ok=%f
            end
            if ~ok then
                message(mess)
            else
                rpar=[xmin;xmax;ymin;ymax]
                ipar=[win;imode;clrs(:)]
                z=[]
                for i=1:size(clrs,"*")
                    z(6*(i-1)+1)=0
                    z(6*(i-1)+2)=0
                    z(6*(i-1)+3)=2*siz(i)
                    z(6*(i-1)+4)=2*siz(i)
                    z(6*(i-1)+5)=0.000
                    z(6*(i-1)+6)=64.0*360.000;
                end
                model.dstate=z;
                model.rpar=rpar;model.ipar=ipar
                graphics.exprs=exprs;
                x.graphics=graphics;x.model=model
                break
            end
        end
    case "define" then
        win=-1;
        imode=1;
        clrs=[1;2];
        siz=[1;1]
        xmin=-5;
        xmax=5;
        ymin=0;
        ymax=15

        model=scicos_model()
        model.sim=list("bouncexy",4)
        model.in=[-1;-1]
        model.in2=[1;1]
        model.intyp = [1;1]
        model.evtin=1
        z=[]
        for i=1:size(clrs,"*")
            z(6*(i-1)+1)=0
            z(6*(i-1)+2)=0
            z(6*(i-1)+3)=2*siz(i)
            z(6*(i-1)+4)=2*siz(i)
            z(6*(i-1)+5)=0.000
            z(6*(i-1)+6)=64.0*360.000;
        end
        model.dstate=z
        model.rpar=[xmin;xmax;ymin;ymax]
        model.ipar=[win;imode;clrs(:)]
        model.blocktype="d"
        model.firing=[]
        model.dep_ut=[%f %f]

        exprs=[strcat(sci2exp(clrs));
        strcat(sci2exp(siz));
        strcat(sci2exp(win));
        strcat(sci2exp(1));
        strcat(sci2exp(xmin));
        strcat(sci2exp(xmax));
        strcat(sci2exp(ymin));
        strcat(sci2exp(ymax))]
        gr_i=[]
        x=standard_define([2 2],model,exprs,gr_i)
    end
endfunction
