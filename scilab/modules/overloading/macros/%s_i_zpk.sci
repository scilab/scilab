// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C)  2016 - INRIA - Serge Steer
//
// This file is licensed under the terms of the GNU GPL v2.0,
// pursuant to article 5.3.4 of the CeCILL v.2.1.
// and continues to be available under such terms.
// For more information, see the COPYING file which you should have received
// along with this program.
function s2=%s_i_zpk(varargin)
    //assignment  s2(i,j)=s1  s1 scalar
    s2=varargin($)
    s1=varargin($-1)
    if s1==[] then
        s2.K(varargin(1:$-2))=[]
        if s2.K==[] then
            S2=[]
        else
            s2.Z(varargin(1:$-2))=[]
            s2.P(varargin(1:$-2))=[]
        end
    else
        [ny,nu]=size(s1)
        s1=zpk(cell(ny,nu),cell(ny,nu),s1,s2.dt)
        s2(varargin(1:$-2))=s1
    end
endfunction

