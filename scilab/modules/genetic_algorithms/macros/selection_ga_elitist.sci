// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2008 - Yann COLLETTE <yann.collette@renault.com>
//
// Copyright (C) 2012 - 2016 - Scilab Enterprises
//
// This file is hereby licensed under the terms of the GNU GPL v2.0,
// pursuant to article 5.3.4 of the CeCILL v.2.1.
// This file was originally licensed under the terms of the CeCILL v2.1,
// and continues to be available under such terms.
// For more information, see the COPYING file which you should have received
// along with this program.

function [Pop_out,FObj_Pop_out,Efficiency,MO_Total_FObj_out] = selection_ga_elitist(Pop_in,Indiv1,Indiv2,FObj_Pop_in,FObj_Indiv1,FObj_Indiv2, ...
    MO_Total_FObj_in,MO_FObj_Indiv1,MO_FObj_Indiv2,param)

    [nargout,nargin] = argn();

    mo_is_defined = ~isempty("MO_Total_FObj_in");

    if ~isdef("param","local") then
        param = [];
    end

    [pressure,err] = get_param(param,"pressure",0.05);

    pop_size = length(Pop_in);

    Total_Pop  = lstcat(Pop_in, Indiv1, Indiv2);
    Total_FObj = [FObj_Pop_in' FObj_Indiv1' FObj_Indiv2']';

    // Normalization of the efficiency
    FObj_Pop_Max = max(Total_FObj);
    FObj_Pop_Min = min(Total_FObj);

    Efficiency = (1 - pressure) * (FObj_Pop_Max - Total_FObj)/max([FObj_Pop_Max - FObj_Pop_Min, %eps]) + pressure;
    [Efficiency, Index_sort] = gsort(Efficiency);
    Efficiency = Efficiency(1:pop_size);

    // Extraction and selection of the phenotype
    Total_FObj   = Total_FObj(Index_sort);
    FObj_Pop_out = Total_FObj(1:pop_size);
    // Extraction and selection of the genotype
    Total_Pop = list(Total_Pop(Index_sort));
    Pop_out   = list(Total_Pop(1:pop_size));
    // Extraction of the multiobjective values (if defined)
    if mo_is_defined then
        // MO_Total_FObj for multiobjective function values
        MO_Total_FObj_out = [MO_Total_FObj_in' MO_FObj_Indiv1' MO_FObj_Indiv2']';
        MO_Total_FObj_out = MO_Total_FObj_out(Index_sort,:);
        MO_Total_FObj_out = MO_Total_FObj_out(1:pop_size,:);
    end

    Total_Pop = list(); // Reinitialisation of Total_Pop
endfunction

