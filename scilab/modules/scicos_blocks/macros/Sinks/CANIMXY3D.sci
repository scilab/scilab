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
            "Set Scope parameters",..
            ["Number of curves";
            "color (>0) or mark (<0)";
            "line or mark size";
            "Output window number (-1 for automatic)";
            "Output window position";
            "Output window sizes";
            "Xmin and Xmax";
            "Ymin and Ymax";
            "Zmin and Zmax";
            "Alpha and Theta";
            "Buffer size"],..
            list("vec",1,"vec",-1,"vec",-1,"vec",1,"vec",-1,"vec",-1,"vec",-1,"vec",-1,"vec",-1,"vec",-1,"vec",1),exprs)
            if ~ok then
                break,
            end //user cancel modification
            mess=[]
            if size(wpos,"*")<>0 &size(wpos,"*")<>2 then
                mess=[mess;"Window position must be [] or a 2 vector";" "]
                ok=%f
            end
            if size(wdim,"*")<>0 &size(wdim,"*")<>2 then
                mess=[mess;"Window dim must be [] or a 2 vector";" "]
                ok=%f
            end
            if win<-1 then
                mess=[mess;"Window number cannot be inferior than -1";" "]
                ok=%f
            end
            if size(clrs,"*")<>size(siz,"*") then
                mess=[mess;"Colors and Size must have same size";" "]
                ok=%f
            end
            if nbr_curves<=0 then
                mess=[mess;"Number of curves cannot be negative or null";" "]
                ok=%f
            end
            if size(clrs,"*")<nbr_curves then
                mess=[mess;"You must have at least same size for clrs and the number of curves";" "]
                ok=%f
            end
            if N<1 then
                mess=[mess;"Buffer size must be at least 1";" "]
                ok=%f
            end
            if N<2
                for i=1:nbr_curves
                    if clrs(i)>0 then
                        mess=[mess;"Buffer size must be at least 2 or Change a color (must be <0)";" "]
                        ok=%f
                    end
                end
            end
            if vec_y(1)>=vec_y(2) then
                mess=[mess;"Ymax must be higher than Ymin";" "]
                ok=%f
            end
            if vec_x(1)>=vec_x(2) then
                mess=[mess;"Xmax must be higher than Xmin";" "]
                ok=%f
            end
            if vec_z(1)>=vec_z(2) then
                mess=[mess;"Zmax must be higher than Zmin";" "]
                ok=%f
            end
            if ~ok then
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
