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

function [L]=mtlb_dir(path)
    // Emulation function for dir() Matlab function
    // See SCI/modules/fileio/macros/dir.sci for details

    // opt=='disp' -> result not affected to a variable
    // opt=='var' -> result affected to a variable

    rhs=argn(2)

    mask = int32(61440)
    dirtype = 16384
    basepath=""

    if rhs==1 then
        files=listfiles(path)
        basepath=path
    else
        files=listfiles()
    end

    n=size(files,"*")
    if n<>0 then
        files=gsort(files,"g","i")
    end

    lfiles=list()
    ldate=list()
    lbytes=list()
    lisdir=list()
    for k=1:n
        lfiles(k)=files(k)
        [x,ierr]=fileinfo(basepath+"/"+files(k))
        if ( (x == []) & (ierr== -1) ) then [x,ierr]=fileinfo(files(k)),end
        if x<>[] then
            w=getdate(x(6))
            month=[gettext("Jan"),..
            gettext("Feb"),..
            gettext("Mar"),..
            gettext("Apr"),..
            gettext("May"),..
            gettext("Jun"),..
            gettext("Jul"),..
            gettext("Aug"),..
            gettext("Sep"),..
            gettext("Oct"),..
            gettext("Nov"),..
            gettext("Dec")];

            ldate(k)=string(w(6))+"-"+month(w(2))+"-"+string(w(1))+" "+string(w(7))+":"+string(w(8))+":"+string(w(9))
            lbytes(k)=x(1);
            lisdir(k)=double(int32(x(2)) & mask) == dirtype
        end
    end
    if n==1 then
        lfiles=lfiles(1);
        ldate=ldate(1);
        lbytes=lbytes(1);
        lisdir=lisdir(1);
    end
    L=mlist(["st","dims","name","date","bytes","isdir"],int32([n 1]),lfiles,ldate,lbytes,lisdir)

endfunction
