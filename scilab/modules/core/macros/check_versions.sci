// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2007 - INRIA - Allan CORNET
//
// Copyright (C) 2012 - 2016 - Scilab Enterprises
//
// This file is hereby licensed under the terms of the GNU GPL v2.0,
// pursuant to article 5.3.4 of the CeCILL v.2.1.
// This file was originally licensed under the terms of the CeCILL v2.1,
// and continues to be available under such terms.
// For more information, see the COPYING file which you should have received
// along with this program.

function ret=check_versions(varargin)
    [lhs,rhs]=argn(0);
    ret=%T;
    modules=getmodules();
    szmodules=size(modules);

    for i=1:1:szmodules(1)
        file_to_check=SCI+"/modules/"+modules(i)+"/version.xml";
        mprintf(gettext("Checking: %s\n"),modules(i))
        if isfile(file_to_check) then
            try
                if getos() == "Windows" then
                    stat = unix_w(SCI+"\modules\helptools\bin\xmllint\xmllint --noout --valid "+file_to_check);
                else
                    stat = unix_w("xmllint --noout --valid "+file_to_check);
                end
            catch
                ret=%F;
                mprintf(gettext("%s: Please check: %s.\n"),"check_versions",file_to_check);
            end
        else
            mprintf(gettext("%s: File doesn''t exist: %s"),"check_versions",file_to_check);
        end
    end

endfunction
