// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2020 - ESI Group - Clement DAVID
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================
//
// <-- CLI SHELL MODE -->
// <-- NO CHECK REF -->
//

// only check API there and ensure there is some instrumentation in progress

info = covStart("SCI/modules/xcos");
assert_checktrue(info > 0);

info = covStart(["SCI/modules/xcos/macros" "xcoslib" ; ..
                 "SCI/modules/scicos/macros/scicos_auto" "scicos_autolib" ; ..
                 "SCI/modules/scicos/macros/scicos_scicos" "scicos_scicoslib" ; ..
                 "SCI/modules/scicos/macros/scicos_utils" "scicos_utilslib"]);
assert_checktrue(info > 0);

info = covStart("elementary_functions");
assert_checktrue(info > 0);

info = covStart("all");
assert_checktrue(info > 0);
