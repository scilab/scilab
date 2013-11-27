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

function [x,y,typ]=CFSCOPE(job,arg1,arg2)
    x=[];
    y=[];
    typ=[];
    select job
    case "set" then
        x=arg1;
        graphics=arg1.graphics;
        exprs=graphics.exprs

        model=arg1.model;
        //dstate=model.in
        while %t do
            [ok,clrs,win,wpos,wdim,ymin,ymax,per,N,wu,exprs]=scicos_getvalue(..
            "Set Scope parameters",..
            ["Color (>0) or mark (<0) vector (8 entries)";
            "Output window number (-1 for automatic)";
            "Output window position";
            "Output window sizes";
            "Ymin";
            "Ymax";
            "Refresh period";
            "Buffer size"
            "Links to view"],..
            list("vec",8,"vec",1,"vec",-1,"vec",-1,"vec",1,..
            "vec",1,"vec",1,"vec",1,"vec",-1),..
            exprs)
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
            if per<=0 then
                mess=[mess;"Refresh period must be positive";" "]
                ok=%f
            end
            if N<2 then
                mess=[mess;"Buffer size must be at least 2";" "]
                ok=%f
            end
            if ymin>=ymax then
                mess=[mess;"Ymax must be greater than Ymin";" "]
                ok=%f
            end
            if wu<0 then
                mess=[mess;"Link to view must be positive";" "]
                ok=%f
            end
            if ~ok then
                message(["Some specified values are inconsistent:";
                " ";mess])
            end
            if ok then
                if wpos==[] then
                    wpos=[-1;-1];
                end
                if wdim==[] then
                    wdim=[-1;-1];
                end
                rpar=[0;ymin;ymax;per]
                if size(clrs,"*")>8 then
                    clrs=clrs(1:8);
                end
                if size(clrs,"*")<8 then
                    clrs(8)=0;
                end
                ipar=[win;1;N;clrs(:);wpos(:);wdim(:);size(wu,"*");wu(:)]
                //if prod(size(dstate))<>(8+1)*N+1 then dstate=-eye((8+1)*N+1,1),end
                //model.dstate=dstate;
                model.rpar=rpar;
                model.ipar=ipar
                model.firing=[] //compatibility
                model.dep_ut=[%t %f] //compatibility
                graphics.exprs=exprs;
                x.graphics=graphics;
                x.model=model
                break
            end
        end
    case "define" then
        win=-1;
        wdim=[600;400]
        wpos=[-1;-1]
        clrs=[1;3;5;7;9;11;13;15];
        N=2;
        ymin=-15;
        ymax=+15;
        per=30;
        model=scicos_model()
        model.sim=list("cfscope",4)
        model.evtin=1
        model.rpar=[0;ymin;ymax;per]
        model.ipar=[win;1;N;clrs;wpos;wdim;1;1]
        model.blocktype="c"
        model.dep_ut=[%t %f]

        exprs=[strcat(string(clrs)," ");
        string(win);
        sci2exp([]);
        sci2exp(wdim);
        string(ymin);
        string(ymax);
        string(per);
        string(N);
        string([1])];
        gr_i=[]
        x=standard_define([2 2],model,exprs,gr_i)
    end
endfunction
