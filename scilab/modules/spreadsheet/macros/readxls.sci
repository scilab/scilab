
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2005-2008 - INRIA - Serge STEER <serge.steer@inria.fr>
// Copyright (C) 2005- INRIA - Pierrick Mode
//
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt

function sheets=readxls(fil)
    if argn(2)==1 then opt=1,end
    fil=pathconvert(fil,%f,%t)
    sheets=mlist(["xls","sheets"],list())
    [fd,Text,Sheetnames,Abspos]=xls_open(fil)
    Text=["", Text];
    for k=1:size(Abspos,"*")
        [Value,TextInd]=xls_read(fd,Abspos(k))
        if Value==[] then
            sheet=mlist(["xlssheet","name","text","value"],Sheetnames(k),[],Value)
        else
            if size(Text,"*")==1 then
                T=emptystr(Value)
            else
                T=matrix(Text(TextInd+1),size(Value))
            end
            sheet=mlist(["xlssheet","name","text","value"],Sheetnames(k),T,Value)
        end
        sheets.sheets($+1)= sheet
    end
    mclose(fd)
endfunction
