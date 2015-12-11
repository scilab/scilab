// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) DIGITEO - 2011 - Allan CORNET
//
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt

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
