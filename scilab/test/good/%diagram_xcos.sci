//
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2009 - DIGITEO - Vincent COUVERT
//
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
//
//
function %diagram_xcos(scs_m)

k= 1 ;
tempfile = TMPDIR + filesep() + "xcos_scs_m_" + string(k) + ".h5";

while isfile(tempfile)
  k = k + 1;
  tempfile = TMPDIR + filesep() + "xcos_scs_m_" + string(k) + ".h5";
end

export_to_hdf5(tempfile, "scs_m");

xcos(tempfile);

endfunction
