// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2008 - INRIA - Delphine GASC <delphine.gasc@scilab.org>
//
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt

// Download and install

function result = atomsDlInstall(nom, version)
  rep = atomsToolboxDirectory()
  arch = nom + "." + version + ".tar.gz"
  // Download of the archive
  cd (rep)
  listMirror = atomsToolboxMirror()
  if dlArchive(listMirror, 1, arch)
    // Unzip
    if dezipp(arch)
      // Delete the archive
      if removeFile(arch)
        result = %t
        return result
      else
        result = %f
        return result
      end
    else
      result = %f
      return result
    end
  else
    result = %f
    return result  
  end
endfunction

// Recup the archive in the web
function result = dlArchive(listWeb, position, name)
  [rep,stat,err] = unix_g("wget " + listWeb(position) + "/" + name)
  // If the file is not present
  if stat <> 0
    [n, m] = size(listWeb)
    // If we exceed the list size
    if (position + 1) > m
      atomsDisplayMessage("Check the validity of the repository")
      result = %f
      return result
    else
      result = dlArchive(listWeb, position+1, name)
    end
  else 
    result = %t
  end
  return result  
endfunction

// Unzip
function result = dezipp(fileD)
  if getos() == "Windows"
    [rep,stat,err] = unix_g("zcat " + fileD + " | tar -xvf -")
  else // linux and mac
    [rep,stat,err] = unix_g("tar -zxvf " + fileD)
  end
  if stat <> 0
    result = %f
    return result
  else
    result = %t
    return result
  end
endfunction

// Delete function of the file
function result = removeFile(fileR)
  result = deletefile(fileR)
  return result
endfunction
