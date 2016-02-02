// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) DIGITEO - 2011 - Allan CORNET
//
// Copyright (C) 2012 - 2016 - Scilab Enterprises
//
// This file is hereby licensed under the terms of the GNU GPL v2.0,
// pursuant to article 5.3.4 of the CeCILL v.2.1.
// This file was originally licensed under the terms of the CeCILL v2.1,
// and continues to be available under such terms.
// For more information, see the COPYING file which you should have received
// along with this program.

//=============================================================================
function scilabincludeslist = dlwGetScilabIncludes()

    scilabincludeslist = ["$(SCIDIR)/libs/MALLOC/includes"; ..
    "$(SCIDIR)/libs/f2c"; ..
    "$(SCIDIR)/libs/intl"; ..
    "$(SCIDIR)/modules/core/includes"; ..
    "$(SCIDIR)/modules/api_scilab/includes"; ..
    "$(SCIDIR)/modules/call_scilab/includes"; ..
    "$(SCIDIR)/modules/output_stream/includes"; ..
    "$(SCIDIR)/modules/jvm/includes"; ..
    "$(SCIDIR)/modules/localization/includes"; ..
    "$(SCIDIR)/modules/dynamic_link/includes"; ..
    "$(SCIDIR)/modules/mexlib/includes"; ..
    "$(SCIDIR)/modules/time/includes"; ..
    "$(SCIDIR)/modules/windows_tools/includes"; ..
    "$(SCIDIR)/modules/threads/includes"; ..
    "$(SCIDIR)/modules/ast/includes/ast"; ..
    "$(SCIDIR)/modules/ast/includes/exps"; ..
    "$(SCIDIR)/modules/ast/includes/operations"; ..
    "$(SCIDIR)/modules/ast/includes/parse"; ..
    "$(SCIDIR)/modules/ast/includes/symbol"; ..
    "$(SCIDIR)/modules/ast/includes/system_env"; ..
    "$(SCIDIR)/modules/ast/includes/types"; ..
    "$(SCIDIR)/modules/ast/includes/analysis"; ..
    "$(SCIDIR)/modules/string/includes"; ..
    "$(SCIDIR)/modules/console/includes"];
endfunction
//=============================================================================
