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

function model_name=get_model_name(mo_model,Bnames)
    //Copyright INRIA
    //## return an unique name for a modelica model
    //## for the compiled modelica structure
    //##
    //## inputs :
    //##   mo_model : a string that gives the name of the model
    //##              in the modelica list (equations) of a modelica block.
    //##
    //##   Bnames   : vector of strings of already attribuate models name
    //##
    //## output :
    //##   model_name : the output string of the model name
    //##

    ind = 1;
    model_name = strsubst(mo_model," ","") + string(ind);

    while find(model_name==Bnames) <> [] then
        ind = ind + 1;
        model_name=strsubst(mo_model," ","") + string(ind);
    end

endfunction
