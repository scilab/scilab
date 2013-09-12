// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) ???? - INRIA - Scilab
//
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt

function h=mtlb_axes(varargin)
    global AXES_T
    if AXES_T==[] then AXES_T=list([]),end
    win=xget("window")
    k=find(AXES_T(1)==win)
    if k==[] then
        AXES_T(1)=[AXES_T(1);win]
        AXES_T($+1)=[]
        k=size(AXES_T(1),1)+1
    else
        k=k+1
    end
    if size(varargin)==0 then
        rect=[0 0 1 1]
        xsetech([0 0 1 1])
        AXES_T(k)=[AXES_T(k);rect];h=size(AXES_T(k),1)
    elseif varargin(1)=="position"
        rect=matrix(varargin(2),1,-1)
        xsetech(rect)
        AXES_T(k)=[AXES_T(k);rect];h=size(AXES_T(k),1)
    elseif type(varargin(1))==1 then
        xsetech(AXES_T(k)(varargin(1),:))
    else
        error(msprintf(gettext("%s: This feature has not been implemented.\n"),"mtlb_axes"));
    end


endfunction
