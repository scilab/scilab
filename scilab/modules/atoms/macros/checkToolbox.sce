// Vérification de la bonne contruction d'une toolbox
// avril 2008 by Delphine

function result = checkToolbox(nom)
  // On va dans le repertoire contenant les toolboxes
  rep = toolboxDirectory()
  d = rep + nom
  // Si le dossier n'existe pas
  if ~(isdir(d))
    displayMessage("Le dossier de la toolbox " + nom + " n''existe pas ou est mal nomme")
    result = %f
    return result
  end
  cd (d)
  if ~checkDescription()
    displayMessage("Le fichier DESCRIPTION n''est pas valide")
    result = %f
    return result
  end
  if ~checkLoader()
    displayMessage("Le fichier loader.sce n''est pas present")
    result = %f
    return result
  end
  if ~checkTest()
    displayMessage("Il serait interessant que des test soient disponibles")
  end
  if ~checkHelp()
    displayMessage("Il serait interessant qu''une aide soit disponible")
  end
  if ~checkDemo()
    displayMessage("Il serait interessant qu''une demonstration soit disponible")
  end
  if ~checkReadme()
    displayMessage("Il serait interessant que le fichier readme soit present")
  end
  if ~checkLicense()
    displayMessage("Il serait interessant que le fichier license soit present")
  end
  result = %t
  return result
endfunction

function result = checkDescription()
  // Si le fichier DESCRIPTION est présent
  if ls("DESCRIPTION") <> []
    // Lecture du fichier description qu'on stocke dans un tableau
    tab = readFile("DESCRIPTION")
    // Création d'un "tableau de hash"
    desc = listDescription()
    // Vérification de la présence d'un seul espace après les :
    [n, m] = size(desc)
      for i=1:n
        ind = strindex(tab(i),':')
        if (part([tab(i)],[ind+1]) <> " " | part([tab(i)],[ind+2])== " ")
          displayMessage("The fields "  + tab(i) + " should have one and only one whitespace after the :")
          result = %f
          return result
        end
      end
    // Remplissage du tableau de hash
    desc = hashTable(desc, tab)
    // Verification que tous les champs obligatoires sont remplis (Toolbox, Version, License, Description, Title, Author and Maintainer)
    [listeObl, listeOpt] = constant()
    for i=1:7
      if desc(listeObl(i)) == []
        displayMessage("Le champs " + listeObl(i) + " est absent de la DESCRIPTION")
        result = %f
        return result
      end
    end
    // Verification que le nom correspond bien
    if desc("Toolbox: ") <> nom
      displayMessage("Le nom de la toolbox present dans le fichier DESCRIPTION n''est pas le bon")
      result = %f
      return result
    end
    // Verification que la version est au bon format
    v = desc("Version: ")
    if regexp(v, '/[0-9]+\.[0-9]+\.?[0-9]*$/') <> 1
      disp("The version is a sequence of at least two (and usually three) non-negative integers separated by single ''.'' characters.")
      result = %f
      return result
    end
  // Soit le fichier DESCRIPTION n'existe pas
  else
    displayMessage("Le fichier DESCRIPTION n''existe pas ou n''est pas a la bonne place")
    result = %f
    return result
  end
  result = %t
  return result
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
