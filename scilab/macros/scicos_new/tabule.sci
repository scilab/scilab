function texte = tabule(tab)
// transforms a matrix of strings into a column vector of string in tabular form 
//
//%Syntaxe
// [texte] = tableau(tab)
// 
//%Entrees
// tab		| c | 
//
//%Sorties
// texte	| c | Commentaire sur les lignes selectionn≈es
//
//%Variables globales
//
//%Remarques
//
//%Modifications
//     |   /  /   | 
//
//% Version
// METALIDO 1.0
//

// Copyright INRIA
[n_lignes, n_colonnes] = size(tab)
//- Largeur des colonnes
for i = 1 : n_colonnes
  //- Largeur maximale 
  largeur = maxi(length(tab(:, i)))
  //- Formatage de la colonne 
  for j = 1 : n_lignes
    if tab(j, i) <> '-' then
      tab(j, i) = part(tab(j, i), 1 : largeur)
    else
      tab(j, i) = part('-', ones(1,largeur))
    end
  end
//
end
//- Creation des lignes ---------------------------------------------------------------
texte = []
//
for i = 1 : n_lignes
  texte = [texte;'|'+strcat(tab(i, :), '|')+'|' ]
end
endfunction
