// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C)  2016 - INRIA - Serge Steer
//
// This file is licensed under the terms of the GNU GPL v2.0,
// pursuant to article 5.3.4 of the CeCILL v.2.1.
// For more information, see the COPYING file which you should have received
// along with this program.
function r=%r_f_zpk(a,b)
    //  r=[a;b]
    [ma,na]=size(a)
    [mb,nb]=size(b)
    if a.dt<>b.dt then
        error(msprintf(_("%s: time domains are not compatible.\n"),"[a;b]"))
    end
    if na<>nb then
        error(msprintf(_("%s: Incompatible input arguments #%d and #%d: Same number of columns expected.\n"),"[a;b]",1,2))
    end
    r=[zpk(a);b];
endfunction
