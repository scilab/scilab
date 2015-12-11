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

function [x,y,typ]=CONVERT(job,arg1,arg2)
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
            [ok,it,ot,np,exprs] = scicos_getvalue([msprintf(gettext("Set %s block parameters"), "CONVERT");" "; gettext("Type conversion");" "], ..
            [gettext("Input Type (1:double, 3:int32, 4:int16, 5:int8, ...)"); gettext("Output Type (1:double, 3:int32, 4:int16, 5:int8, ...)"); ..
            gettext("Do on Overflow (0:Nothing, 1:Saturate, 2:Error)")], ..
            list("vec",1,"vec",1,"vec",1), exprs);
            if ~ok then
                break,
            end

            if it==2 then
                it =1;
            end
            if ot==2 then
                ot=1;
            end

            if (np ~= 0 & np ~= 1 & np ~= 2) then
                block_parameter_error(msprintf(gettext("Wrong value for ''%s'' parameter: %d."), gettext("Do on Overflow"), np), ..
                msprintf(gettext("Must be in the interval %s."), "[0, 2]"));
                ok=%f;
            elseif (it > 8 | it < 1) then
                block_parameter_error(msprintf(gettext("Wrong value for ''%s'' parameter: %d."), gettext("Input Type"), it), ..
                msprintf(gettext("Must be in the interval %s."), "[1, 8]"));
                ok=%f;
            elseif (ot > 8 | ot < 1) then
                block_parameter_error(msprintf(gettext("Wrong value for ''%s'' parameter: %d."), gettext("Output Type"), ot), ..
                msprintf(gettext("Must be in the interval %s."), "[1, 8]"));
                ok=%f;
            end

            model.sim=list("convert",4)
            if (it==ot) then
                model.ipar=1;
            else
                if (np==0) then
                    if (it==1) then
                        if (ot==3) then
                            model.ipar=2;
                        elseif (ot==4) then
                            model.ipar=3;
                        elseif (ot==5) then
                            model.ipar=4;
                        elseif (ot==6) then
                            model.ipar=5;
                        elseif (ot==7) then
                            model.ipar=6;
                        elseif (ot==8) then
                            model.ipar=7;
                        end
                    elseif (it==3) then
                        if (ot==1) then
                            model.ipar=8;
                        elseif (ot==4) then
                            model.ipar=9;
                        elseif (ot==5) then
                            model.ipar=10;
                        elseif (ot==6) then
                            model.ipar=1;
                        elseif (ot==7) then
                            model.ipar=11;
                        elseif (ot==8) then
                            model.ipar=12;
                        end
                    elseif (it==4) then
                        if (ot==1) then
                            model.ipar=13;
                        elseif (ot==3) then
                            model.ipar=14;
                        elseif (ot==5) then
                            model.ipar=15;
                        elseif (ot==6) then
                            model.ipar=16;
                        elseif (ot==7) then
                            model.ipar=1;
                        elseif (ot==8) then
                            model.ipar=17;
                        end
                    elseif (it==5) then
                        if (ot==1) then
                            model.ipar=18;
                        elseif (ot==3) then
                            model.ipar=19;
                        elseif (ot==4) then
                            model.ipar=20;
                        elseif (ot==6) then
                            model.ipar=21;
                        elseif (ot==7) then
                            model.ipar=22;
                        elseif (ot==8) then
                            model.ipar=1;
                        end
                    elseif (it==6) then
                        if (ot==1) then
                            model.ipar=23;
                        elseif (ot==3) then
                            model.ipar=1;
                        elseif (ot==4) then
                            model.ipar=24;
                        elseif (ot==5) then
                            model.ipar=25;
                        elseif (ot==7) then
                            model.ipar=26;
                        elseif (ot==8) then
                            model.ipar=27;
                        end
                    elseif (it==7) then
                        if (ot==1) then
                            model.ipar=28;
                        elseif (ot==3) then
                            model.ipar=29;
                        elseif (ot==4) then
                            model.ipar=1;
                        elseif (ot==5) then
                            model.ipar=30;
                        elseif (ot==6) then
                            model.ipar=31;
                        elseif (ot==8) then
                            model.ipar=32;
                        end
                    elseif (it==8) then
                        if (ot==1) then
                            model.ipar=33;
                        elseif (ot==3) then
                            model.ipar=34;
                        elseif (ot==4) then
                            model.ipar=35;
                        elseif (ot==5) then
                            model.ipar=1;
                        elseif (ot==6) then
                            model.ipar=36;
                        elseif (ot==7) then
                            model.ipar=37;
                        end
                    end
                elseif (np==1) then
                    if (it==1) then
                        if (ot==3) then
                            model.ipar=38;
                        elseif (ot==4) then
                            model.ipar=39;
                        elseif (ot==5) then
                            model.ipar=40;
                        elseif (ot==6) then
                            model.ipar=41;
                        elseif (ot==7) then
                            model.ipar=42;
                        elseif (ot==8) then
                            model.ipar=43;
                        end
                    elseif (it==3) then
                        if (ot==1) then
                            model.ipar=8;
                        elseif (ot==4) then
                            model.ipar=44;
                        elseif (ot==5) then
                            model.ipar=45;
                        elseif (ot==6) then
                            model.ipar=46;
                        elseif (ot==7) then
                            model.ipar=47;
                        elseif (ot==8) then
                            model.ipar=48;
                        end
                    elseif (it==4) then
                        if (ot==1) then
                            model.ipar=13;
                        elseif (ot==3) then
                            model.ipar=14;
                        elseif (ot==5) then
                            model.ipar=49;
                        elseif (ot==6) then
                            model.ipar=50;
                        elseif (ot==7) then
                            model.ipar=51;
                        elseif (ot==8) then
                            model.ipar=52;
                        end
                    elseif (it==5) then
                        if (ot==1) then
                            model.ipar=18;
                        elseif (ot==3) then
                            model.ipar=19;
                        elseif (ot==4) then
                            model.ipar=20;
                        elseif (ot==6) then
                            model.ipar=53;
                        elseif (ot==7) then
                            model.ipar=54;
                        elseif (ot==8) then
                            model.ipar=55;
                        end
                    elseif (it==6) then
                        if (ot==1) then
                            model.ipar=23;
                        elseif (ot==3) then
                            model.ipar=56;
                        elseif (ot==4) then
                            model.ipar=57;
                        elseif (ot==5) then
                            model.ipar=58;
                        elseif (ot==7) then
                            model.ipar=59;
                        elseif (ot==8) then
                            model.ipar=60;
                        end
                    elseif (it==7) then
                        if (ot==1) then
                            model.ipar=28;
                        elseif (ot==3) then
                            model.ipar=29;
                        elseif (ot==4) then
                            model.ipar=61;
                        elseif (ot==5) then
                            model.ipar=62;
                        elseif (ot==6) then
                            model.ipar=31;
                        elseif (ot==8) then
                            model.ipar=63;
                        end
                    elseif (it==8) then
                        if (ot==1) then
                            model.ipar=33;
                        elseif (ot==3) then
                            model.ipar=34;
                        elseif (ot==4) then
                            model.ipar=35;
                        elseif (ot==5) then
                            model.ipar=64;
                        elseif (ot==6) then
                            model.ipar=36;
                        elseif (ot==7) then
                            model.ipar=37;
                        end
                    end
                elseif (np==2) then
                    if (it==1) then
                        if (ot==3) then
                            model.ipar=65;
                        elseif (ot==4) then
                            model.ipar=66;
                        elseif (ot==5) then
                            model.ipar=67;
                        elseif (ot==6) then
                            model.ipar=68;
                        elseif (ot==7) then
                            model.ipar=69;
                        elseif (ot==8) then
                            model.ipar=70;
                        end
                    elseif (it==3) then
                        if (ot==1) then
                            model.ipar=8;
                        elseif (ot==4) then
                            model.ipar=71;
                        elseif (ot==5) then
                            model.ipar=72;
                        elseif (ot==6) then
                            model.ipar=73;
                        elseif (ot==7) then
                            model.ipar=74;
                        elseif (ot==8) then
                            model.ipar=75;
                        end
                    elseif (it==4) then
                        if (ot==1) then
                            model.ipar=13;
                        elseif (ot==3) then
                            model.ipar=14;
                        elseif (ot==5) then
                            model.ipar=76;
                        elseif (ot==6) then
                            model.ipar=77;
                        elseif (ot==7) then
                            model.ipar=78;
                        elseif (ot==8) then
                            model.ipar=79;
                        end
                    elseif (it==5) then
                        if (ot==1) then
                            model.ipar=18;
                        elseif (ot==3) then
                            model.ipar=19;
                        elseif (ot==4) then
                            model.ipar=20;
                        elseif (ot==6) then
                            model.ipar=80;
                        elseif (ot==7) then
                            model.ipar=81;
                        elseif (ot==8) then
                            model.ipar=82;
                        end
                    elseif (it==6) then
                        if (ot==1) then
                            model.ipar=23;
                        elseif (ot==3) then
                            model.ipar=83;
                        elseif (ot==4) then
                            model.ipar=84;
                        elseif (ot==5) then
                            model.ipar=85;
                        elseif (ot==7) then
                            model.ipar=86;
                        elseif (ot==8) then
                            model.ipar=87;
                        end
                    elseif (it==7) then
                        if (ot==1) then
                            model.ipar=28;
                        elseif (ot==3) then
                            model.ipar=29;
                        elseif (ot==4) then
                            model.ipar=88;
                        elseif (ot==5) then
                            model.ipar=89;
                        elseif (ot==6) then
                            model.ipar=31;
                        elseif (ot==8) then
                            model.ipar=90;
                        end
                    elseif (it==8) then
                        if (ot==1) then
                            model.ipar=33;
                        elseif (ot==3) then
                            model.ipar=34;
                        elseif (ot==4) then
                            model.ipar=35;
                        elseif (ot==5) then
                            model.ipar=91;
                        elseif (ot==6) then
                            model.ipar=36;
                        elseif (ot==7) then
                            model.ipar=37;
                        end
                    end
                end
            end
            in=[model.in model.in2]
            out=[model.out model.out2]
            if ok then
                [model,graphics,ok]=set_io(model,graphics,...
                list(in,it),...
                list(out,ot),[],[])
            end
            if ok then
                graphics.exprs=exprs
                x.graphics=graphics;
                x.model=model
                break
            end
        end
    case "define" then

        sgn=2
        model=scicos_model()
        model.sim=list("convert",4)
        model.in=-1
        model.out=-1
        model.in2=-2
        model.out2=-2
        model.intyp=1
        model.outtyp=3
        model.rpar=[]
        model.ipar=sgn
        model.blocktype="c"
        model.dep_ut=[%t %f]


        exprs=[sci2exp(1);sci2exp(3);sci2exp(0)]
        gr_i=[]
        x=standard_define([3 2],model, exprs,gr_i)
    end
endfunction
