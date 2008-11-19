// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2008 - INRIA - Delphine GASC <delphine.gasc@scilab.org>
//
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt

// Verification that the toolbox is build correctly

function result = checkToolbox(nom)
  // We remove the special caracters
  nom = atomsSubstituteString(nom)
  // We go in the repectory with toolboxes
  rep = atomsToolboxDirectory()
  d = rep + nom
  // If the repertory doesn't exist
  if ~(isdir(d))
    atomsDisplayMessage("The toolbox directory" + nom + " doesn''t exist or the name is false")
    result = %f
    return result
  end
  cd (d)
  if ~checkDescription()
    atomsDisplayMessage("The DESCRIPTION file isn''t valid")
    result = %f
    return result
  end
    if ~checkDescriptionFunctions()
    atomsDisplayMessage("The DESCRIPTION-FUNCTIONS file isn''t valid")
    result = %f
    return result
  end
  if ~checkLoader()
    atomsDisplayMessage("The loader.sce file isn''t present")
    result = %f
    return result
  end
  if ~checkTest()
    atomsDisplayMessage("It would be interesting that tests are available")
  end
  if ~checkHelp()
    atomsDisplayMessage("It would be interesting that a help is available")
  end
  if ~checkDemo()
    atomsDisplayMessage("It would be interesting that a demonstration is available")
  end
  if ~checkReadme()
    atomsDisplayMessage("It would be interesting that the readme file is present")
  end
  if ~checkLicense()
    atomsDisplayMessage("It would be interesting that the license file is present")
  end
  result = %t
  return result
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
          atomsDisplayMessage("The field "  + tab(i) + " should have one and only one whitespace after the :")
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
        atomsDisplayMessage("The field " + listeObl(i) + " is missing in the DESCRIPTION file")
        result = %f
        return result
      end
    end
    // Check if the name correspond
    if desc("Toolbox") <> nom
      atomsDisplayMessage("The toolbox name present in the DESCRIPTION file is false")
      result = %f
      return result
    end
    // Check if the version has a good format
    v = desc("Version")
    if regexp(v, '/[0-9]+\.[0-9]+\.?[0-9]*$/') <> 1
      disp("The version is a sequence of at least two (and usually three) non-negative integers separated by single ''.'' characters.")
      result = %f
      return result
    end
  // If the DESCRIPTION file doesn't exist 
  else
    atomsDisplayMessage("The DESCRIPTION file doesn''t exist or isn''t at the good place")
    result = %f
    return result
  end
  result = %t
  return result
endfunction

function result = checkDescriptionFunctions()
  result = %t
  // If the DESCRIPTION-FUNCTIONS file is présent
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
        atomsDisplayMessage("The function " + funct + " already exist in Scilab. Thanks to change the name.")
        result = %f
      end
    end
  // If the DESCRIPTION-FUNCTIONS file doesn't exist
  else
    atomsDisplayMessage("The file DESCRIPTION-FUNCTIONS doesn''t exist or isn''t at the good place")
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
