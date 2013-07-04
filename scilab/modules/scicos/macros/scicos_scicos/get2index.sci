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

function [scs_m_i, cpr_i, sflag] = get2index(object, k)
    //
    //**               scs_m_i : Object numeber (index) in the diagram (scs_m)
    //**
    //**               cpr_i   : Compiled structure index
    //**
    //** the code is derived from
    //**
    //** "Info" -> GetInfo -> do_block_info -> get_block_info ->  standard_document
    //
    //
    //pause

    sflag=[]
    object_type = typeof(object)
    //
    if object_type=="Block" then

        scs_m_i = k ;
        //disp('ici');pause
        if %cpr<>list()&needcompile<>4&k<>-1 then
            cor = %cpr.cor
            corinv = %cpr.corinv
            path = list()
            for kp=1:size(super_path,"*")
                path(kp) = super_path(kp)
            end
            path($+1) = k
            ind = cor(path)

            //** if ind>0 & ind<=size(corinv) then
            cpr_i = cor(path) ; //** the good one
            //disp('ici');pause
            if type(cpr_i)==15 then
                tmp_cpr=[];
                sflag="*"
                ssflag=""
                for i=1:size(cpr_i)
                    if type(cpr_i(i))<>15 then
                        if cpr_i(i)<>0&cpr_i(i)<=size(corinv) then
                            tmp_cpr=[tmp_cpr,cpr_i(i)]
                        end
                    else //another sblock
                        ssflag=" *"
                    end
                end
                cpr_i="["+strcat(string(tmp_cpr)," ")+ssflag+"]";
            end
            //** else
            //**  cpr_i= -7; //** ['Compiled structure Index   : suppressed'; ' ']
            //**end
        else
            cpr_i= -9; //**   ['Compiled structure Index   : Not available';' ']
        end
    else
        scs_m_i = [] ;
        cpr_i   = [] ;
    end

endfunction
