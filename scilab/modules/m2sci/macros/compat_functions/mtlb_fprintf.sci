// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2002-2004 - INRIA - Vincent COUVERT
// Copyright (C) ???? - INRIA - Serge STEER
//
// Copyright (C) 2012 - 2016 - Scilab Enterprises
//
// This file is hereby licensed under the terms of the GNU GPL v2.0,
// pursuant to article 5.3.4 of the CeCILL v.2.1.
// This file was originally licensed under the terms of the CeCILL v2.1,
// and continues to be available under such terms.
// For more information, see the COPYING file which you should have received
// along with this program.

function count = mtlb_fprintf(varargin)
    // Emulation function for fprintf() Matlab function

    [lhs,rhs]=argn()

    count=0
    // No fid given: mtlb_fprintf(fmt,...)
    // Output is the screen

    if size(varargin)==1 & type(varargin(1))==10 then
        mprintf("%s \n",varargin(1))
        count=length(varargin(1))
    elseif type(varargin(1))==10 then
        fmt=varargin(1)
        nfmt=size(strindex(fmt,"%"),"*")
        nv=size(varargin)-1
        if nv==1 then
            a=varargin(2)
            na=size(a,"*")

            // If more values than formats
            mult=max(na/nfmt,1)
            fmt=strcat(fmt(ones(1,mult))) // duplicate format

            l=list()
            A=a
            for k=1:size(a,"*")
                l($+1)=A(k)
            end
            mprintf(fmt,l(:))
            count=size(a,"*")
        elseif nv==0 then
            error(msprintf(gettext("%s: %s is not implemented.\n"),"mtlb_fprintf","mprintf("+fmt+")"));
        else
            sz=[]
            for k=1:nv
                sz=[sz size(varargin(k+1),1)]
            end
            // Size of args is verified because Scilab mprintf function
            // does not work if args have more than one row
            if and(sz==1) then // All args have only one row
                mult=max(nv/nfmt,1)
                fmt=strcat(fmt(ones(1,mult))) // duplicate format
                mprintf(fmt,varargin(2:$))
                count=size(sz,"*")
            else
                error(msprintf(gettext("%s: %s Scilab function does not work with more than one row variables.\n"),"mtlb_fprintf","mprintf"))
            end
        end
        // mtlb_fprintf(fid,fmt,...)
    else
        fid=varargin(1)
        fmt=varargin(2)

        // count % in fmt
        nfmt=size(strindex(fmt,"%"),"*")
        nv=size(varargin)-2
        if nv==1 then
            a=varargin(3)
            na=size(a,"*")

            mult=max(na/nfmt,1)

            fmt=strcat(fmt(ones(1,mult))) // duplicate format

            l=list()
            for k=1:size(a,"*")
                l(k)=a(k)
            end
            if or(fid==[1 2]) then
                mprintf(fmt,l(:))
                count=size(a,"*")
            else
                mfprintf(fid,fmt,l(:))
                count=size(a,"*")
            end
        elseif nv==0 & nfmt==0 & type(varargin(2))==10
            mfprintf(fid,"%s",varargin(2))
            count=length(varargin(2))
        elseif nv==0 then
            if or(fid==[1 2]) then
                error(msprintf(gettext("%s: %s is not implemented.\n"),"mtlb_fprintf", "mprintf(format)"))
            else
                error(msprintf(gettext("%s: %s is not implemented.\n"),"mtlb_fprintf","mfprintf(fid,format)"))
            end
        else
            sz=[]
            for k=1:nv
                sz=[sz size(varargin(k+2),1)]
            end
            if and(sz==1) then
                mult=max(nv/nfmt,1)
                fmt=strcat(fmt(ones(1,mult))) // duplicate format

                if or(fid==[1 2]) then
                    mprintf(fmt,varargin(3:$))
                    count=size(sz,"*")
                else
                    mfprintf(fid,fmt,varargin(3:$))
                    count=size(sz,"*")
                end
            else
                if or(fid==[1 2]) then
                    error(msprintf(gettext("%s: %s Scilab function does not work with more than one row variables.\n"),"mtlb_fprintf", "mprintf"))
                else
                    fprintfMat(fid,varargin(3:$),fmt)
                    for k=1:nv
                        count=count+size(varargin(k+2),"*")
                    end
                end
            end
        end
    end
endfunction
