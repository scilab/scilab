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

function [x,y,typ]=CANIMXY(job,arg1,arg2)
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
        //dstate=model.dstate

        while %t do
            [ok,nbr_curves,clrs,siz,win,wpos,wdim,xmin,xmax,ymin,ymax,N,exprs]=scicos_getvalue(..
            "Set Scope parameters",..
            ["Number of Curves";
            "color (>0) or mark (<0)";
            "line or mark size";
            "Output window number (-1 for automatic)";
            "Output window position";
            "Output window sizes";
            "Xmin";
            "Xmax";
            "Ymin";
            "Ymax";
            "Buffer size"],..
            list("vec",1,"vec",1,"vec",1,"vec",1,"vec",-1,"vec",-1,"vec",1,..
            "vec",1,"vec",1,"vec",1,"vec",1),exprs)
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
            if nbr_curves<=0 then
                mess=[mess;"Number of curves cannot be negative or null";" "]
                ok=%f
            end
            if N<1 then
                mess=[mess;"Buffer size must be at least 1";" "]
                ok=%f
            end
            if N==1 & clrs>0 then
                mess=[mess;"Buffer size must be at least 2";" "]
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
                in = nbr_curves*ones(2,1);
                in2 = ones(2,1);
                [model,graphics,ok]=set_io(model,graphics,list([in in2],ones(2,1)),list(),ones(1,1),[]);
                if wpos==[] then
                    wpos=[-1;-1];
                end
                if wdim==[] then
                    wdim=[-1;-1];
                end
                rpar=[xmin;xmax;ymin;ymax]
                ipar=[win;1;N;clrs;siz;0;wpos(:);wdim(:);nbr_curves]
                //if prod(size(dstate))<>2*N+1 then dstate=zeros(2*N+1,1),end
                //model.dstate=dstate;
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
        clrs=-4;
        N=2;
        siz=1;
        wpos=[-1;-1];
        wdim=[-1;-1];
        xmin=-15;
        xmax=15;
        ymin=-15;
        ymax=+15
        nbr_curves = 1;

        model=scicos_model()
        model.sim=list("canimxy",4)
        model.in=[1;1]
        model.in2=[1;1]
        model.intyp=[1;1]
        model.evtin=1
        model.rpar=[xmin;xmax;ymin;ymax]
        model.ipar=[win;1;N;clrs;siz;0;wpos(:);wdim(:);nbr_curves]
        model.blocktype="d"
        model.firing=[]
        model.dep_ut=[%f %f]

        exprs=[string(nbr_curves);
        string(clrs);
        string(siz);
        string(win);
        "[]";
        "[]";
        string(xmin);
        string(xmax);
        string(ymin);
        string(ymax);
        string(N)]
        gr_i=[]
        x=standard_define([2 2],model,exprs,gr_i)
    end
endfunction
