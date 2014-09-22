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

function cpr=scicos_cpr(v1,v2,v3,v4,v5,v6,v7,v8,v9,v10)
    //initialisation de cpr scicos_new
    if exists("state","local")==0 then state=scicos_state(),end
    if exists("sim","local")==0 then sim=scicos_sim(),end
    if exists("cor","local")==0 then cor=list(),end
    if exists("corinv","local")==0 then corinv=list(),end
    cpr=scicos_new(["cpr","state","sim","cor","corinv"],..
    state,sim,cor,corinv)
endfunction
