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
function generateBlockImages(palFiles, outPath)

rhs = argn(2);

if rhs < 2 then
  error(msprintf(gettext("%s: Wrong number of input argument(s): %d expected.\n"), "importScicosPal", 2));
  return
end


//load all files
varsToLoad = [];
for fIndex = 1:size(palFiles, "*")

  if ~isfile(palFiles(fIndex)) then
    error(msprintf(gettext("%s: File ''%s'' does not exist.\n"), "importScicosPal", palFiles(fIndex)));
    return
  end
  
  exec(palFiles(fIndex), -1);
  names = who("get");

  // Try to find scs_m variables
  for kVar = 1:size(names, "*")
    if ~isempty(strindex(names(kVar), "scs_m")) then
      currentPal = eval(names(kVar));
      for kBlock = 1:size(currentPal.objs)
	if typeof(currentPal.objs(kBlock))=="Block" then
	  if find(varsToLoad == currentPal.objs(kBlock).gui) == [] then
	    varsToLoad($+1) = currentPal.objs(kBlock).gui;
	  end
	end
      end
    end
  end
end

failed = [];

f = gcf();
for kBlock = 1 : size(varsToLoad, "*")
  
  clf();
  
  BlockFile = outPath + varsToLoad(kBlock) + ".h5";

  ierr = execstr("scs_m  = " + varsToLoad(kBlock) + "(""define"")", "errcatch");
  
  if ierr == 0 then
    mprintf("%d: %s",  kBlock, varsToLoad(kBlock));
    sz = scs_m.graphics.sz;
    //if or( f.axes_size < 0) then pause; end
    orig = scs_m.graphics.orig;
    a = gca();
    a.data_bounds = [orig(1), orig(2); sz(1), sz(2)];
    a.isoview = "on";
    a.margins = [0, 0, 0, 0];
    f.axes_size = [max(20, 20 * sz(1)), max(20, 20 * sz(2))];
    a.background = addcolor([0.9,0.9,0.9]);
    ierr = execstr(scs_m.graphics.gr_i(1), "errcatch");
    //execstr(scs_m.graphics.gr_i(1));
    if ierr <> 0 then
      mprintf(" FAILED\n");
      failed($+1) = kBlock;
    else
      //xs2jpg(f.figure_id, outPath + scs_m.gui + ".jpg");
      xs2gif(f.figure_id, outPath + scs_m.gui + ".gif");
      //xs2svg(f.figure_id, outPath + scs_m.gui + ".svg");
      mprintf(" SUCCEED %d %d\n", f.axes_size(1), f.axes_size(2));
    end
  end
  
end

delete(f);

endfunction

function c=scs_color(c)
endfunction
