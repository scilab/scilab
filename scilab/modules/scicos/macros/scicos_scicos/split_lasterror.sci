//  Scicos
//
//  Copyright (C) INRIA - Alan Layec <alan.layec@inria.fr>
//  Copyright (C) 2012-2012 - Scilab Enterprises - Bruno JOFRET
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

function str_out=split_lasterror(str_in)
    //** Fix the unreadable error messages
    //** returned by the simulator.
    //** Alan, 11/10/07 : Initial rev
    str_out = [];
    for nLine = 1:size(str_in, "r")
        ind_bl=strindex(str_in(nLine)," ");

        // Find some room in a line with more than 50 characters.
        if find(ind_bl > 50) <> [] then
            ind_bl2 = ind_bl;
            nind = [];
            for i = 1:size(ind_bl,2)
                if ind_bl2(i) > 50 then
                    nind = [nind ; ind_bl(i)];
                    ind_bl2 = ind_bl2 - ind_bl2(i);
                end
            end
            str_out = [str_out ; strsplit(str_in(nLine),nind)]
        else
            str_out = [str_out ; str_in(nLine)];
        end

    end
endfunction
