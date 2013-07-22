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
function [ierr,scicos_ver,scs_m_out]=update_version(scs_m_in)

    //## contrib update

    //## scicos update
    current_version = get_scicos_version()
    scicos_ver = find_scicos_version(scs_m_in)

    if scicos_ver<>current_version then
        txt_to_run = "scs_m_out=do_version(scs_m_in,scicos_ver)"
        ierr=execstr(txt_to_run,"errcatch")
        if ierr<>0 then
            scs_m_out=scs_m_in
        end
    else
        scs_m_out=scs_m_in
        ierr=0
    end

endfunction