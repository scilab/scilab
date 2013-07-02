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

function scs_m_new = do_purge(scs_m)

    // suppress deleted elements in a scicos data structure
    nx = lstsize(scs_m.objs);

    // get index of deleted blocks
    deleted=[];
    for k=1:nx
        typ = typeof(scs_m.objs(k));
        if typ=="Deleted" then
            deleted = [deleted,k];
        elseif typ=="Block" then
            if scs_m.objs(k).model.sim=="super" then
                scs_m.objs(k).model.rpar = do_purge(scs_m.objs(k).model.rpar)
            end
        end

    end

    if deleted==[] then // nothing has to be done
        scs_m_new = scs_m
        return ; //** EXIT point
    end

    retained = 1:nx;
    retained(deleted)=[];

    // compute index cross table
    old_to_new = ones(1,nx);
    old_to_new(deleted) = 0*deleted;
    old_to_new = rtitr(1,%z-1,old_to_new)';

    scs_m_new = scicos_diagram( version=get_scicos_version() );
    scs_m_new.props = scs_m.props
    for k=1:size(retained,"*")
        o = scs_m.objs(retained(k))
        if typeof(o)=="Block" then
            if o.graphics.pin<>[] then
                o.graphics.pin = old_to_new(o.graphics.pin+1);
            end
            if o.graphics.pout<>[] then
                o.graphics.pout = old_to_new(o.graphics.pout+1);
            end
            if o.graphics.pein<>[] then
                o.graphics.pein = old_to_new(o.graphics.pein+1);
            end
            if o.graphics.peout<>[] then
                o.graphics.peout = old_to_new(o.graphics.peout+1);
            end
        elseif typeof(o)=="Link" then
            o.from(1) = old_to_new(o.from(1)+1);
            o.to(1) = old_to_new(o.to(1)+1);
        end
        scs_m_new.objs(k)=o;
    end

endfunction
