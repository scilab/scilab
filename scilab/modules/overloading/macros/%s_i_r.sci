// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) INRIA
//
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt

function s2=%s_i_r(varargin)
    //author Serge Steer INRIA
    s1=varargin($-1)
    s2=varargin($)
    //s2(i,j)=s1 s1 matrix of scalar
    if s1==[] then
        num=s2.num;den=s2.den;
        num(varargin(1:$-2))=[];
        den(varargin(1:$-2))=[];
        s2=rlist(num,den,s2.dt)
    else
        num=s2.num;
        ind=list(); for i=size(num),ind($+1)=1:i;end
        num(varargin(1:$-2))=s1
        den=ones(num)
        den(ind(:))=s2.den;
        den(varargin(1:$-2))=1
        s2=rlist(num,den,s2.dt)
    end
endfunction
