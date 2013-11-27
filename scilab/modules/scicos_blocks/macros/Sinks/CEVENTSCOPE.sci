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

function [x,y,typ]=CEVENTSCOPE(job,arg1,arg2)
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
            [ok,nclock,clrs,win,wpos,wdim,per,exprs]=scicos_getvalue(..
            "Set Scope parameters",..
            ["Number of event inputs";
            "colors c (>0) or mark (<0)";
            "Output window number (-1 for automatic)";
            "Output window position";
            "Output window sizes";
            "Refresh period"],..
            list("vec",1,"vec",-1,"vec",1,"vec",-1,"vec",-1,"vec",1),exprs);
            nclock=int(nclock)
            clrs=int(clrs)
            win=int(win)

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
            if nclock<=0 then
                mess=[mess;"Block must have at least one input event";" "]
                ok=%f
            end
            if size(clrs,"*")<>nclock then
                mess=[mess;"Inputs color c size must be equal to Number of inputs";" "]
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
            if ok then
                [model,graphics,ok]=set_io(model,graphics,list(),list(),ones(nclock,1),[])
            else
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
                rpar=per
                ipar=[win;1;clrs(:);wpos(:);wdim(:)]
                model.rpar=rpar;
                model.ipar=ipar
                graphics.exprs=exprs;
                x.graphics=graphics;
                x.model=model
                break
            end
        end
    case "define" then
        nclock=1
        win=-1;
        clrs=[1;3;5;7;9;11;13;15];
        wdim=[600;400]
        wpos=[-1;-1]
        per=30;

        model=scicos_model()
        model.sim=list("cevscpe",4)
        model.evtin=1
        model.rpar=per
        model.ipar=[win;1;clrs(nclock);wpos(:);wdim(:)]
        model.blocktype="d"
        model.dep_ut=[%f %f]

        exprs=[sci2exp(nclock);
        strcat(sci2exp(clrs(nclock))," ");
        string(win);
        sci2exp([]);
        sci2exp(wdim);
        string(per)]
        gr_i=[]
        x=standard_define([2 2],model,exprs,gr_i)
    end
endfunction
