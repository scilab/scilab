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

function [x,y,typ]=CANIMXY3D(job,arg1,arg2)
    //Scicos 3D animated visualization block
    x=[];
    y=[];
    typ=[];
    select job
    case "set" then
        x=arg1;
        graphics=arg1.graphics;
        exprs=graphics.exprs
        model=arg1.model;
        while %t do
            [ok,nbr_curves,clrs,siz,win,wpos,wdim,vec_x,vec_y,vec_z,param3ds,N,exprs]=scicos_getvalue(..
            msprintf(_("Set %s block parameters"), "CANIMXY3D Scope"),..
             [_("Number of curves");
              _("Curves styles: Colors>0 | marks<0");
              _("Curves thicknesses or marks sizes");
              _("Output window number (-1 for automatic)");
              _("Output window position");
              _("Output window sizes");
                "Xmin, Xmax";
                "Ymin, Ymax";
                "Zmin, Zmax";
                "Alpha, Theta";
              _("Buffer size")],..
            list("vec",1,"vec",-1,"vec",-1,"vec",1,"vec",-1,"vec",-1,"vec",-1,"vec",-1,"vec",-1,"vec",-1,"vec",1),exprs)
            if ~ok then
                break,
            end //user cancel modification
            mess = []
            if size(wpos,"*")<>0 &size(wpos,"*")<>2 then
                mess=[mess ; _("''Window position'' must be [] or a 2 vector") ; " "]
            end
            if size(wdim,"*")<>0 &size(wdim,"*")<>2 then
                mess=[mess ; _("''Window sizes'' must be [] or a 2 vector") ; " "]
            end
            if win<-1 then
                mess=[mess ; _("The Window number must be >= -1") ; " "]
            end
            if size(clrs,"*")<>size(siz,"*") then
                mess=[mess ; _("The numbers of curves styles and of<br>curves thickness or marks size must match.") ; " "]
            end
            if nbr_curves<=0 then
                mess=[mess ; _("The Number of curves must be >= 1") ; " "]
            end
            if size(clrs,"*")<nbr_curves then
                msg = _("Not enough curves styles: At least %d expected.")
                mess=[mess ; msprintf(msg, nbr_curves) ; " "]
            end
            if N<1 then
                mess=[mess ; _("The Buffer size must be >= 1") ; " "]
            end
            if N<2
                for i=1:nbr_curves
                    if clrs(i)>0 then
                        mess=[mess;
                        _("The Buffer size must be >= 2, or Change a color (must be <0)");
                        " "]
                    end
                end
            end
            if vec_y(1)>=vec_y(2) then
                mess=[mess _("Ymax > Ymin is required") ; " "]
            end
            if vec_x(1)>=vec_x(2) then
                mess=[mess ; _("Xmax > Xmin is required") ; " "]
            end
            if vec_z(1)>=vec_z(2) then
                mess=[mess ; _("Zmax > Zmin is required") ; " "]
            end
            //
            if mess <> [] then
                message(mess)
            else
                in = nbr_curves*ones(3,1);
                in2 = ones(3,1);
                [model,graphics,ok]=set_io(model,graphics,list([in in2],ones(3,1)),list(),ones(1,1),[]);
                if wpos==[] then
                    wpos=[-1;-1];
                end
                if wdim==[] then
                    wdim=[-1;-1];
                end
                rpar=[vec_x(:);vec_y(:);vec_z(:);param3ds(:)]
                size_siz = size(siz,"*");
                ipar=[win;size_siz;N;clrs(:);siz(:);1;wpos(:);wdim(:);nbr_curves]
                model.rpar=rpar;
                model.ipar=ipar
                graphics.exprs=exprs;
                x.graphics=graphics;
                x.model=model
                break
            end
        end

    case "define" then
        win=-1;
        N=2;
        clrs = [1;2;3;4;5;6;7;13]
        siz = [1;1;1;1;1;1;1;1]
        wpos=[-1;-1];
        wdim=[-1;-1];
        param3ds=[50;280]
        vec_x = [-15;15]
        vec_y = [-15;15]
        vec_z = [-15;15]
        nbr_curves = 1;

        model=scicos_model()
        model.sim=list("canimxy3d",4)
        model.in=[1;1;1]
        model.evtin=1
        model.in2=[1;1;1]
        model.intyp=[1;1;1]
        model.rpar=[vec_x(:);vec_y(:);vec_z(:);param3ds(:)]
        model.ipar=[win;8;N;clrs(:);siz(:);8;wpos(:);wdim(:);nbr_curves]
        model.blocktype="d"
        model.firing=[]
        model.dep_ut=[%f %f]

        exprs=[string(nbr_curves);
        strcat(string(clrs)," ");
        strcat(string(siz)," ");
        string(win);
        "[]";
        "[]";
        strcat(string(vec_x)," ");
        strcat(string(vec_y)," ");
        strcat(string(vec_z)," ");
        strcat(string(param3ds)," ");
        string(N)]
        gr_i=[]
        x=standard_define([2 2],model,exprs,gr_i)
    end
endfunction
