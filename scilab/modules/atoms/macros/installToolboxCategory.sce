// Installation d'une sous catégorie de toolbox
// avril 2008 by Delphine

// Category : cat1 (sscat1, sscat2); cat2 (sscat3, sscat4)
function result = installToolboxCategory(cat, sscat)
  result = %f
  // On charge la liste de toutes les toolboxes dispo sur le net
  desc = readDescription("")
  [nbTool, m] = size(desc("Toolbox"))
  for i=1:nbTool
    catTool = desc("Category")(i)
    // On regarde si cette toolbox appartient à la cat/sscat demandée
    regularExpression = "/" + cat + " \((\w*, )*" + sscat + "(, \w*)*\)/"
    // Si oui on l'installe
    if regexp(catTool, regularExpression) <> []
      // On ne peut pas prendre result = installToolbox() car s'il y a une erreur d'install, result = %f
      installToolbox(desc("Toolbox")(i));
      result = %t
    end   
  end
  // Si on n'a rien installé
  if ~result
    displayMessage ("Aucune Toolbox ne correspond a cet ensemble categorie/sous categorie")
  end
  return result
endfunction
