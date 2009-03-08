// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2008 - INRIA - Delphine GASC <delphine.gasc@scilab.org>
//
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt

// Parsing of the Description file

function desc = atomsReadDesc(nom)
  // We go to the directory with the toolboxes
  rep = atomsToolboxDirectory()
  d = rep + nom
  // Support of the different OS 
  if getos() == "Windows"
    directory = d + "\DESCRIPTION"
  else // linux and mac
    directory = d + "/DESCRIPTION"
  end
  // If we find the repertory in local and the DESCRIPTION file is present
  if (isdir(d) & ls(directory) <> [])
    cd (d)
    // Reading of the DESCRIPTION file which we stock in a array
    tab = atomsReadFile("DESCRIPTION")
    // Creation of a "hash table"
    desc = atomsListDescription()
    // Filling of the hash table
    desc = hashTable(desc, tab)
    // Treatment of the file name (special characteres) to insure the validity of the name file and the PATH 
    desc("Toolbox") = atomsSubstituteString(desc("Toolbox"))
    // We add the function field
    clearglobal numberFunction
    desc("Function") = atomsReadDescFunctions(nom)
  // Else we search in the net
  else
  	cd (rep)
  	// Creation of a "hash table"
	desc = atomsListDescription()
	// Sites list to cross
	listMirror = atomsToolboxMirror()
	[n, m] = size(listMirror)
	clearglobal numberFunction
	global numberFunction
	numberFunction = 0
	for i=1:m
	  // We recup the file in the net and we put a copy in the directory under the name TOOLBOXES
  	  if dlFile(listMirror(i), "TOOLBOXES")
        // Reading of the TOOLBOXES file which we stock in a array
        tab = atomsReadFile("TOOLBOXES")
        // We delete the temporary file created
        if ~removeFile("TOOLBOXES")
	      disp(_("Please delete the file TOOLBOXES of your current directory"))
	    end
        // We fill the array with the different toolboxes
        desc = hashTable2(desc, tab)
        // Treatment of the file name (special characteres) to insure the validity of the name file and the PATH 
        [a, b] = size(desc("Toolbox"))
        for j=1:a
          desc("Toolbox")(j) = atomsSubstituteString(desc("Toolbox")(j))
        end
      else
        result = %f
        return result
      end
    end
  end
  result = %t
  return result
endfunction

// Retrieve of the file in the web
function result = dlFile(web, fileWeb)
	URL=web + "/TOOLBOXES";
	[rep,stat,err] = unix_g("wget "+URL + " -O " + fileWeb)
	// If the file is not present
	if stat <> 0
	  atomsDisplayMessage(_("Please check the validity of the repository:"))
	  atomsDisplayMessage(sprintf(_("Download of ''%s'' failed"),URL))
	  if ~removeFile(fileWeb)
	    disp(sprintf(_("Please delete the file %s in your current repertory."),fileWeb))
	  end
	  result = %f
	  return result
	end
	result = %t
	return result
endfunction

// Delete function of the file
function result = removeFile(fileR)
  result = deletefile(fileR)
  return result
endfunction

// Easy filling of the hash table (DESCRIPTION file)
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

// Filling of the hash table (TOOLBOX file)
function listDesc = hashTable2(listDesc, tabDesc)
  global numberFunction
  [listeObl, listeOpt] = atomsConstant()
  [n, m] = size(tabDesc)
  [nbTool, m] = size(listDesc("Toolbox"))
  [o, p] = size(listeOpt)
  inFunct = %f
  for i=1:n
    // We create a flag to know if we are in the function part
    if tabDesc(i) == "--"
      inFunct = %t
    elseif tabDesc(i) == "//"
      inFunct = %f
    end
    if tabDesc(i) <> "//" & ~inFunct
      ind = strindex(tabDesc(i),':')
      // ind+1 to remove the space before the second field
      temp = strsplit(tabDesc(i),ind+1)
      // We remove the ": "
      temp(1) = strsubst(temp(1), ": ", "")
      listDesc(temp(1))(nbTool+1)= temp(2)
    elseif tabDesc(i) == "--"
      clear tmp
      nbFunct = 0
    elseif tabDesc(i) <> "--" & inFunct
      ind = strindex(tabDesc(i),'-')
      // If ind = [] we are always in the previous field
      if ind == []
        tmp(string(nbFunct)) = tmp(string(nbFunct)) + tabDesc(i)
      else
        nbFunct = nbFunct + 1
        tmp(string(nbFunct)) = tabDesc(i)
        if numberFunction < nbFunct
          numberFunction = nbFunct
        end
      end
    elseif tabDesc(i) == "//"
      // If a toolbox doesn't have functions, to avoid the bug : "Undefined variable: tmp"
      if isdef("tmp")
        listDesc("Function")(nbTool+1)= tmp
      end
      // We create all the values for all the keys so that even if one optional field is not present in the array, there is no error
      // But only if the // is followed by other things
      if i <> n
        for j=1:p
          listDesc(listeOpt(j))(nbTool+2) = ""
        end
      end
      nbTool=nbTool+1
    end
  end
endfunction
