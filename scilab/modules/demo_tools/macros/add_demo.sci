// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) INRIA
// Copyright (C) DIGITEO - 2011 - Allan CORNET
//
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt

function add_demo(demotitle, path)

  if and(getscilabmode() <> ["NW";"STD"]) then
    return;
  end

  global demolist;
  global demolistlock;

  if isempty(demolist) then
    if isempty(demolistlock) then
      demolistlock = %t;
      // we load scilab demos only when it is required
      modules = getmodules();
      for i=1:size(modules,"*")
        if isfile("SCI/modules/"+modules(i)+"/demos/" + modules(i) + ".dem.gateway.sce") then
          exec("SCI/modules/"+modules(i)+"/demos/" + modules(i) + ".dem.gateway.sce",-1);
        end
      end
      clear demolistlock;
      clearglobal demolistlock;
    end
  end

  path = pathconvert(path, %f, %t);

  if ~isfile(path) then
    error(msprintf(gettext("%s: The file %s does not exist.\n"), "add_demo", path));
  end

  if isdir(path) then
    error(msprintf(gettext("%s: Wrong input argument #%d: Path to a scilab script file expected.\n"), "add_demo", 2));
  end

  k = find(demolist(:,1) == demotitle);

  if k == [] then
    demolist = [demolist; demotitle,path];
  else
    k1 = find(demolist(k,2) == path)
    if k1 == [] then
      demolist = [demolist; demotitle + " (" + string(size(k,"*")) + ")", path];
    end
  end

  clear demolist

endfunction
