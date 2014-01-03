//  Xcos
//
//  Copyright (C) INRIA - METALAU Project <scicos@inria.fr>
//  Copyright 2011 - Bernard DUJARDIN <bernard.dujardin@contrib.scilab.org>
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

function [x,y,typ]=SHIFT(job,arg1,arg2)
    x=[];
    y=[];
    typ=[];
    select job
    case "set" then
        x=arg1;
        graphics=arg1.graphics
        model=arg1.model
        exprs=graphics.exprs
        while %t do
            [ok,Datatype,nb,np,exprs]=scicos_getvalue([msprintf(gettext("Set %s block parameters"), "SHIFT");" "; gettext("Shift/Rotates bits")], ..
            [msprintf(gettext("Data Type %s"), "(3:int32, 4:int16, 5:int8, ...)"); gettext("Number of Bits to Shift Left (Negative number to shift right)"); ..
            gettext("Shift Type (0:Arithmetic, 1:Circular)")], ..
            list("vec",1,"vec",1,"vec",1), exprs);

            if ~ok then
                break,
            end
            if (np ~= 0 & np ~= 1) then
                block_parameter_error(msprintf( gettext("Wrong value for ''%s'' parameter: %d."), gettext("Shift Type"), np), ..
                msprintf(gettext("Must be in the interval %s."), "[0, 1]"));
                ok=%f;
            end
            it=Datatype;
            ot=Datatype;
            //    model.sim=list('shift_ia',4)
            if (Datatype==3 | Datatype==6) then
                if nb>0 then
                    select np
                    case 0 then
                        model.sim=list("shift_32_LA",4)
                    case 1 then
                        model.sim=list("shift_32_LC",4)
                    end
                elseif nb<0
                    select np
                    case 0 then
                        select Datatype
                        case 3 then
                            model.sim=list("shift_32_RA",4)
                        case 6 then
                            model.sim=list("shift_u32_RA",4)
                        end
                    case 1 then
                        model.sim=list("shift_32_RC",4)
                    end
                end
            elseif (Datatype==4 | Datatype==7) then
                if nb>0 then
                    select np
                    case 0 then
                        model.sim=list("shift_16_LA",4)
                    case 1 then
                        model.sim=list("shift_16_LC",4)
                    end
                elseif nb<0
                    select np
                    case 0 then
                        select Datatype
                        case 4 then
                            model.sim=list("shift_16_RA",4)
                        case 7 then
                            model.sim=list("shift_u16_RA",4)
                        end
                    case 1 then
                        model.sim=list("shift_16_RC",4)
                    end
                end
            elseif (Datatype==5 | Datatype==8) then
                if nb>0 then
                    select np
                    case 0 then
                        model.sim=list("shift_8_LA",4)
                    case 1 then
                        model.sim=list("shift_8_LC",4)
                    end
                elseif nb<0
                    select np
                    case 0 then
                        select Datatype
                        case 5 then
                            model.sim=list("shift_8_RA",4)
                        case 8 then
                            model.sim=list("shift_u8_RA",4)
                        end
                    case 1 then
                        model.sim=list("shift_8_RC",4)
                    end
                end
            else
                block_parameter_error(msprintf( gettext( "Wrong value for ''%s'' parameter: %d."), gettext("Data Type"), Datatype), ..
                msprintf(gettext( "Must be in the interval %s."), "[3, 8]" ));
                ok=%f;
            end
            if ok then
                [model,graphics,ok]=set_io(model,graphics,...
                list([-1,-2],it),...
                list([-1,-2],ot),[],[])
            end
            if ok then
                model.ipar=nb
                graphics.exprs=exprs
                x.graphics=graphics;
                x.model=model
                break
            end
        end
    case "define" then
        sgn=[0;0]
        OPER=0;
        model=scicos_model()
        model.sim=list("shift_32_LA",4)
        model.in=-1
        model.out=-1
        model.in2=-2
        model.out2=-2
        model.intyp=3
        model.outtyp=3
        model.rpar=[]
        model.ipar=sgn
        model.blocktype="c"
        model.dep_ut=[%t %f]


        exprs=[sci2exp(3);sci2exp(0);sci2exp(0)]
        gr_i=[]
        x=standard_define([3 2],model, exprs,gr_i)
    end
endfunction
