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

function [pop_out] = coding_ga_binary(pop_in,direction,param)
    // pop_in is a list().
    // pop_out must be a list().
    if ~isdef("param","local") then
        param = [];
    end

    // We deal with some parameters to take into account the boundary of the domain and the neighborhood size
    [BinLen,err]    = get_param(param,"binary_length",8);
    [MinBounds,err] = get_param(param,"minbound",-2*ones(size(pop_in(1),1),size(pop_in(1),2)));
    [MaxBounds,err] = get_param(param,"maxbound",2*ones(size(pop_in(1),1),size(pop_in(1),2)));

    pop_out   = list();
    Dimension = length(pop_in(1));
    Max_Bin   = 2^BinLen-1;
    // A template of '0' so as to be sure that the binary code will be 8 bits long
    template  = strsubst(dec2bin(Max_Bin,BinLen),"1","0");

    if (direction=="code") then
        for i=1:length(pop_in)
            str_tmp = "";
            for j=1:Dimension
                tmp = dec2bin(int(Max_Bin * (pop_in(i)(j) - MinBounds(j)) / (MaxBounds(j) - MinBounds(j))),BinLen);
                tmp = strcat([part(template,1:BinLen-length(tmp)) tmp]);
                str_tmp = strcat([str_tmp, tmp]);
            end
            pop_out(i) = str_tmp;
        end
    elseif (direction=="decode") then
        for i=1:length(pop_in)
            pop_out(i) = zeros(size(MaxBounds,1),size(MaxBounds,2));
            for j=1:length(MaxBounds)
                pop_out(i)(j) = (MaxBounds(j) - MinBounds(j)) * (bin2dec(part(pop_in(i),(j-1)*BinLen+1:j*BinLen)) / Max_Bin) + MinBounds(j);
            end
        end
    else
        error(sprintf(gettext("%s: wrong direction"),"coding_ga_binary"));
    end
endfunction
