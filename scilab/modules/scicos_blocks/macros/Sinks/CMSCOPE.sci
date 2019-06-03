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

function [x,y,typ] = CMSCOPE(job,arg1,arg2)
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
        //pause
        while %t do
            [ok,in,clrs,win,wpos,wdim,ymin,ymax,per,N,heritance,nom,exprs]=..
            scicos_getvalue(msprintf(_("Set %s block parameters"), "CMSCOPE"),..
             _(["Input ports sizes";
                "Curves styles: Colors>0 | marks<0";
                "Output window number (-1 for automatic)";
                "Output window position";
                "Output window sizes";
                "Ymin vector";
                "Ymax vector";
                "Refresh periods";
                "Buffer size";
                "Accept herited events 0/1";
                "Name of Scope (label&Id)"]),..
            list("vec",-1,"vec",-1,"vec",1,"vec",-1,"vec",-1,..
            "vec","size(%1,''*'')","vec","size(%1,''*'')","vec","size(%1,''*'')",..
            "vec",1,"vec",1,"str",1), exprs)
            if ~ok then
                break,
            end //user cancel modification
            mess = []
            if size(in,"*")<=0 then
                mess=[mess ; _("The block must have at least one input port"); " "]
            end
            if min(in)<=0 then
                mess=[mess ; _("Port sizes must be > 0") ; " "]
            end
            if size(clrs,"*")<sum(in) then
                msg = _("At least %d curves styles expected.")
                mess=[mess ; msprintf(msg, sum(in)) ; " "]
            end
            if size(wpos,"*")<>0 &size(wpos,"*")<>2 then
                mess = [mess ; _("''Window position'' must be [] or a 2 vector") ; " "]
            end
            if size(wdim,"*")<>0 &size(wdim,"*")<>2 then
                mess = [mess ; _("''Window sizes'' must be [] or a 2 vector") ; " "]
            end
            if win<-1 then
                mess=[mess ; _("The Window number must be >= -1") ; " "]
            end
            if size(per,"*")<>size(ymin,"*") then
                mess=[mess ;
                      _("The size of ''Refresh Periods'' must match the Ymin|Ymax''s one");
                      " "]
            end
            for i = 1:size(per,"*")
                if (per(i)<=0) then
                    mess=[mess ; _("All Refresh Periods must be > 0") ; " "]
                end
            end
            if N<2 then
                mess=[mess ; _("The Buffer size must be >= 2") ; " "]
            end
            if or(ymin>=ymax) then
                mess=[mess ; _("Ymax > Ymin is required") ; " "]
            end
            if ~or(heritance==[0 1]) then
                mess=[mess ; _("''Accept herited events'' must be 0 or 1") ; " "]
            end

            if mess <> [] then
                message([_("Some specified values are inconsistent:");
                " ";mess])
            else
                in = in(:);
                a = size(in,1);
                in2 = ones(a,1);
                [model,graphics,ok]=set_io(model,graphics,list([in in2],ones(a,1)),list(),ones(1-heritance,1),[]);

                if wpos==[] then
                    wpos=[-1;-1];
                end
                if wdim==[] then
                    wdim=[-1;-1];
                end
                period=per(:)';
                yy=[ymin(:)';ymax(:)']
                rpar=[0;period(:);yy(:)]
                clrs=clrs(1:sum(in))
                ipar=[win;size(in,"*");N;wpos(:);wdim(:);in(:);clrs(:);heritance]
                //if prod(size(dstate))<>(sum(in)+1)*N+1 then
                //dstate=-eye((sum(in)+1)*N+1,1),
                //end
                model.evtin=ones(1-heritance,1)
                model.dstate=[]
                //model.dstate=dstate;
                model.rpar=rpar;
                model.ipar=ipar
                model.label=nom;
                graphics.id=nom;
                graphics.exprs=exprs;
                graphics.style = "CMSCOPE;verticalLabelPosition=bottom;" + ..
                                 "verticalAlign=top;spacing=2;displayedLabel=" + nom
                x.graphics=graphics;
                x.model=model
                //pause;
                break
            end
        end

    case "define" then
        win=-1;
        in=[1;1]
        wdim=[-1;-1]
        wpos=[-1;-1]
        clrs=[1;3;5;7;9;11;13;15];
        N=20;

        ymin=[-1;-5];
        ymax=[1;5];
        per=[30;30];
        yy=[ymin(:)';ymax(:)']
        period=per(:)'
        model=scicos_model()
        model.sim=list("cmscope",4)
        model.in=in
        model.in2=[1;1]
        model.intyp=[1;1]
        model.evtin=1
        model.rpar=[0;period(:);yy(:)]
        model.ipar=[win;size(in,"*");N;wpos(:);wdim(:);in(:);clrs(1:sum(in))]
        model.blocktype="c"
        model.dep_ut=[%t %f]

        exprs=[strcat(string(in)," ");
        strcat(string(clrs)," ");
        string(win);
        sci2exp([]);
        sci2exp([]);
        strcat(string(ymin)," ");
        strcat(string(ymax)," ");
        strcat(string(per)," ");
        string(N);
        string(0);
        emptystr()];
        gr_i=[]
        x=standard_define([2 2],model,exprs,gr_i)
    end
endfunction
