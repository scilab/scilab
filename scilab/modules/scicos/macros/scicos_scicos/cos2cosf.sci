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

function ierr = cos2cosf(u,scs_m,count)

    //write scilab instructions whose evaluation
    //returns the  value of scicos data structure scs_m.
    //in the opened file associated with logical unit u

    [lhs,rhs]=argn(0)
    ierr=0;
    if rhs<3 then
        count=0,
        lname="scs_m"
    else
        count=count+1
        lname="scs_m_"+string(count)
    end

    lmax=80;

    //////////////////
    txt=[];

    //version
    txt=[txt;"version="""+get_scicos_version()+""",.."];

    //scicos_params
    tt=[];
    fields=getfield(1,scs_m.props);
    for i=1:lstsize(scs_m.props)-1
        field_nam=fields(i+1);
        if field_nam=="title" then field_nam="Title", end
        tt2=sci2exp(getfield(i+1,scs_m.props),lmax);
        tt2(1)=field_nam+"="+tt2(1);
        if i<>lstsize(scs_m.props)-1 then
            tt2($)=tt2($)+",";
        end
        tt=[tt;tt2];
    end

    tt=my_strcat(tt);
    txt=[txt;
    "props=scicos_params(..";
    "      "+tt(1)];
    for i=2:size(tt,1)
        txt=[txt;"      "+tt(i)];
    end
    txt($)=txt($)+")";

    //scicos_diagram
    txt=["scicos_diagram(..";txt];
    txt($)=txt($)+")";

    //final work
    txt(1)=lname+"="+txt(1);
    bl1=" ";
    for i=2:size(txt,1)
        txt(i)=part(bl1,1:length(lname)+1)+txt(i);
    end
    //////////////////

    ierr=execstr("write(u,txt,''(a)'')","errcatch");
    if ierr<>0 then return, end;

    t=[];

    for k=1:lstsize(scs_m.objs)

        o=scs_m.objs(k)
        if typeof(o)=="Block" then
            lhs=lname+".objs("+string(k)+")="

            if o.model.sim=="super"| o.model.sim=="csuper"| o.model.sim(1)=="asuper" then  //Super blocks
                cos2cosf(u,o.model.rpar,count);//model.rpar
            end
            //////////////////
            txt=[];

            //gui
            tt=sci2exp(o.gui);
            txt=[txt;"gui="+tt(1)];
            for i=2:size(tt,1)
                txt=[txt;"         "+tt(i)];
            end
            txt($)=txt($)+",..";

            //scicos_graphics
            tt=[];
            fields=getfield(1,o.graphics);
            for i=1:lstsize(o.graphics)-1
                field_nam=fields(i+1);
                tt2=sci2exp(getfield(i+1,o.graphics),lmax);
                tt2(1)=field_nam+"="+tt2(1);
                if i<>lstsize(o.graphics)-1 then
                    tt2($)=tt2($)+",";
                end
                tt=[tt;tt2];
            end

            tt=my_strcat(tt);
            txt=[txt;
            "graphics=scicos_graphics(..";
            "         "+tt(1)];
            for i=2:size(tt,1)
                txt=[txt;"         "+tt(i)];
            end
            txt($)=txt($)+"),..";

            //scicos_model
            tt=[];
            fields=getfield(1,o.model);
            for i=1:lstsize(o.model)-1
                field_nam=fields(i+1);
                if field_nam=="rpar"&(o.model.sim=="super"| o.model.sim=="csuper"| o.model.sim(1)=="asuper") then
                    tt2="scs_m_"+string(count+1);
                else
                    tt2=sci2exp(getfield(i+1,o.model),lmax);
                end
                tt2(1)=field_nam+"="+tt2(1);
                if i<>lstsize(o.model)-1 then
                    tt2($)=tt2($)+",";
                end
                tt=[tt;tt2];
            end

            tt=my_strcat(tt);
            txt=[txt;
            "model=scicos_model(..";
            "         "+tt(1)];
            for i=2:size(tt,1)
                txt=[txt;"         "+tt(i)];
            end
            txt($)=txt($)+"),..";

            //doc
            tt=sci2exp(o.doc,50);
            txt=[txt;"doc="+tt(1)];
            for i=2:size(tt,1)
                txt=[txt;"         "+tt(i)];
            end

            //scicos_block
            txt=["scicos_block(..";txt];
            txt($)=txt($)+")";

            //final work
            txt(1)=lhs+txt(1);
            bl1=" ";
            for i=2:size(txt,1)
                txt(i)=part(bl1,1:length(lhs))+txt(i);
            end
            write(u,txt,"(a)");
            ///////////////
        else //link
            lhs=lname+".objs("+string(k)+")="
            if typeof(o)=="Link" then
                //scicos_link
                tt=[];
                txt=[];
                fields=getfield(1,o);
                for i=1:lstsize(o)-1
                    field_nam=fields(i+1);
                    tt2=sci2exp(getfield(i+1,o),lmax);
                    tt2(1)=field_nam+"="+tt2(1);
                    if i<>lstsize(o)-1 then
                        tt2($)=tt2($)+",";
                    end
                    tt=[tt;tt2];
                end

                tt=my_strcat(tt);
                txt=[txt;
                "scicos_link(..";
                "  "+tt(1)];
                for i=2:size(tt,1)
                    txt=[txt;"  "+tt(i)];
                end
                txt($)=txt($)+")";

                //final work
                txt(1)=lhs+txt(1);
                bl1=" ";
                for i=2:size(txt,1)
                    txt(i)=part(bl1,1:length(lhs))+txt(i);
                end
                write(u,txt,"(a)");

            else // ??
                // Alan : JESAISPASIYADAUTRESOBJS
                // QUEDESBLOCKSETDESLINKSDANSSCICOS
                // ALORSJELAISSELEVIEUCODE
                t=[]
                t1=sci2exp(o,lmax-length(lhs))
                n1=size(t1,1)
                bl1=" ";bl1=part(bl1,1:length(lhs))
                t=[t;lhs+t1(1);bl1(ones(n1-1,1))+t1(2:$)]
                write(u,t,"(a)");
            end
        end
    end

endfunction

function txt_out=my_strcat(txt)
    txt_out=[]
    for i=1:size(txt,1)
        txt_out(i)=txt(i);
        if length(txt_out(i))<>0 then
            if part(txt_out(i),length(txt_out(i)))=="," then
                txt_out(i)=txt_out(i)+"..";
            end
        end
    end
endfunction
