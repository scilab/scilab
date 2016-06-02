// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C)  2016 - INRIA - Serge Steer
//
// This file is licensed under the terms of the GNU GPL v2.0,
// pursuant to article 5.3.4 of the CeCILL v.2.1.
// For more information, see the COPYING file which you should have received
// along with this program.
function s2=%zpk_i_s(varargin)
    //assignment  s2(i,j)=s1, s2 gain, s1 zpk
    s1=varargin($-1)
    s2=varargin($)
    [ny,nu]=size(s2)
    s2=zpk(cell(ny,nu),cell(ny,nu),s2,s1.dt)
    s2(varargin(1:$-2))=s1
endfunction


