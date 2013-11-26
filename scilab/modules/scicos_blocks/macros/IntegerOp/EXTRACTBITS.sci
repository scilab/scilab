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

function [x,y,typ]=EXTRACTBITS(job,arg1,arg2)
    x=[];
    y=[];
    typ=[];
    select job
    case "set" then
        x=arg1;
        graphics=arg1.graphics;exprs=graphics.exprs
        model=arg1.model;
        while %t do
            [ok,Datatype,rule,bit,scal,exprs]=scicos_getvalue( ..
            [msprintf(gettext("Set %s block parameters"),"EXTRACTBITS");" "; gettext("Bits Extraction");" "; ..
            gettext("&nbsp;- Bits to Extract:"); ..
            gettext("&nbsp;&nbsp;&nbsp;&nbsp;1 Upper Half"); gettext("&nbsp;&nbsp;&nbsp;&nbsp;2 Lower Half"); ..
            gettext("&nbsp;&nbsp;&nbsp;&nbsp;3 Range from MSB"); gettext("&nbsp;&nbsp;&nbsp;&nbsp;4 Range to LSB"); ..
            gettext("&nbsp;&nbsp;&nbsp;&nbsp;5 Range of Bits"); gettext("&nbsp;- Number of Bits or Index of bit : Index 0 is LSB"); ..
            gettext("&nbsp;&nbsp;&nbsp;&nbsp;If ''Bits to Extract'' is set to ''Range of bits'': [Start, End]");" "],..
            [msprintf(gettext("Data Type %s"), "(3:int32, 4:int16, 5:int8, ...)"); gettext("Bits to extract"); ..
            gettext("Number of Bits or Index of Bit"); gettext("Treat Bit Field as an Integer (0:No, 1:Yes)")],..
            list("vec",1,"vec",1,"vec",-1,"vec",1), exprs);

            if ~ok then
                break,
            end
            bitstr = strcat(string(bit(:))," ")
            if (rule < 1) | (rule > 5) then
                block_parameter_error(msprintf(gettext("Wrong value for ''%s'' parameter: %d."), gettext("Bits to Extract"), rule), ..
                msprintf(gettext("Must be in the interval %s."), "[1, 5]"));
                ok = %f;
            elseif scal < 0 | scal > 1 then
                block_parameter_error(msprintf(gettext("Wrong value for ''%s'' parameter: %d."), gettext("Treat Bit Field as an Integer"), scal), ..
                msprintf(gettext("Must be in the interval %s."), "[0, 1]"));
                ok = %f
            else

                in=[model.in model.in2];
                bit=int(bit);
                rule=int(rule);

                if (rule==3)|(rule==4) then
                    if (size(bit,"*") ~= 1) then
                        block_parameter_error(msprintf(gettext("Wrong size for ''%s'' parameter: %s."), gettext("Number of Bits or Index of Bit"), bitstr), ..
                        gettext("Must be a single value."));
                        ok=%f;
                    else
                        numb=bit;
                    end
                elseif (rule==5)
                    if (size(bit,"*") ~= 2) then
                        block_parameter_error(msprintf(gettext("Wrong size for ''%s'' parameter: %s."), gettext("Number of Bits or Index of Bit"), bitstr), ..
                        gettext("Must have this form: [Start, End]."));
                        ok=%f;
                    elseif bit(1) > bit(2) then
                        block_parameter_error(msprintf(gettext("Wrong values for ''%s'' parameter: %s."), gettext("Number of Bits or Index of Bit"), bitstr), ..
                        msprintf(gettext("''Start'' must be less than ''End''.")));
                        ok=%f;
                    else
                        numb=bit(2)-bit(1);
                    end ;
                else
                    bit=0;
                    numb=[]
                end
            end

            if ok then
                if (Datatype==3 | Datatype==6) then

                    if or(bit(:) > 31) | or(bit(:) < 0) then
                        block_parameter_error(msprintf(gettext("Wrong value for ''%s'' parameter: %s."), gettext("Number of Bits or Index of Bit"), bitstr), ..
                        msprintf(gettext("Indexes must be in the interval %s."), "[0, 31]"));
                        ok=%f;
                    end
                    select rule
                    case 1 then
                        select scal
                        case 0 then
                            model.sim=list("extract_bit_32_UH0",4)
                        case 1 then
                            select Datatype
                            case 3 then
                                model.sim=list("extract_bit_32_UH1",4)
                            case 6 then
                                model.sim=list("extract_bit_u32_UH1",4)
                            end
                        end
                    case 2 then
                        model.sim=list("extract_bit_32_LH",4)
                    case 3 then
                        select scal
                        case 0 then
                            model.sim=list("extract_bit_32_MSB0",4)
                        case 1 then
                            select Datatype
                            case 3 then
                                model.sim=list("extract_bit_32_MSB1",4)
                            case 6 then
                                model.sim=list("extract_bit_u32_MSB1",4)
                            end
                        end
                    case 4 then
                        model.sim=list("extract_bit_32_LSB",4)
                    case 5 then
                        select scal
                        case 0 then
                            model.sim=list("extract_bit_32_RB0",4)
                        case 1 then
                            select Datatype
                            case 3 then
                                model.sim=list("extract_bit_32_RB1",4)
                            case 6 then
                                model.sim=list("extract_bit_u32_RB1",4)
                            end
                        end
                    end
                elseif (Datatype==4 | Datatype==7) then
                    if or(bit(:) > 15) | or(bit(:) < 0) then
                        block_parameter_error(msprintf(gettext("Wrong value for ''%s'' parameter: %s."), gettext("Number of Bits or Index of Bit"), bitstr), ..
                        msprintf(gettext("Indexes must be in the interval %s."), "[0, 15]"));
                        ok=%f;
                    end
                    select rule
                    case 1 then
                        select scal
                        case 0 then
                            model.sim=list("extract_bit_16_UH0",4)
                        case 1 then
                            select Datatype
                            case 4 then
                                model.sim=list("extract_bit_16_UH1",4)
                            case 7 then
                                model.sim=list("extract_bit_u16_UH1",4)
                            end
                        end
                    case 2 then
                        model.sim=list("extract_bit_16_LH",4)
                    case 3 then
                        select scal
                        case 0 then
                            model.sim=list("extract_bit_16_MSB0",4)
                        case 1 then
                            select Datatype
                            case 4 then
                                model.sim=list("extract_bit_16_MSB1",4)
                            case 7 then
                                model.sim=list("extract_bit_u16_MSB1",4)
                            end
                        end
                    case 4 then
                        model.sim=list("extract_bit_16_LSB",4)
                    case 5 then
                        select scal
                        case 0 then
                            model.sim=list("extract_bit_16_RB0",4)
                        case 1 then
                            select Datatype
                            case 4 then
                                model.sim=list("extract_bit_16_RB1",4)
                            case 7 then
                                model.sim=list("extract_bit_u16_RB1",4)
                            end
                        end
                    end
                elseif (Datatype == 5 | Datatype == 8) then
                    if or(bit(:) > 7) | or(bit(:) < 0) then
                        block_parameter_error(msprintf(gettext("Wrong value for ''%s'' parameter: %s."), gettext("Number of Bits or Index of Bit"), bitstr), ..
                        msprintf(gettext("Indexes must be in the interval %s."), "[0, 7]"));
                        ok=%f;
                    end
                    select rule
                    case 1 then
                        select scal
                        case 0 then
                            model.sim=list("extract_bit_8_UH0",4)
                        case 1 then
                            select Datatype
                            case 5 then
                                model.sim=list("extract_bit_8_UH1",4)
                            case 8 then
                                model.sim=list("extract_bit_u8_UH1",4)
                            end
                        end
                    case 2 then
                        model.sim=list("extract_bit_8_LH",4)
                    case 3 then
                        select scal
                        case 0 then
                            model.sim=list("extract_bit_8_MSB0",4)
                        case 1 then
                            select Datatype
                            case 5 then
                                model.sim=list("extract_bit_8_MSB1",4)
                            case 8 then
                                model.sim=list("extract_bit_u8_MSB1",4)
                            end
                        end
                    case 4 then
                        model.sim=list("extract_bit_8_LSB",4)
                    case 5 then
                        select scal
                        case 0 then
                            model.sim=list("extract_bit_8_RB0",4)
                        case 1 then
                            select Datatype
                            case 5 then
                                model.sim=list("extract_bit_8_RB1",4)
                            case 8 then
                                model.sim=list("extract_bit_u8_RB1",4)
                            end
                        end
                    end
                else
                    block_parameter_error(msprintf(gettext("Wrong value for ''%s'' parameter: %d."), gettext("Data Type"), Datatype), ..
                    msprintf(gettext("Must be in the interval %s."), "[3, 8]"));
                    ok=%f;
                end
            end

            if ok then
                it=Datatype
                ot=Datatype
                out=[1 1]
                [model,graphics,ok]=set_io(model,graphics,list(in,it),list(out,ot),[],[])
            end
            if ok then
                graphics.exprs=exprs;
                model.ipar=[int(bit(:));int(numb(:))];
                x.graphics=graphics;
                x.model=model;
                break
            end
        end

    case "define" then
        numb=[];
        model=scicos_model()
        model.sim=list("extract_bit_32_UH0",4)
        model.in=1
        model.in2=1
        model.out=1
        model.out2=1;
        model.intyp=3
        model.outtyp=3
        model.ipar=[0,numb]
        model.blocktype="c"
        model.dep_ut=[%t %f]

        exprs=[sci2exp(3);sci2exp(1);sci2exp(0);sci2exp(0)]
        gr_i=[]
        x=standard_define([4 2],model,exprs,gr_i)

    end
endfunction
