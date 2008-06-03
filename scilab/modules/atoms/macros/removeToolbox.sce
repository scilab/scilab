// suppression d'une toolbox
// avril 2008 by Delphine

function result = removeToolbox(nom)
  rep = toolboxDirectory()
  // Si nom = "all" on supprime toutes les Toolboxes
  if nom == "all"  
    rmdir(rep, "s")
    mkdir(rep)
  else
    // On vérifie que la toolbox existe
    d = rep + nom
    if ~isdir(d)
      displayMessage("La toolbox " + nom + " n''est pas installee")
      result = %f
      return result
    end
    cd (rep)
    // On vérifie que la toolbox n'est pas une dépendance d'une autre
    listLocal = ls()
    [n, m] = size(listLocal)
    for i=1:n
      desc = readDescription(listLocal(i))
      depends = extractValue("Depends", desc, 1)
      depends = splitValue(depends, ",")
      [n, m] = size(depends)
      for j=1:n
        [depend, version] = separateVersionDep(depends(j))
        if find(depend == nom) <> []
          displayMessage("La Toolbox " + nom + " est une dependance de " + listLocal(i))
          reponse = input("Continuer? [""y""/""n""]")
          if (reponse <> "y" & reponse <> "Y")
            result = %f
            return result
          end
        end
      end
    end
    rmdir(rep + nom, "s")
    displayMessage(nom + " supprime")
    result = %t
    return result
  end
endfunction
