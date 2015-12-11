// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2002-2004 - INRIA - Vincent COUVERT
//
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt

function varargout=mtlb_colordef(varargin)
    // Emulation function for Matlab colordef()

    lhs=argn(1);

    if lstsize(varargin)==1 then
        color_option=varargin(1)
        fig=[]

        f=gcf()
        if or(color_option==["black","none"]) then
            f.background=-1
        else
            f.background=-2
        end
        if lhs==1 then
            varargout(1)=f
        end
    else
        color_option=varargin(2)
        fig=varargin(1)

        if type(fig)==10 then //'new'
            fig=scf(max(winsid())+1);
        end

        if or(color_option==["black","none"]) then
            fig.background=-1
        else
            fig.background=-2
        end
        if lhs==1 then
            varargout(1)=fig
        end

    end


endfunction


