//  Xcos
//
//  Copyright (C) INRIA - METALAU Project <scicos@inria.fr>
//                      - Alan Layec <alan.layec@inria.fr>
//  Copyright (C) 2011 <bernard.dujardin@contrib.scilab.org>
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

function bad_connection(path_out,prt_out,nout,outtyp,path_in,prt_in,nin,intyp,typ)
    // alert for badly connected blocks
    // path_out : Path of the "from block" in scs_m
    // path_in  : Path of the "to block" in scs_m
    // Alan, 28/12/06 : added rhs parameter : outtyp/intyp and typ flag.
    // typ : a flag. If not present or equal to zero then
    //               display a message concerning size.
    //               Else if equal to 1 then display a message
    //               concerning type.
    //!

    if %scicos_debug_gr then
        disp("bad_connection...")
    end

    rhs = argn(2) ;
    if (rhs == 8) then typ=0, end

    if type(path_out)==15 then //set of modelica blocks
        // look for modelica bloc associated with prt_out
        outports=list()
        for b=path_out,
            path=list();
            for l=b(1:$-1),
                path($+1)=l;path($+1)="model";path($+1)="rpar";path($+1)="objs";
            end
            path($+1)=b($);
            if size(path)==1 then path=path(1),end
            mb=scs_m.objs(path)
            k=find(mb.graphics.out_implicit=="E")
            for kk=k,outports($+1)=path,end
        end
        path_out=outports(prt_out)
    end
    if type(path_in)==15 then //set of modelica blocks
        // look for modelica bloc associated with prt_in
        inports=list()
        for b=path_in,
            path=list();
            for l=b(1:$-1),
                path($+1)=l;path($+1)="model";path($+1)="rpar";path($+1)="objs";
            end
            path($+1)=b($);
            if size(path)==1 then path=path(1),end
            mb=scs_m.objs(path)
            k=find(mb.graphics.in_implicit=="E")
            for kk=k,inports($+1)=path,end
        end
        path_in=inports(prt_in)
    end

    if type(path_out)==15 then //problem with implicit block
        messagebox([gettext("Problem with the block generated from modelica blocks.")],"warning", "modal")
        return;
    end


    // warn the output port block
    if typ==0 then
        msg = "<html><body>";
        msg = msg + gettext("Block has connected output port<br />with incompatible size:");
        msg = msg + "<ul>";
        msg = msg + "<li>" + msprintf(gettext("Output port %s size is: %s"),string(prt_out), sci2exp(nout)) + "</li>";
        if prt_in <> -1 then
            msg = msg + "<li>" + msprintf(gettext("Input port %s size is: %s"),string(prt_in), sci2exp(nin)) + "</li>";
        end
        msg = msg + "</ul>";
        msg = msg + "</body></html>";
    else
        msg = "<html><body>";
        msg = gettext("Block has connected output port<br />with incompatible type.");
        msg = msg + "<ul>";
        msg = msg + "<li>" + msprintf(gettext("Output port %s type is: %s"),string(prt_out), sci2exp(outtyp)) + "</li>";
        if prt_in <> -1 then
            msg = msg + "<li>" + msprintf(gettext("Input port %s type is: %s"),string(prt_in), sci2exp(intyp)) + "</li>";
        end
        msg = msg + "</ul>";
        msg = msg + "</body></html>";
    end

    hilite_path(path_out, msg);

    // warn the input port block
    if prt_in <> -1 then
        if typ==0 then
            msg = "<html><body>";
            msg = msg + gettext("Block has connected input port<br />with incompatible size:");
            msg = msg + "<ul>";
            msg = msg + "<li>" + msprintf(gettext("Output port %s size is: %s"),string(prt_out), sci2exp(nout)) + "</li>";
            msg = msg + "<li>" + msprintf(gettext("Input port %s size is: %s"),string(prt_in), sci2exp(nin)) + "</li>";
            msg = msg + "</ul>";
            msg = msg + "</body></html>";
        else
            msg = "<html><body>";
            msg = msg + gettext("Block has connected input port<br />with incompatible type:");
            msg = msg + "<ul>";
            msg = msg + "<li>" + msprintf(gettext("Output port %s type is: %s"),string(prt_out), sci2exp(outtyp)) + "</li>";
            msg = msg + "<li>" + msprintf(gettext("Input port %s type is: %s"),string(prt_in), sci2exp(intyp)) + "</li>";
            msg = msg + "</ul>";
            msg = msg + "</body></html>";
        end

        if or(path_in<>path_out) then
            hilite_path(path_in, msg)
        end
    end
endfunction

