// Affichage des infos d'une toolbox
// avril 2008 by Delphine

function infoToolbox(nom)
  rep = toolboxDirectory()
  // On va dans le repertoire contenant les toolboxes
  d = rep + nom
  // Soit on trouve le dossier en local et le fichier DESCRIPTION est présent
  if (isdir(d) & ls(d + "/DESCRIPTION") <> [])
    desc = readDescription(nom)
    displayMessage("La Toolboxe est presente localement")
    displayMessage(desc)
  // Soit on regarde si des versions sont disponibles sur le net
  else
    displayMessage("Versions de la Toolbox presentes sur le web")
    listDesc = readDescription("")
    versions = ""
    [n, m] = size(listDesc("Toolbox"))
    for i=1:n
      if listDesc("Toolbox")(i) == nom
        // Pour éviter les redondances de version
        [a, b] = size(versions)
        if find(versions == listDesc("Version")(i))
          continue
        else
          versions(a+1) = listDesc("Version")(i)
          desc = listDescription()
          [listeObl, listeOpt] = constant()
          [o, p] = size(listeOpt)
          [n, m] = size(listeObl)
          for j=1:m
            desc(listeObl(j)) = listDesc(listeObl(j))(i)
          end
          for j=1:p
            desc(listeOpt(j)) = listDesc(listeOpt(j))(i)
          end
          displayMessage(desc)
        end
      end
    end
    // Si aucune version n'a été trouvée
    [a, b] = size(versions)
    if a == 1
     displayMessage("Aucune")
    end
  end
endfunction
