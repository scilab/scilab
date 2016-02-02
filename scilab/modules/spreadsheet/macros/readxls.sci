
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2005-2008 - INRIA - Serge STEER <serge.steer@inria.fr>
// Copyright (C) 2005- INRIA - Pierrick Mode
//
// Copyright (C) 2012 - 2016 - Scilab Enterprises
//
// This file is hereby licensed under the terms of the GNU GPL v2.0,
// pursuant to article 5.3.4 of the CeCILL v.2.1.
// This file was originally licensed under the terms of the CeCILL v2.1,
// and continues to be available under such terms.
// For more information, see the COPYING file which you should have received
// along with this program.

function sheets=readxls(fil)
    if argn(2)==1 then
        opt=1;
    end

    fil=pathconvert(fil,%f,%t);
    sheets=mlist(["xls","sheets"],list());
    [fd,Text,Sheetnames,Abspos]=xls_open(fil);
    Text=["", Text];

    for k=1:size(Abspos,"*")
        [Value,TextInd]=xls_read(fd,Abspos(k));
        if Value==[] then
            sheet = mlist(["xlssheet","name","text","value"],Sheetnames(k),[],Value)
        else
            if size(Text,"*")==1 then
                T=emptystr(Value);
            else
                T=matrix(Text(TextInd+1),size(Value));
            end

            sheet=mlist(["xlssheet","name","text","value"],Sheetnames(k),T,Value);
        end
        sheets.sheets($+1) = sheet;
    end

    mclose(fd)
endfunction
