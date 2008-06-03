// Construction de la liste des dépendances
// avril 2008 by Delphine

function listeDep = checkConflict(nom, version)
  global conflictLocal
  global conflictingList
  global nomconflictLocal
  global conflictVersion
  // S'il n'y a pas de version
  if argn(2) == 1
    version = ""
  end
  clearglobal conflictingList
  listeDep = checkDependencies(nom, version)
  [n, m] = size(listeDep)
  for i=1:n-1
    // On compare les nom
    [nom1, version1] = separateVersionDep(listeDep(i))
    [nom2, version2] = separateVersionDep(listeDep(i+1))
    if nom1 == nom2
      // s'il y a 2 versions possible c'est que forcément il y a une dep <=
      // Si c'est le cas, il faut que cette version max concorde avec les autres dép
      // on l'installe en local, et si le tag conflictLocal est à 1 il y a conflit version, sinon, c'est ok
      v1 = decoup(version1)
      v2 = decoup(version2)
      if compareVersion(v1, ">=", v2)
        dlInstall(nom1, version2)
      else
        dlInstall(nom1, version1)
      end
      clearglobal conflictLocal
      clearglobal conflictingList
      clearglobal nomconflictLocal
      clearglobal conflictVersion
      listeDep2 = checkDependencies(nom, version)
      if conflictLocal == 1
        conflictVersion = 1
        rep = toolboxDirectory()
        rmdir(rep + nom1, "s")
      end
    end
  end
endfunction

function listeDep = checkDependencies(nom, version)
  // Pour éviter les redondances et les boucles
  global conflictingList
  global conflictLocal
  global nomconflictLocal
  global conflictVersion
  // S'il n'y a pas de version
  if argn(2) == 1
    version = ""
  end
  // Lecture du fichier description
  desc = readDescription(nom)
  // Selection de la position de la toolbox dans la liste des toolboxes disponibles.
  position = selectPosition(desc, nom, version)
  // Cas ou la toolboxe n'est pas présente
  if position == 0
    displayMessage("Toolbox " + nom + " non trouvee")
    listeDep = ""
    return listeDep
  elseif position == 0.1
    conflictLocal = 1
    nomconflictLocal = nom
    listeDep = ""
    return listeDep
  end
  // Si la toolbox a déjà été rencontrée sous cette version, inutile de refaire l'arbre des dépendances
  if find(conflictingList == (desc("Toolbox")(position) + " (" + desc("Version")(position) + ")"))
    resume
  end
  listeDep = desc("Toolbox")(position) + " (" + desc("Version")(position) + ")"
  [a, b] = size(conflictingList)
  conflictingList(a+1) = desc("Toolbox")(position) + " (" + desc("Version")(position) + ")"
  // Récuperation des dependances
  depends = extractValue("Depends", desc, position)
  // S'il n'y a pas de dépendance
  if depends == " " | depends == ""
  	return listeDep
  end
  // S'il y a plusieurs dépendances on split en plusieurs string
  depends = splitValue(depends, ',')
  // Ajout dans la liste des toolboxes à installer
  [n, m] = size(depends)
  for i=1:n
	[nb, x] = size(listeDep)
	// Separation de la version et de la dependance
	[depend, version] = separateVersionDep(depends(i))
	// Recherche des dépendances
	deps = checkDependencies(depend, version)
	[o, p] = size(deps)
	for i=1:o
      listeDep(nb+i)=deps(i)
    end	
  end
  listeDep = unique(listeDep)
  return listeDep
endfunction

// Selection de la position de la toolbox dans la liste des toolboxes disponibles.
function position = selectPosition(listDesc, nomToolbox, versionToolbox)
  [n, m] = size(listDesc("Toolbox"))
  if versionToolbox <> ""
    [signeToolbox, versionToolbox] = separateSignVersion(versionToolbox)
    versionToolbox = decoup(versionToolbox)
  end
  for i=1:n
    if versionToolbox == "" & listDesc("Toolbox")(i) == nomToolbox
      position = i
      // Recherche de version plus récente
      for j=i+1:n
        if listDesc("Toolbox")(j) == nomToolbox
          vi = decoup(listDesc("Version")(position))
          vj = decoup(listDesc("Version")(j))
		  if compareVersion(vj, ">=", vi)
		    position = j
		  end
        end
      end      
      return position
    elseif listDesc("Toolbox")(i) == nomToolbox
      if compareVersion(decoup(listDesc("Version")(i)), signeToolbox, versionToolbox)
        position = i
        // Recherche de version plus récente
        for j=i+1:n
          if listDesc("Toolbox")(j) == nomToolbox & compareVersion(decoup(listDesc("Version")(j)), signeToolbox, versionToolbox)
            vi = decoup(listDesc("Version")(position))
            vj = decoup(listDesc("Version")(j))
		    if compareVersion(vj, ">=", vi)
		      position = j
		    end
          end
        end
        return position
      end
    end
  end
  // Toolbox non trouvée
  // Cas ou on a regardé en local
  if n == 1
    position = 0.1
  // Cas ou on a regardé sur les repository
  else
    position = 0
  end
endfunction

// Séparation de la dépendance et de la version
function [depend, version] = separateVersionDep(depend)
  version = ""
  index = strindex(depend, "(")
  if index > 0
    // index-1 pour laisser la ( avec la version
    temp = strsplit(depend,index-1) 
    depend = temp(1)
    version = temp(2)
    // On enlève les parenthèses et les espaces
    version = strsubst(version, "(", "")
    version = strsubst(version, ")", "")
    version = strsubst(version, " ", "")
    depend = strsubst(depend, " ", "")
  end
endfunction

// Séparation du signe >=, <= ou =  et de la version
function [signe, version] = separateSignVersion(version)
  index = strindex(version, "=")
  version = strsplit(version,index)
  signe = version(1)
  version = version(2)
endfunction

// Découpage de la version en 2 ou 3 chiffres
function version = decoup(version)
  index = strindex(version, ".")
  version = strsplit(version,index)
  version = strsubst(version, ".", "")
  version = strsubst(version, " ", "")
endfunction

// Comparaison de deux versions
function result = compareVersion(v1, signe, v2)
  // On converti les string en double
  v1 = strtod(v1)
  v2 = strtod(v2)
  if v1(1) > v2(1)
    if signe == ">="
      result = %T
    else
      result = %F
    end
  elseif v1(1) ==  v2(1)
    if v1(2) > v2(2)
      if signe == ">="
        result = %T
      else
        result = %F
      end
   	elseif v1(2) == v2(2)
  	  // On regarde s'il existe un 3ème chiffre
  	  [o, m] = size(v2)
  	  [n, m] = size(v1)
  	  if n == 3 & o == 3
  	    if v1(3) > v2(3)
          if signe == ">="
            result = %T
          else
            result = %F
          end
    	elseif v1(3) == v2(3)
          result = %T
  	    else
  	      if signe == "<="
  	        result = %T
  	      else
  	        result = %F
  	      end
  	    end
      elseif n == 2 & o == 2
        result = %T
  	  else
  	    if n == 2 & v2(3) == 0 | o == 2 & v1(3) == 0
          result = %T
        elseif n == 2 & v2(3) > 0
  	      if signe == "<="
  	        result = %T
  	      else
  	        result = %F
  	      end
        elseif o == 2 & v1(3) > 0
          if signe == ">="
            result = %T
          else
            result = %F
          end
  	    else
  	      result = %F
  	    end
  	  end
  	else
  	  if signe == "<="
  	    result = %T
  	  else
  	    result = %F
  	  end
   	end
  else
    if signe == "<="
  	  result = %T
  	else
  	  result = %F
  	end
  end
endfunction

// Récupération d'un champs précis de la description pour une toolbox
function fieldValue = extractValue(field, listdDesc, position)
  fieldValue = listdDesc(field)(position)
endfunction

// S'il y a plusieurs valeurs on split en plusieurs string
function fieldValue = splitValue(fieldValue, separate)
  index = strindex(fieldValue, separate)
  if length(index) > 0
    fieldValue = strsplit(fieldValue,index)
    //On enleve le separateur et les expaces qui trainent en fin field
    fieldValue = strsubst(fieldValue, separate, "")
    fieldValue = strsubst(fieldValue, " ", "")
  end
endfunction
