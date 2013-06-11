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

function [ok,mac,txt]=genfunc(txt)
    [lhs,rhs]=argn(0)
    if rhs<1 then txt=" ",end
    mac=[]
    while %t do
        txt=dialog(["Set Function Block";
        " "
        "Enter Scilab instructions defining"
        "y as a function of u"],txt)


        if txt==[] then ok=%f,return,end
        // check if txt defines y from u
        deff("[]=mac()",txt)
        vars=macrovar(mac)
        if or(vars(3)=="u")&or(vars(5)=="y") then break,end
    end
    ok=%t
    deff("[%_1,%_2]=mac(%_model,%_x,%_z,u,%_clock,%_flag,%_rpar,%_ipar)",..
    ["%_1=[];%_2=[];";
    "select %_flag";
    "case 1 then";
    txt
    "%_1=y";
    "case -1 then ";
    "  %_model=list(%_model(1),1,1,[],[],[],[],[],[],''c'',%f,[%t %f])";
    "  %_1=list(%_model,'' '')";
    "case -2 then ";
    "  txt=%_model.ipar";
    "  [ok,mac,txt]=genfunc(txt)";
    "  if ok then "
    "    %_model.sim=mac"
    "    %_model.ipar=txt"
    "    %_1=list(model,%_x)"
    "  end"
    "end"])
endfunction
