// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2008 - DIGITEO - Allan CORNET
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- TEST WITH GRAPHIC -->
// <-- INTERACTIVE TEST -->

plot3d();

lnf = getlookandfeel();
if lnf == [] then pause,end
if lnf == '' then pause,end

lnfs = getinstalledlookandfeels();
if lnfs == [] then pause,end
if lnfs == '' then pause,end
if size(lnfs,'*') == 0 then pause,end
if size(lnfs,'*') == 1 then pause,end

setlookandfeel(); // system look and feel
lnf = getlookandfeel();

for i=1:size(lnfs,'*')
  setlookandfeel(lnfs(i));
  sleep(2000);
end

setlookandfeel(); // system look and feel

xdel(winsid());
