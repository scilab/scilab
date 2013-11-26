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

function [x,y,typ]=MATZCONJ(job,arg1,arg2)
    //
    x=[];
    y=[];
    typ=[];
    select job
    case "set" then
        x=arg1
    case "define" then
        model=scicos_model()
        function_name="matz_conj";
        funtyp=4;
        model.sim=list(function_name,funtyp)
        model.in=-1
        model.in2=-2
        model.intyp=2
        model.out=-1
        model.out2=-2
        model.outtyp=2
        model.evtin=[]
        model.evtout=[]
        model.state=[]
        model.dstate=[]
        model.rpar=[]
        model.ipar=[]
        model.blocktype="c"
        model.firing=[]
        model.dep_ut=[%t %f]
        label=[];
        gr_i=[]
        x=standard_define([2 2],model,label,gr_i)
    end
endfunction

