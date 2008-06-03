// Parsing of the Description file
// avril 2008 by Delphine

function desc = readDescription(nom)
  // On va dans le repertoire contenant les toolboxes
  rep = toolboxDirectory()
  d = rep + nom
  // Soit on trouve le dossier en local et le fichier DESCRIPTION est présent
  if (isdir(d) & ls(d + "/DESCRIPTION") <> [])
    cd (d)
    // Lecture du fichier description qu'on stocke dans un tableau
    tab = readFile("DESCRIPTION")
    // Création d'un "tableau de hash"
    desc = listDescription()
    // Remplissage du tableau de hash
    desc = hashTable(desc, tab)
    // On rajoute le champs fonction
    desc("Function") = readDescriptionFunctions(nom)
  // Soit on va voir sur le net
  else
  	cd (rep)
  	// Création d'un "tableau de hash"
	desc = listDescription()
	// Liste des site à parcourir
	listMirror = toolboxMirror()
	[n, m] = size(listMirror)
	for i=1:m
	  // On récupère le fichier sur le site et on met une copie dans le dossier sous le nom de TOOLBOXES
  	  if dlFile(listMirror(i), "TOOLBOXES")
        // Lecture du fichier TOOLBOXES qu'on stocke dans un tableau
        tab = readFile("TOOLBOXES")
        // On supprime le fichier temporaire créé
        if ~removeFile("TOOLBOXES")
	      disp("Veuillez supprimer le fichier TOOLBOXES dans le repertoire courant")
	    end
        // On rempli le tableau avec les différentes toolboxes
        desc = hashTable2(desc, tab)
      else
        result = %f
        return result
      end
    end
  end
  result = %t
  return result
endfunction

// Fonction de lecture d'un fichier ligne à ligne
function tab = readFile(nom)
  // On a vérifié la présence du fichier avant
  // Il faudra verifier qu'on a les droits pour lire
  fd = mopen(nom,"r")
  [n, tab] = mfscanf(-1, fd, '%80[^\n]\n')
  mclose(fd)
endfunction

// Récupération d'un fichier sur le web
function result = dlFile(web, fileWeb)
	temp = unix("wget " + web + "/TOOLBOXES" + " -O " + fileWeb + " 2>&1 > /dev/null")
	// Si le fichier n'est pas present
	if temp <> 0
	  displayMessage("Verifiez la validite du repository")
	  if ~removeFile("TOOLBOXES")
	    disp("Veuillez supprimer le fichier TOOLBOXES dans le repertoire courant")
	  end
	  result = %f
	  return result
	end
	result = %t
	return result
endfunction

// Fonction de suppression d'un fichier
function result = removeFile(fileR)
  result = deletefile(fileR)
  return result
endfunction

// Création du "tableau de hash" contenant les informations du fichier DESCRIPTION
function listDesc = listDescription()
  [listeObl, listeOpt] = constant()
  [n, m] = size(listeObl)
  for i=1:m
    listDesc(listeObl(i))= []
  end
  [n, o] = size(listeOpt)
  for i=1:o
    listDesc(listeOpt(i))= []
  end
endfunction

// Remplissage du tableau de hash simple (fichier DESCRIPTION)
function listDesc = hashTable(listDesc, tabDesc)
  [listeObl, listeOpt] = constant()
  // On crée toutes les "cases" pour que même si un champs optionnel n'est pas présent dans le tableau, la case existe.
  [o, p] = size(listeOpt)
  for i=1:p
    listDesc(listeOpt(i))= ""
  end
  [n, m] = size(tabDesc)
  for i=1:n
    ind = strindex(tabDesc(i),':')
    // Si ind = [] on est dans la n-ième ligne du champs précédent
    if ind == []
      listDesc(temp(1)) = listDesc(temp(1)) + tabDesc(i)
    else
      // ind+1 pour enlever l'espace avant le 2ème champ
      temp = strsplit(tabDesc(i),ind+1)
      // On retire le ": "
      temp(1) = strsubst(temp(1), ": ", "")
      listDesc(temp(1))= temp(2)
    end
  end
endfunction

// Remplissage du tableau de hash (fichier TOOLBOXE)
function listDesc = hashTable2(listDesc, tabDesc)
  [listeObl, listeOpt] = constant()
  [n, m] = size(tabDesc)
  [nbTool, m] = size(listDesc("Toolbox"))
  [o, p] = size(listeOpt)
  inFunct = %f
  for i=1:n
    // On crée un flag pour savoir si on est dans la partie function
    if tabDesc(i) == "--"
      inFunct = %t
    elseif tabDesc(i) == "//"
      inFunct = %f
    end
    if tabDesc(i) <> "//" & ~inFunct
      ind = strindex(tabDesc(i),':')
      // ind+1 pour enlever l'espace avant le 2ème champ
      temp = strsplit(tabDesc(i),ind+1)
      // On retire le ": "
      temp(1) = strsubst(temp(1), ": ", "")
      listDesc(temp(1))(nbTool+1)= temp(2)
    elseif tabDesc(i) == "--"
      clear tmp
    elseif tabDesc(i) <> "--" & inFunct
      ind = strindex(tabDesc(i),'-')
      // Si ind = [] on est dans la n-ième ligne du champs précédent
      if ind == []
        tmp(temp(1)) = tmp(temp(1)) + tabDesc(i)
      else
        // ind+1 pour enlever l'espace avant le 2ème champ
        temp = strsplit(tabDesc(i),ind+1)
        // On enlève le " - " du nom de la fonction
        temp(1) = strsubst(temp(1), " - ", "")
        tmp(temp(1)) = temp(2)
      end
    elseif tabDesc(i) == "//"
      // Au cas ou une des toolbox n'ait pas les fonctions, pour éviter le bug : "Undefined variable: tmp"
      if isdef("tmp")
        listDesc("Function")(nbTool+1)= tmp
      end
      // On crée les "cases" optionnel pour que même si ce champs n'est pas présent dans le tableau, la case existe.
      // Mais seulement si le // est suivi d'autre chose
      if i <> n
        for j=1:p
          listDesc(listeOpt(j))(nbTool+2) = ""
        end
      end
      nbTool=nbTool+1
    end
  end
endfunction
