// update d'une toolbox
// avril 2008 by Delphine

function result = updateToolbox(nom)
  rep = toolboxDirectory()
  // Si nom = "all" on update toutes les Toolboxes
  if nom == "all"
    listLocal = ls()
    [n, m] = size(listLocal)
    for k=1:n
      if ~updateToolbox(listLocal(k))
        break
      end
    end
  else
    // On vérifie que la Toolboxe existe bien en local
    d = rep + nom
    if ~isdir(d)
      displayMessage("La toolbox " + nom + " n''est pas installee")
      result = %f
      return result
    end
    // On récupère la version de l'actuelle
    desc = readDescription(nom)
    versionActuelle = extractValue("Version", desc, 1)
    versionActuelle = decoup(versionActuelle)
    // On recupère la liste de toutes les autres toolboxes disponibles
    listDesc = readDescription("")
    // On prend la version la plus récente
    position = selectPosition(listDesc, nom, "")
    // Si la toolbox n'est pas disponible en ligne
    if position == 0
      displayMessage("Toolbox non disponible sur le net")
      result = %f
      return result
    end
    versionNew = extractValue("Version", listDesc, position)
    versionNew = decoup(versionNew)
    // On regarde si c'est une mise à jour par rapport au local
    if compareVersion(versionNew, ">=", versionActuelle) & ~compareVersion(versionNew, "=", versionActuelle)
      // On regarde s'il était une dépendance max pour d'autres toolboxes
      listLocal = ls()
      [n, m] = size(listLocal)
      for i=1:n
        desc = readDescription(listLocal(i))
        depends = extractValue("Depends", desc, 1)
        depends = splitValue(depends, ",")
        [n, m] = size(depends)
        for j=1:n
          [depend, version] = separateVersionDep(depends(j))
          if version <> ""
            [signe, version] = separateSignVersion(version)
            if find(depend == nom) <> [] & signe == "<="
              version = decoup(version)
              // On regarde si la nouvelle version valide cette dépendance max
              if ~compareVersion(versionNew, "<=", version)
                displayMessage("probleme de dependance")
                result = %f
                return result
              end
            end
          end
        end
      end
    end
    // On vérifie s'il y a besoin d'updater des dépendances
    dependsNew = extractValue("Depends", listDesc, position)
    dependsNew = splitValue(dependsNew, ",")
    [n, m] = size(dependsNew)
    for i=1:n
      [dependsNew, versionNew] = separateVersionDep(dependsNew(i))
      listLocal = ls()
      if find(listLocal == dependsNew)
        if versionNew <> ""
          desc = readDescription(listLocal(i))
          v1 = extractValue("Version", desc, 1)
          v1 = decoup(v1)
          [signeNew, versionNew] = separateSignVersion(versionNew)
          versionNew = decoup(versionNew)
          if ~compareVersion(v1, signeNew, versionNew)
            updateToolbox(dependsNew);
          end
        end
      end
    end
    // On le prévient de ce qu'on va faire
    versionNew = extractValue("Version", listDesc, position)
    displayMessage("La toolbox " + nom + " va etre update en version " + versionNew)
    // On installe la nouvelle version
    rmdir(rep + nom, "s")
    installToolbox(nom);
  end
  result = %t
  return result
endfunction
