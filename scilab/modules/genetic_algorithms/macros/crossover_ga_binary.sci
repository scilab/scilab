// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2008 - Yann COLLETTE <yann.collette@renault.com>
// Copyright (C) 2014 - Michael BAUDIN <michael.baudin@contrib.scilab.org>
// Copyright (C) Scilab Enterprises - 2014 - Pierre-Aime AGNEL
//
// Copyright (C) 2012 - 2016 - Scilab Enterprises
//
// This file is hereby licensed under the terms of the GNU GPL v2.0,
// pursuant to article 5.3.4 of the CeCILL v.2.1.
// This file was originally licensed under the terms of the CeCILL v2.1,
// and continues to be available under such terms.
// For more information, see the COPYING file which you should have received
// along with this program.

//==============================================================================
// crossover_ga_binary
//==============================================================================
// INPUT
//    Indiv1 = A string representing a binary code
//    Indiv2 = A strings representing a binary code
//    param  = A plist with the following parameters
//        binary_length  = an integer, the length of the binary code (def. 8)
//        multi_cross    = a boolean, flag for multiple crossovers (def. %f)
//        multi_cross_nb = an integer, number of multiple crossovers (def. 2)
//
// OUTPUT
//    Crossed_Indiv1 = A string,
//                     result of the cross over for Indiv1
//    Crossed_Indiv2 = A string,
//                     result of the cross over for Indiv2
//    mix            = a vector of double, the positions
//                      where the crossover was done
//
// DESCRIPTION
//    Function crossover_ga_binary generates the crossover between
//    Indiv1 and Indiv2 taken as binary code.
//
//    Without any additional parameters crossover_ga_binary(Indiv1, Indiv2)
//    returns the crossover occruring on a single random location
//
//    crossover_ga_binary(Indiv1, Indiv2, param) modifies its behaviour
//    with the values of param:
//        binary_length:
//            length of the binary code (default 8)
//        multi_cross:
//            if set to %t multiple crossover can happen (default %f)
//        multi_cross_nb:
//            the maximal number of possible crossovers.
//            (default 2 if
//            multi_cross is set to %t, 1 otherwise)
//            At least one crossover will occur.

function [Crossed_Indiv1, Crossed_Indiv2, mix] = crossover_ga_binary(Indiv1,Indiv2,param)

    fname = "crossover_ga_binary";
    if ~isdef("param","local") then
        param = [];
    end

    // We deal with some parameters to take into account the boundary of the domain and the neighborhood size
    [BinLen, errLen] = get_param(param, "binary_length", 8);
    [MultiCross, errMultiCross] = get_param(param, "multi_cross", %F);
    if MultiCross
        [MultiCrossNb, errMultiCrossNb] = get_param(param, "multi_cross_nb", 2);
    else
        MultiCrossNb = 1;
    end

    len1 = length(Indiv1);
    len2 = length(Indiv2);
    BinLen = max([len1, len2, BinLen]);
    if len1 < BinLen
        Indiv1 = strcat(string(zeros(1, BinLen - len1))) + Indiv1; //0 padding on the right
    end
    if len2 < BinLen
        Indiv2 =  strcat(string(zeros(1, BinLen - len2))) + Indiv2; //0 padding on the right
    end

    // Crossover positions selection
    mix = unique(gsort(sample(MultiCrossNb, 1:BinLen-1), "g", "i"))';
    Crossed_Indiv1 = Indiv1;
    Crossed_Indiv2 = Indiv2;

    for j = 1:size(mix, "*")
        H1 = part(Crossed_Indiv1, 1:mix(j)); //Head for Indiv1
        T1 = part(Crossed_Indiv1, (mix(j) + 1):BinLen); //Tail for Indiv1
        H2 = part(Crossed_Indiv2, 1:mix(j)); //Head for Indiv2
        T2 = part(Crossed_Indiv2, (mix(j) + 1):BinLen); //Tail for Indiv2

        Crossed_Indiv1 = [H1 + T2];
        Crossed_Indiv2 = [H2 + T1];
    end
endfunction
