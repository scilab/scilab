//  Scicos
//
//  Copyright (C) INRIA - METALAU Project <scicos@inria.fr>
//
// This program is free software; you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation; either version 2 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program; if not, write to the Free Software
// Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
//
// See the file ../license.txt
//

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

[n_lignes, n_colonnes] = size(tab)
//- Largeur des colonnes
for i = 1 : n_colonnes
  //- Largeur maximale 
  largeur = max(length(tab(:, i)))
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
