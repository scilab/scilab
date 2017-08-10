// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C)  2016 - INRIA - Serge Steer
//
// This file is licensed under the terms of the GNU GPL v2.0,
// pursuant to article 5.3.4 of the CeCILL v.2.1.
// For more information, see the COPYING file which you should have received
// along with this program.
function s2=%zpk_i_zpk(varargin)
    //assignment  s2(i,j)=s1
    s1=varargin($-1)
    s2=varargin($)
    if s1.dt<>s2.dt then
        error(msprintf(_("%s: time domains are not compatible.\n"),"a(i,j)=b"))
    end
    s2.K(varargin(1:$-2))=s1.K
    s2.Z(varargin(1:$-2))=s1.Z
    s2.P(varargin(1:$-2))=s1.P
endfunction


