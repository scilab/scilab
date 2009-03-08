// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2008 - INRIA - Delphine GASC <delphine.gasc@scilab.org>
//
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt

// Research of a toolbox

function result = searchToolboxes(keyword, typeSearch)
  // If there is no type
  if argn(2) == 1
    typeSearch = "all"
  end
  result = %f
  listTool = []
  // We shape
  keyword = splitWord(keyword)
  typeSearch = convstr(typeSearch,"l")
  [a, b] = size(keyword)
  global numberFunction
  listDesc = atomsReadDesc("")
  [n, m] = size(listDesc("Toolbox"))
  // We check all toolboxes one by one
  for i=1:n
    // Different value case of typeSearch
    if typeSearch == "all"
      // Research in Description + Title + Toolbox + Category + Author
      titleTool = convstr(listDesc("Title")(i),"l")
      descTool = convstr(listDesc("Description")(i),"l")
      nameTool = convstr(listDesc("Toolbox")(i),"l")
      catTool = convstr(listDesc("Category")(i),"l")
      authorTool = convstr(listDesc("Author")(i),"l")
      functionTool = listDesc("Function")(i)
      // Comparaison keyword and words (we research the key-word position in the word, if it's == [] the word is not found; in this case, it's useless to compare the other keywords)
      toolFind = %t
      for j=1:a
        if strindex(titleTool, keyword(j)) == [] & strindex(descTool, keyword(j)) == [] & strindex(nameTool, keyword(j)) == [] & strindex(catTool, keyword(j)) == [] & strindex(authorTool, keyword(j)) == []
          toolFind = %f
          for k=1:numberFunction
            if strindex(convstr(functionTool(string(k)), "l"), keyword(j)) <> []
              toolFind = %t
            end
          end
          if ~toolFind
            break
          end
        end
      end
    elseif typeSearch == "author"
      // Research in Author
      authorTool = convstr(listDesc("Author")(i),"l")
      // Comparaison keyword and words
      toolFind = %t
      for j=1:a
        if strindex(authorTool, keyword(j)) == []
          toolFind = %f
          break
        end
      end
    elseif typeSearch == "entity"
      // Research in Entity
      entityTool = convstr(listDesc("Entity")(i),"l")
      // Comparaison keyword and words
      toolFind = %t
      for j=1:a
        if strindex(entityTool, keyword(j)) == []
          toolFind = %f
          break
        end
      end 
    elseif typeSearch == "function"
      // Research in Function
      functionTool = listDesc("Function")(i)
      // Reading lines to lines of the functions
      // Comparaison keyword and words
      toolFind = %t
      for j=1:a
        for k=1:numberFunction
          toolFind = %f
          if strindex(convstr(functionTool(string(k)), "l"), keyword(j)) <> []
            toolFind = %t
            break
          end
        end
      end 
    else
      atomsDisplayMessage(sprintf(_("The research field %s doesn''t exist"),typeSearch))
      return result
    end
    // If we found the words
    if toolFind
      [n, m] = size(listTool)
      // We check if the Toolbox suit to the scilab version
      if atomsVerifVersionScilab(listDesc("ScilabVersion")(i))
        listTool(n+1) = listDesc("Toolbox")(i) + " - " + listDesc("Title")(i)
      else
        listTool(n+1) = listDesc("Toolbox")(i) + " - " + listDesc("Title")(i) + " - Warning this toolbox (Version " + listDesc("Version")(i) + ") isn''t compatible with your version of scilab"
      end
      result = %t
    end
  end
  if ~result
    disp(_("No toolbox matches the research"))
  else
    listTool = unique(listTool)
    [n, m] = size(listTool)
    if n == 1
      disp(_("The following toolbox matches the research:"))
      disp(listTool)
    else
      disp(_("The following toolboxes match the research:"))
      for i=1:n
        disp("- " + listTool(i))
      end
    end
  end
  return result
endfunction

// We split the sentences on a words array
function var = splitWord(var)
  // We cross everything in small letter
  var = convstr(var,"l")
  // We split (error if the last caracter is a " ")
  var = atomsSplitValue(var, " ")
endfunction
