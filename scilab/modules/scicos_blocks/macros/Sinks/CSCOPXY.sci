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

function [x,y,typ]=CSCOPXY(job,arg1,arg2)
    x=[];
    y=[];
    typ=[];
    select job
    case "set" then
        x=arg1;
        graphics=arg1.graphics;
        exprs=graphics.exprs
        model=arg1.model;
        //dstate=model.dstate;
        while %t do
            [ok,nbr_curves,clrs,siz,win,wpos,wdim,xmin,xmax,ymin,ymax,N,exprs]=scicos_getvalue(..
            msprintf(_("Set %s block parameters"), "CSCOPXY"), ..
            [_("Number of Curves");
             _("Curves styles: Colors>0 | marks<0");
             _("Curves thicknesses | marks sizes");
             _("Output window number (-1 for automatic)");
             _("Output window position");
             _("Output window sizes");
             "Xmin";
             "Xmax";
             "Ymin";
             "Ymax";
            _("Buffer size")],..
            list("vec",1,"vec",1,"vec",1,"vec",1,"vec",-1,"vec",-1,"vec",1,"vec",1,..
            "vec",1,"vec",1,"vec",1),exprs)
            if ~ok then
                break,
            end //user cancel modification

            mess=[];
            if size(wpos,"*")<>0 &size(wpos,"*")<>2 then
                mess=[mess ; _("''Window position'' must be [] or a 2 vector") ; " "]
            end
            if size(wdim,"*")<>0 &size(wdim,"*")<>2 then
                mess=[mess ; _("''Window sizes'' must be [] or a 2 vector") ; " "]
            end
            if nbr_curves<=0 then
                mess=[mess ; _("The Number of Curves must be > 0") ; " "]
            end
            if win<-1 then
                mess=[mess ; _("The Window number must be >= -1") ; " "]
            end
            if N<1 then
                mess=[mess ; _("The Buffer size must be >= 1") ; " "]
            end
            if N==1 & clrs>0 then
                mess=[mess ; _("The Buffer size must be >= 2") ; " "]
            end
            if ymin>=ymax then
                mess=[mess ; _("Ymax > Ymin is required.") ; " "]
            end
            if xmin>=xmax then
                mess=[mess ; _("Xmax > Xmin is required.") ; " "]
            end
            if mess<>[] then
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
                ipar=[win;1;N;clrs;siz;1;wpos(:);wdim(:);nbr_curves]
                //if prod(size(dstate))<>2*N+1 then dstate=-eye(2*N+1,1),end
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
        clrs=4;siz=1
        wdim=[600;400]
        wpos=[-1;-1]
        N=2;
        xmin=-15;
        xmax=15;
        ymin=-15;
        ymax=+15
        nbr_curves = 1;

        model=scicos_model()
        model.sim=list("cscopxy",4)
        model.in=[1;1]
        model.in2=[1;1]
        model.intyp=[1;1]
        model.evtin=1
        model.rpar=[xmin;xmax;ymin;ymax]
        model.ipar=[win;1;N;clrs;siz;1;wpos(:);wdim(:);nbr_curves]
        model.blocktype="d"
        model.dep_ut=[%f %f]

        exprs=[string(nbr_curves);
        sci2exp(clrs);
        sci2exp(siz);
        string(win);
        sci2exp([]);
        sci2exp(wdim);
        string(xmin);
        string(xmax);
        string(ymin);
        string(ymax);
        string(N)];
        gr_i=[]
        x=standard_define([2 2],model,exprs,gr_i)
    end
endfunction
