//  Scicos
//
//  Copyright (C) INRIA - Author : EADS-CCR
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
// Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301, USA.
//
// See the file ../license.txt
//

function type_meth=arbre_decision(delta)
    // Cette fonction renvoie le type de la m�thode //
    // apres le percour du fichier xml en testant   //
    // le signe du discriminant                     //
    // entr�e :                                     //
    //   - delat (Double) : discriminant de l'EDP   //
    // sortie :                                     //
    //   - type_meth (Entier) correspond au code de //
    //     la m�thode de discr�tisation.            //
    //----------------------------------------------//
    nom="meth.xml";
    // lecture du fichier xml
    txt_xml=lecture_xml(nom);
    decision=interpret(txt_xml,delta,1)
    // interpretation de la decision
    execstr("deci= ("+decision+")");
    if (deci == "type_meth = 1") then
        type_meth = 1;
    elseif (deci == "type_meth = 3") then
        type_meth = 3;
    else
        type_meth = 2;
    end
endfunction

