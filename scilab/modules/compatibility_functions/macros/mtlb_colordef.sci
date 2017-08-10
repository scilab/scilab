// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2002-2004 - INRIA - Vincent COUVERT
//
// Copyright (C) 2012 - 2016 - Scilab Enterprises
//
// This file is hereby licensed under the terms of the GNU GPL v2.0,
// pursuant to article 5.3.4 of the CeCILL v.2.1.
// This file was originally licensed under the terms of the CeCILL v2.1,
// and continues to be available under such terms.
// For more information, see the COPYING file which you should have received
// along with this program.

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


