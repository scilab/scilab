//
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) ????-2008 - INRIA
//
// This file is distributed under the same license as the Scilab package.
//

cd(SCI+'/modules/others/demos/loop_demo');
if MSDOS then
  while %t
    unix_g(SCI+'/bin/Wscilex -nb -f loop.sce');
  end 
else
  while %t
    unix_g(SCI+'/bin/scilex -nb -f loop.sce');
  end
end