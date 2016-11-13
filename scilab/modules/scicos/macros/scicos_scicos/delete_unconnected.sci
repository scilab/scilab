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

function scs_m=delete_unconnected(scs_m);

    n = lstsize(scs_m.objs);

    if n==0 then
        return
    end ; //** exit point

    DEL=[];
    DELL=[]
    finish=%f

    while ~finish
        finish = %t
        for k=1:n  //loop on scs_m objects
            x = getfield(1,scs_m.objs(k))
            if x(1)=="Block" then
                if scs_m.objs(k).gui<>"SUM_f"&scs_m.objs(k).gui<>"SOM_f" then
                    if find(scs_m.objs(k).gui==["IFTHEL_f","ESELECT_f"]) then
                        kk=[find(scs_m.objs(k).graphics.pein==0),find(scs_m.objs(k).graphics.pin==0)]
                        if kk<> [] // a synchro block is not active, remove it
                            [scs_m,DEL1,DELL1]=do_delete1(scs_m,k,%f)
                            DEL=[DEL DEL1]
                            DELL=[DELL DELL1]
                            finish=%f
                        end
                    else
                        kk=[find(scs_m.objs(k).graphics.pin==0)]
                        if kk<>[] then // at least one  input port is not connected delete the block
                            if or(getfield(1,scs_m.objs(k).graphics)=="in_implicit") then
                                if or(scs_m.objs(k).graphics.in_implicit(kk)<>"I") then
                                    [scs_m,DEL1,DELL1]=do_delete1(scs_m,k,%f)
                                    DEL=[DEL DEL1]
                                    DELL=[DELL DELL1]
                                    finish=%f
                                end
                            else
                                [scs_m,DEL1,DELL1]=do_delete1(scs_m,k,%f)
                                DEL=[DEL DEL1]
                                DELL=[DELL DELL1]
                                finish=%f
                            end
                        end
                    end
                end
            end
        end
    end

    //suppress right-most deleted elements
    while getfield(1,scs_m.objs($))=="Deleted" then
        scs_m.objs($)=null();
        if lstsize(scs_m.objs)==0 then
            break
        end
    end

    // Notify by hiliting and message
    if DEL<>[] then
        // ind_del are deleted and not relinked blocks/links indexes of DEL
        if DELL <> [] then
            deleted = DEL(find(DEL<>DELL));
        else
            deleted = DEL;
        end

        // Store updated scs_m and retrieve the previous one
        scs_save = scs_m;
        scs_m = scs_m_s;

        msg = gettext("Block is ignored because of undefined input(s).");
        for del=deleted
            hilite_path([path del], msg);
        end

        // Restore the updated scs_m
        scs_m = scs_save;
    end
endfunction

