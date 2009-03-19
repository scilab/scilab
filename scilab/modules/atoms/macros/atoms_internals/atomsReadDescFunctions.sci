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

// Parsing of the Description-Function file

function descFunct = atomsReadDescFunctions(nom)
  global numberFunction
  rep = atomsToolboxDirectory()
  d = rep + nom
  // Support of the different OS 
  if getos() == "Windows"
    directory = d + "\DESCRIPTION-FUNCTIONS"
  else // linux and mac
    directory = d + "/DESCRIPTION-FUNCTIONS"
  end
  // If we find the local directory and the DESCRIPTION-FUNCTIONS file is present
  if (isdir(d) & ls(directory) <> [])
    cd (d)
    // Reading of thedescription file which we store in a array
    tab = atomsReadFile("DESCRIPTION-FUNCTIONS")
    // Creation of a liste
    [n, m] = size(tab)
    numberFunction = 0
    for i=1:n
      ind = strindex(tab(i),'-')
      // If ind = [] we are always in the previous field
      if ind == []
        descFunct(string(numberFunction)) = descFunct(string(numberFunction)) + tab(i)
      else
        numberFunction = numberFunction + 1
        descFunct(string(numberFunction)) = tab(i)
      end
    end
  else
    atomsDisplayMessage(sprintf(_("The DESCRIPTION-FUNCTIONS file of the toolbox %s is missing"),nom))
    descFunct = ""
  end
  return descFunct
endfunction
