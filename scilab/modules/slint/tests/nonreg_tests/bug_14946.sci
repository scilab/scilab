// ============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2019 - ESI-Group - Delamarre Cedric
//
//  This file is distributed under the same license as the Scilab package.
// ============================================================================

function bug_14946()
    b = (%pi + %e + %eps + %i + %t + %f + %T + %F);
    t = (TMPDIR + SCI + WSCI + SCIHOME);
    c = %s;
    d = %z;
    e = home;
endfunction