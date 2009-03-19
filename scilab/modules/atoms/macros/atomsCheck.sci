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

// End user function

// Verification that the toolbox is built correctly

function result = atomsCheck(name)
  rhs=argn(2);

  if rhs == 1 then
  // We remove the special characters
  name = atomsSubstituteString(name)
  // We go in the repectory with toolboxes
  rep = atomsToolboxDirectory()
  d = rep + name
  // If the repertory doesn't exist
  if ~(isdir(d))
    atomsDisplayMessage(sprintf(_("The toolbox directory ''%s'' doesn''t exist or the name is wrong.\n"),name))
    result = %f
    return result
  end
  cd (d)
  if ~checkDescription()
    atomsDisplayMessage(_("The DESCRIPTION file isn''t valid"))
    result = %f
    return result
  end
    if ~checkDescriptionFunc()
    atomsDisplayMessage(_("The DESCRIPTION-FUNCTIONS file isn''t valid"))
    result = %f
    return result
  end
  if ~checkLoader()
    atomsDisplayMessage(_("The loader.sce file isn''t present"))
    result = %f
    return result
  end
  if ~checkTest()
    atomsDisplayMessage(_("It would be interesting to have tests available."))
  end
  if ~checkHelp()
    atomsDisplayMessage(_("It would be interesting to have help available."))
  end
  if ~checkDemo()
    atomsDisplayMessage(_("It would be interesting to have demonstrations available."))
  end
  if ~checkReadme()
    atomsDisplayMessage(_("It would be interesting to have a README file."))
  end
  if ~checkLicense()
    atomsDisplayMessage(_("It would be interesting to have a license file."))
  end
  result = %t
  return result
  else
    error(msprintf(gettext("%s: Wrong number of input argument: %d expected.\n"),"atomsCheck",1));
  end
endfunction

function result = checkDescription()
  // If the DESCRIPTION file is present
  if ls("DESCRIPTION") <> []
    // Reading of the description file which we stock in an array
    tab = atomsReadFile("DESCRIPTION")
    // Creation of a "hash table"
    desc = atomsListDescription()
    // Check of the presence of only one space after the :
    [n, m] = size(desc)
      for i=1:n
        ind = strindex(tab(i),':')
        if (part([tab(i)],[ind+1]) <> " " | part([tab(i)],[ind+2])== " ")
          atomsDisplayMessage(sprintf(_("The field %s should have one and only one whitespace after the :"),tab(i)))
          result = %f
          return result
        end
      end
    // Filling of the hash table
    desc = hashTable(desc, tab)
    // Check if all the compulsory fields are filled 
    [listeObl, listeOpt] = atomsConstant()
    for i=1:7
      if desc(listeObl(i)) == []
        atomsDisplayMessage(sprintf(_("The field %s is missing in the DESCRIPTION file"),listeObl(i)))
        result = %f
        return result
      end
    end
    // Check if the name correspond
    if desc("Toolbox") <> name
      atomsDisplayMessage(_("The toolbox name present in the DESCRIPTION file is false"))
      result = %f
      return result
    end
    // Check if the version has a good format
    v = desc("Version")
    if regexp(v, '/[0-9]+\.[0-9]+\.?[0-9]*$/') <> 1
      disp(_("The version is a sequence of at least two (and usually three) non-negative integers separated by single ''.'' characters."))
      result = %f
      return result
    end
  // If the DESCRIPTION file doesn't exist 
  else
    atomsDisplayMessage(_("The DESCRIPTION file doesn''t exist or isn''t at the right place"))
    result = %f
    return result
  end
  result = %t
  return result
endfunction

function result = checkDescriptionFunc()
  result = %t
  // If the DESCRIPTION-FUNCTIONS file is present
  if ls("DESCRIPTION-FUNCTIONS") <> []
    // Reading of the description file which we stock in an array
    tab = atomsReadFile("DESCRIPTION-FUNCTIONS")
    // Creation of a list
    [n, m] = size(tab)
    numberFunction = 0
    for i=1:n
      ind = strindex(tab(i),'-')
      // If ind = [] we are always in the previous field
      if ind == []
        descFunct(string(numberFunction)) = descFunct(string(numberFunction)) + tab(i)
      else
        descFunct(string(numberFunction + 1)) = tab(i)
        numberFunction = numberFunction + 1
      end
    end
    // Check if no function have the name of a Scilab function
    // Recuperation of the Scilab functions
    scilabFunction = listPrimitives()
    // Recuperation of each function name
    for i=1:numberFunction
      ind = strindex(descFunct(string(i)),'-')
      funct = strsplit(descFunct(string(i)), ind)
      funct = funct(1)
      funct = strsubst(funct, "-", "")
      funct = strsubst(funct, " ", "")
      // We search the name of the function in the Scilab functions list
      if strindex(scilabFunction, " " + funct + " ") <> []
        atomsDisplayMessage(sprintf(_("The function %s already exist in Scilab. Please change the name."),funct))
        result = %f
      end
    end
  // If the DESCRIPTION-FUNCTIONS file doesn't exist
  else
    atomsDisplayMessage(_("The file DESCRIPTION-FUNCTIONS doesn''t exist or isn''t at the good place."))
    result = %f
  end
  return result
endfunction

// Filling of a list with the DESCRIPTION file
function listDesc = hashTable(listDesc, tabDesc)
  [listeObl, listeOpt] = atomsConstant()
  // We create all the values for all the keys so that even if one optional field is not present in the array, there is no error.
  [o, p] = size(listeOpt)
  for i=1:p
    listDesc(listeOpt(i))= ""
  end
  [n, m] = size(tabDesc)
  for i=1:n
    ind = strindex(tabDesc(i),':')
    // If ind = [] we are always in the previous field
    if ind == []
      listDesc(temp(1)) = listDesc(temp(1)) + tabDesc(i)
    else
      // ind+1 to remove the space before the second field
      temp = strsplit(tabDesc(i),ind+1)
      // We remove the ": "
      temp(1) = strsubst(temp(1), ": ", "")
      listDesc(temp(1))= temp(2)
    end
  end
endfunction

function var = listPrimitives()
  // Recuperation of the scilab function
  [primitives,commandes] = what();
  listLib = librarieslist();
  [n, m] = size(listLib);
  for i=1:n
    [o, m] = size(primitives);
    temp = libraryinfo(listLib(i));
    [p, m] = size(temp);
    for j=1:p
      primitives(o + j) = temp(j);
    end
  end
  [o, m] = size(primitives);
  var = " "
  for i=1:o
    var = var + primitives(i) + " ";
  end
endfunction

function result = checkLoader()
  if ~ls("loader.sce") <> []
    result = %f
    return result
  end
  result = %t
  return result
endfunction

function result = checkTest()
  if ~isdir("unit tests") <> []
    result = %f
    return result
  end
  result = %t
  return result
endfunction

function result = checkHelp()
  if ~isdir("help") <> []
    result = %f
    return result
  end
  result = %t
  return result  
endfunction

function result = checkDemo()
  if ~isdir("demos") <> []
    result = %f
    return result
  end
  result = %t
  return result  
endfunction

function result = checkReadme()
  if ~ls("readme.txt") <> []
    result = %f
    return result
  end
  result = %t
  return result  
endfunction

function result = checkLicense()
  if ~ls("license.txt") <> []
    result = %f
    return result
  end
  result = %t
  return result  
endfunction
