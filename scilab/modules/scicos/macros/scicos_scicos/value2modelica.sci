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

function txt=value2modelica(v)
    [m,n]=size(v)
    if m==1&n==1 then
        txt=stripblanks(sci2exp(v))
        kdot=strindex(txt,".")
        if kdot==[] then return,end
        if kdot==1 then
            txt="0"+txt
        else
            if or(part(txt,kdot-1)==["+","-"]) then
                txt=part(txt,1:kdot-1)+"0"+part(txt,kdot:length(txt))
            end
        end
    else //matrix case
        txt=stripblanks(sci2exp(v,0))
        kdot=strindex(txt,".")
        if kdot==[] then return,end
        k=strindex(part(txt,kdot-1),["+","-",",",";","["])
        if k==[] then return,end
        t=txt;txt="";k0=1;
        for i=k
            ki=kdot(i);
            txt=txt+part(t,k0:ki-1)+"0";
            k0=ki;
        end
        txt=txt+part(t,k0:length(t))
    end
endfunction
