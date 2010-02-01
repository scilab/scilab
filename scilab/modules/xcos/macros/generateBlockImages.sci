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
  
  f = gcf();

  varsToLoad = gsort(varsToLoad, "r", "i");
  for kBlock = 1 : size(varsToLoad, "*")
    
    clf();
    
    BlockFile = outPath + varsToLoad(kBlock) + ".h5";
    
    ierr = execstr("scs_m  = " + varsToLoad(kBlock) + "(""define"")", "errcatch");
    
    if ierr == 0 then
      mprintf("%d: %s",  kBlock, varsToLoad(kBlock));
      sz = scs_m.graphics.sz;
      orig = scs_m.graphics.orig;

      // Customize axes
      a = gca();
      a.data_bounds = [orig(1), orig(2); sz(1), sz(2)];
      a.isoview = "on";
      a.margins = [0.001, 0.001, 0.001, 0.001];
      a.box ="on";
      f.axes_size = [max(20, 20 * sz(1)), max(20, 20 * sz(2))];

      if stripblanks(scs_m.graphics.gr_i(1)) == ""  | isempty(scs_m.graphics.gr_i(1)) then
	mprintf("(empty gr_i)");
	diagram = scicos_diagram();
	options = diagram.props.options;
	sz = scs_m.graphics.sz;
	orig = scs_m.graphics.orig;
	
	// Create variable o because needed inside "plot"
	o = scs_m; 
	o.graphics.exprs = ["";""];
	ierr = execstr("scs_m  = " + varsToLoad(kBlock) + "(""plot"",o)", "errcatch");
	if ierr <> 0 then
	  mprintf(" FAILED\n");
	else
	  xs2gif(f.figure_id, outPath + varsToLoad(kBlock) + ".gif");
	  mprintf(" SUCCEED\n");
	end
	
      else
        ierr = 0;
        if type(scs_m.graphics.gr_i)==10 & size(scs_m.graphics.gr_i, "*") <> 1 then
          for k=1:size(scs_m.graphics.gr_i, "*")
            ierr = ierr + execstr(scs_m.graphics.gr_i(k), "errcatch");
          end
        else
	  ierr = execstr(strcat(scs_m.graphics.gr_i(1),";"), "errcatch");
        end
        if ierr <> 0 then
          mprintf(" FAILED\n");
        else
          xs2gif(f.figure_id, outPath + scs_m.gui + ".gif");
          mprintf(" SUCCEED\n");
        end
      end
    end
    
  end
  
  delete(f);
  
endfunction

function c=scs_color(c)
endfunction
