// Recherche d'une toolbox
// avril 2008 by Delphine

function result = searchToolboxes(keyWord, typeSearch)
  // S'il n'y a pas de type
  if argn(2) == 1
    typeSearch = "all"
  end
  result = %f
  listTool = []
  // On met en forme
  keyWord = splitWord(keyWord)
  typeSearch = convstr(typeSearch,"l")
  [a, b] = size(keyWord)
  listDesc = readDescription("")
  [n, m] = size(listDesc("Toolbox"))
  // On regarde les Toolboxes une à une
  for i=1:n
    // Différents cas de valeur de typeSearch
    if typeSearch == "all"
      // Recherche dans Description + Title + Toolbox + Category + Author
      titleTool = convstr(listDesc("Title")(i),"l")
      descTool = convstr(listDesc("Description")(i),"l")
      nameTool = convstr(listDesc("Toolbox")(i),"l")
      catTool = convstr(listDesc("Category")(i),"l")
      authorTool = convstr(listDesc("Author")(i),"l")
      // Comparaison mot-clé et mots (on recherche la position du mot-clé dans le mot, si c'est == [] c'est qu'il n'a pas été trouvé; auquel cas, inutile de comparer les autres mot-clés)
      toolFind = %t
      for j=1:a
        if strindex(titleTool, keyWord(j)) == [] & strindex(descTool, keyWord(j)) == [] & strindex(nameTool, keyWord(j)) == [] & strindex(catTool, keyWord(j)) == [] & strindex(authorTool, keyWord(j)) == []
          toolFind = %f
          break
        end
      end
    elseif typeSearch == "author"
      // Recherche dans Author
      authorTool = convstr(listDesc("Author")(i),"l")
      // Comparaison mot-clé et mots
      toolFind = %t
      for j=1:a
        if strindex(authorTool, keyWord(j)) == []
          toolFind = %f
          break
        end
      end
    elseif typeSearch == "entity"
      // Recherche dans Entity
      entityTool = convstr(listDesc("Entity")(i),"l")
      // Comparaison mot-clé et mots
      toolFind = %t
      for j=1:a
        if strindex(entityTool, keyWord(j)) == []
          toolFind = %f
          break
        end
      end 
    else
      displayMessage("Le champs de recherche " + typeSearch + " n''existe pas")
      return result
    end
    // Si on a trouvé tous les mots
    if toolFind
      [n, m] = size(listTool)
      listTool(n+1) = listDesc("Toolbox")(i) + " - " + listDesc("Title")(i)
      result = %t
    end
  end
  if ~result
    displayMessage("Aucune Toolbox ne correspond a la recherche")
  else
    listTool = unique(listTool)
    [n, m] = size(listTool)
    if n == 1
      displayMessage("La Toolbox suivante correspond a la recherche :")
      displayMessage(listTool)
    else
      displayMessage("Les Toolboxes suivantes correspondent a la recherche :")
      for i=1:n
        displayMessage("- " + listTool(i))
      end
    end
  end
  return result
endfunction

// On split les phrases en un tableau de mots
function var = splitWord(var)
  // On passe tout en minuscule
  var = convstr(var,"l")
  // On split (erreur si le dernier caractère est un " ")
  var = splitValue(var, " ")
endfunction
