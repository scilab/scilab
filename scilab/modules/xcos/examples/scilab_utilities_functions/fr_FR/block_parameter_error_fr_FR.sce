//
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2011 - Bernard DUJARDIN <develop.dujardin@numericable.fr>
//
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
//
//

loadXcosLibs // Inutile dans un contexte de programmation de bloc Xcos

ok = %f;

while ~ok do
    // Entrée du paramètre
    [Datatype] = x_mdialog( ..
        "Entrée de paramètre", ..
        "Type de données (3:int32, 4:int16, 5:int8, ...) ?", ..
        "3" ..
    );

    if isempty(Datatype) then // Annuler ou OK ?
        break

    else
        Datatype = evstr(Datatype);

    end
    // Tester le paramètre 'Data Type et afficher une boîte d'erreur si nécessaire
    if Datatype < 3 | Datatype > 8 then
        block_parameter_error( ..
            msprintf("Valeur erronée pour le paramètre ''Type de données'' : %d.", Datatype), ..
            msprintf("Doit être dans l''intervalle %s.", "[3, 8]") ..
        );
        ok = %f;

    else // Le paramètre est valide continuer le travail
        ok = %t;
        print(%io(2), "Le paramètre ''Type de données'' est valide");
    end
end

