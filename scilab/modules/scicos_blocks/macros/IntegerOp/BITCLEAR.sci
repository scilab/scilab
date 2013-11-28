//  Xcos
//
//  Copyright (C) INRIA - METALAU Project <scicos@inria.fr>
//  Copyright (C) 2011 - Bernard DUJARDIN <bernard.dujardin@contrib.scilab.org>
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

function [x,y,typ]=BITCLEAR(job,arg1,arg2)
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

            [ok,Datatype,bit,exprs]=scicos_getvalue([msprintf(gettext("Set %s block parameters"), "BITCLEAR"); " ";          gettext("Clear a bit");" "],..
            [msprintf(gettext("Data Type %s"), "(3:int32, 4:int16, 5:int8, ...)"); gettext("Index of Bit (0 is least significant)")],..
            list("vec",1,"vec",1), exprs);

            if ~ok then
                break,
            end
            in=[model.in model.in2];

            if floor(bit) <> bit then
                block_parameter_error(msprintf(gettext("Wrong type for ''%s'' parameter: %5.1f."), gettext("Index of Bit"), bit), ..
                gettext("Must be integer.") ..
                );
                ok=%f;
            elseif (Datatype == 3) | (Datatype == 6) then
                if bit > 31 | bit < 0 then
                    block_parameter_error(msprintf(gettext("Wrong value for ''%s'' parameter: %d."), gettext("Index of Bit"), bit), ..
                    msprintf(gettext("Must be in the interval %s."), "[0, 31]"));
                    ok=%f;
                else
                    bit=uint32(bit)
                    n=(2^32-1)-2^bit;
                    n=uint32(n)
                    model.sim=list("bit_clear_32",4)
                end
            elseif (Datatype == 4)|(Datatype == 7) then
                if bit > 15 | bit < 0 then
                    block_parameter_error(msprintf(gettext("Wrong value for ''%s'' parameter: %d."), gettext("Index of Bit"), bit), ..
                    msprintf(gettext("Must be in the interval %s."), "[0, 15]"));
                    ok=%f;
                else
                    bit=uint16(bit)
                    n=(2^16-1)-2^bit;
                    n=uint16(n)
                    model.sim=list("bit_clear_16",4)
                end
            elseif (Datatype == 5)|(Datatype == 8) then
                if bit > 7 | bit < 0 then
                    block_parameter_error(msprintf(gettext("Wrong value for ''%s'' parameter: %d."), gettext("Index of Bit"), bit), ..
                    msprintf(gettext("Must be in the interval %s."), "[0, 7]"));
                    ok=%f;
                else
                    bit=uint8(bit)
                    n=(2^8-1)-2^bit;
                    n=uint8(n)
                    model.sim=list("bit_clear_8",4)
                end
            else
                block_parameter_error(msprintf(gettext( "Wrong value for ''%s'' parameter: %d."), gettext("Data Type"), Datatype),..
                msprintf(gettext( "Must be in the interval %s."), "[3, 8]"));
                ok=%f;
            end
            if ok then
                it=Datatype
                ot=Datatype
                out=[1 1]
                [model,graphics,ok]=set_io(model,graphics,list(in,it),list(out,ot),[],[])
            end
            if ok then
                graphics.exprs=exprs;
                model.opar=list(n)
                x.graphics=graphics;
                x.model=model;
                break
            end
        end

    case "define" then
        model=scicos_model()
        model.sim=list("bit_clear_32",4)
        model.in=1
        model.in2=1
        model.out=1
        model.out2=1;
        model.intyp=3
        model.outtyp=3
        model.opar=list(int32(0))
        model.blocktype="c"
        model.dep_ut=[%t %f]

        exprs=[sci2exp(3);sci2exp(0)]
        gr_i=[]
        x=standard_define([4 2],model,exprs,gr_i)

    end
endfunction
