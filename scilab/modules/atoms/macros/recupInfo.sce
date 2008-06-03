// Recuperation des inforations de base
// avril 2008 by Delphine

// Champs présents dans le fichier DESCRIPTION
function [listeObl, listeOpt] = constant()
  // Champs obligatoires
  listeObl = ["Toolbox","Version","Title","Author","Maintainer","Description","License", "Category"]
  // Champs optionnels
  listeOpt = ["Date","Depends", "URL", "Entity"]
endfunction

// Fonction de récupération du repertoire contenant les toolboxes
function rep = toolboxDirectory()
  rep = "home/dev/toolboxes/"
endfunction

// Fonction de récupération de la liste des Mirror
function listMirror = toolboxMirror()
  listMirror = ["http://128.93.23.238/scilab/src/contrib", "http://128.93.23.238/scilab/bin/windows/contrib"]
endfunction
