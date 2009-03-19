// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2008 - INRIA - Delphine GASC <delphine.gasc@scilab.org>
// Copyright (C) 2009 - DIGITEO - Sylvestre LEDRU <sylvestre.ledru@scilab.org>
// Copyright (C) 2009 - DIGITEO - Pierre MARECHAL <pierre.marechal@scilab.org>
//
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt

// Internal function

// Selection of the toolbox position in the available toolboxes list .

function position = atomsSelectPosition(listDesc, nomToolbox, versionToolbox, checkVersionScilab)
  [n, m] = size(listDesc("Toolbox"))
  if versionToolbox <> ""
    [signeToolbox, versionToolbox] = atomsSeparateSignVersion(versionToolbox)
    versionToolbox = atomsDecoupVersion(versionToolbox)
  end
  for i=1:n
    if (atomsVerifVersionScilab(listDesc("ScilabVersion")(i)) & checkVersionScilab) | ~checkVersionScilab
      if versionToolbox == "" & listDesc("Toolbox")(i) == nomToolbox
        position = i
        // Research of a more recent version
        for j=i+1:n
          if (atomsVerifVersionScilab(listDesc("ScilabVersion")(j)) & checkVersionScilab) | ~checkVersionScilab
            if listDesc("Toolbox")(j) == nomToolbox
              vi = atomsDecoupVersion(listDesc("Version")(position))
              vj = atomsDecoupVersion(listDesc("Version")(j))
		      if atomsCompareVersion(vj, vi) == 1
		        position = j
		      end
            end
          end      
        end
        return position
      elseif listDesc("Toolbox")(i) == nomToolbox
        cmp = atomsCompareVersion(atomsDecoupVersion(listDesc("Version")(i)), versionToolbox);
        if (signeToolbox == "<=" & cmp <= 0) | (signeToolbox == ">=" & cmp >= 0) | (signeToolbox == "=" & cmp == 0)
          position = i
          // Research of a more recent version
          for j=i+1:n
            if (atomsVerifVersionScilab(listDesc("ScilabVersion")(j)) & checkVersionScilab) | ~checkVersionScilab
              cmp = atomsCompareVersion(atomsDecoupVersion(listDesc("Version")(j)), versionToolbox);
              if listDesc("Toolbox")(j) == nomToolbox & ((signeToolbox == "<=" & cmp <= 0) | (signeToolbox == ">=" & cmp >= 0) | (signeToolbox == "=" & cmp == 0))
                vi = atomsDecoupVersion(listDesc("Version")(position))
                vj = atomsDecoupVersion(listDesc("Version")(j))
	  	        if atomsCompareVersion(vj, vi) >= 0
		          position = j
		        end
              end
            end
          end
          return position
        end
      end
    end
  end
  // Toolbox not found
  // Case where we have searched in local
  if n == 1
    position = 0.1
  // Case where we have searched in the repositories
  else
    position = 0
  end
endfunction
