// Parsing of the Description-Function file
// mai 2008 by Delphine

function descFunct = readDescriptionFunctions(nom)
  // On va dans le repertoire contenant les toolboxes
  rep = toolboxDirectory()
  d = rep + nom
  // Soit on trouve le dossier en local et le fichier DESCRIPTION-FUNCTIONS est présent
  if (isdir(d) & ls(d + "/DESCRIPTION-FUNCTIONS") <> [])
    cd (d)
    // Lecture du fichier description qu'on stocke dans un tableau
    tab = readFile("DESCRIPTION-FUNCTIONS")
    // création d'une liste, dont les valeurs sont les noms de fonction
    [n, m] = size(tab)
    for i=1:n
      ind = strindex(tab(i),'-')
      // Si ind = [] on est dans la n-ième ligne du champs précédent
      if ind == []
        descFunct(temp(1)) = descFunct(temp(1)) + tab(i)
      else
        // ind+1 pour enlever l'espace avant le 2ème champ
        temp = strsplit(tab(i),ind+1)
        // On enlève le " - " du nom de la fonction
        temp(1) = strsubst(temp(1), " - ", "")
        descFunct(temp(1))= temp(2)
      end
    end
  else
    displayMessage("Le fichier DESCRIPTION-FUNCTIONS de la toolbox " + nom + " est manquant")
    descFunct = ""
  end
  return descFunct
endfunction
