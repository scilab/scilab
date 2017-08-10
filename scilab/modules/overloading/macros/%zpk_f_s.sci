// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C)  2016 - INRIA - Serge Steer
//
// This file is licensed under the terms of the GNU GPL v2.0,
// pursuant to article 5.3.4 of the CeCILL v.2.1.
// For more information, see the COPYING file which you should have received
// along with this program.
function a=%zpk_f_s(a,b)
    if b==[] then return;end
    [mb,nb]=size(b)
    if size(a,2)<>nb then
        error(msprintf(_("%s: Incompatible input arguments #%d and #%d: Same number of rows expected.\n"),"[]",1,2))
    end

    a=[a;zpk(cell(mb,nb),cell(mb,nb),b,a.dt)]


endfunction
