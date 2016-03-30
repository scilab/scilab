//
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2011 - Bernard DUJARDIN <develop.dujardin@numericable.fr>
//
// Copyright (C) 2012 - 2016 - Scilab Enterprises
//
// This file is hereby licensed under the terms of the GNU GPL v2.0,
// pursuant to article 5.3.4 of the CeCILL v.2.1.
// This file was originally licensed under the terms of the CeCILL v2.1,
// and continues to be available under such terms.
// For more information, see the COPYING file which you should have received
// along with this program.
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

