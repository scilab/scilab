// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2008-2008 - DIGITEO - Jean-Baptiste Silvy
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================
// <-- NOT FIXED -->

// <-- TEST WITH GRAPHIC -->

// used to create java exceptions
for i=1:100,
  demo_gui();
  delete(gcf());
end

