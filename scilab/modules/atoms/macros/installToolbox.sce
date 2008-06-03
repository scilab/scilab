// Installation d'une toolbox
// avril 2008 by Delphine

function result = installToolbox(nom)
  global conflictingList
  global conflictLocal
  global nomconflictLocal
  global conflictVersion
  // On va dans le repertoire dédié
  rep = toolboxDirectory()
  cd (rep)
  // Nécessité de vider les variables globales avant toute utilisation de checkConflict
  clearglobal conflictingList
  clearglobal conflictLocal
  clearglobal nomconflictLocal
  clearglobal conflictVersion
  // Récupération de la liste des toolboxes à installer
  listTool = checkConflict(nom)
  // S'il existe une ligne vide, c'est qu'il manque une dépendance
  if find(listTool == "") <> []
    if conflictLocal == 1
      displayMessage("La version de la Toolbox " + nomconflictLocal + " presente localement n''est pas la bonne. Updatez la.")
    else
      displayMessage("Impossible d''installer toutes les dependances")
    end
    result =  %f
    return result
  elseif conflictVersion == 1
    displayMessage("Conflit de version pour " + nomconflictLocal)
    result =  %f
    return result
  end
  // Récupération de la liste des toolboxes qui sont déjà en local
  cd (rep)
  listLocal = ls()
  // Traitement des toolboxes à installer
  [m, n] = size(listTool)
  for i=1:m
    [nom, version] = separateVersionDep(listTool(i))
    // Si elle est présente en local on passe à la suivante
    if find(listLocal == nom) <> []
      continue
    else
      displayMessage("Va etre installe : " + nom)
      if ~dlInstall(nom, version)
        disp("Erreur lors de l''installation")
        result = %f
        return result
      end
    end
  end
  result = %t
  return result
endfunction

// DL et install
function result = dlInstall(nom, version)
  rep = toolboxDirectory()
  arch = nom + "." + version + ".tar.gz"
  // Téléchargement de l'archive
  cd (rep)
  listMirror = toolboxMirror()
  if dlArchive(listMirror, 1, arch)
    // Dezippage
    if dezipp(arch)
      // Suppression de l'archive
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

// Récupération d'une archive sur le web
function result = dlArchive(listWeb, position, name)
  temp = unix("wget " + listWeb(position) + "/" + name + " 2>&1 > /dev/null")
  // Si le fichier n'est pas present
  if temp <> 0
    [n, m] = size(listWeb)
    // Si on dépasse la taille de la liste
    if (position + 1) > m
      displayMessage("Verifiez la validite du repository")
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

// Dezippage
function result = dezipp(fileD)
  temp = unix("tar -zxvf " + fileD)
  if temp <> 0
    result = %f
    return result
  else
    result = %t
    return result
  end
endfunction
