// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) INRIA
//
// Copyright (C) 2012 - 2016 - Scilab Enterprises
//
// This file is hereby licensed under the terms of the GNU GPL v2.0,
// pursuant to article 5.3.4 of the CeCILL v.2.1.
// This file was originally licensed under the terms of the CeCILL v2.1,
// and continues to be available under such terms.
// For more information, see the COPYING file which you should have received
// along with this program.

function %x=integrate(%expr,%var,%x0,%x1,%ea,%er)
    // x=integrate(expr,v,x0,x1 [,ea [,er]])  computes
    //                      /x1
    //                     [
    //                 x = I  f(v)dv
    //                     ]
    //                    /x0
    //
    //
    //examples:
    //integrate('sin(x)','x',0,%pi)
    //integrate(['if x==0 then 1,';
    //           'else sin(x)/x,end'],'x',0,%pi)
    rhs = argn(2);

    if rhs < 4 then
        error(msprintf(gettext("%s: Wrong number of input argument(s): At least %d expected.\n"),"integrate",4));
    end

    select rhs
    case 4 then
        %ea=1d-14;%er=1.d-8
    case 5 then
        %er=1d-14;
    end

    if size(%x0,"*")<>1 then
        error(msprintf(gettext("%s: Wrong size for input argument #%d: A real expected.\n"),"integrate",3));
    end

    if size(%ea,"*")<>1 then
        error(msprintf(gettext("%s: Wrong size for input argument #%d: A real expected.\n"),"integrate",5));
    end

    if size(%er,"*")<>1 then
        error(msprintf(gettext("%s: Wrong size for input argument #%d: A real expected.\n"),"integrate",6));
    end

    if imag(%x0)<>0 then
        error(msprintf(gettext("%s: Wrong type for input argument #%d: A real expected.\n"),"integrate",3));
    end

    if norm(imag(%x1),1)<>0 then
        error(msprintf(gettext("%s: Wrong type for input argument #%d: A real expected.\n"),"integrate",4));
    else
        %x1=real(%x1)
    end
    //

    try
        if %expr==%var then
            deff(%var+"=%func("+%var+")",%expr)
        else
            deff("ans=%func("+%var+")",%expr)
        end
    catch
        error(msprintf(gettext("%s: Wrong value for input argument #%d: syntax error in given expression\n"),"integrate",1));
    end

    if strstr(%expr,"%i") <> "" then
        error(msprintf(_("%s: Wrong value for input argument #%d: A real expected.\n"),"integrate",1));
    end

    [%x1,%ks]=gsort(%x1,"g","i")
    %x=zeros(%x1)

    %kkk=find((%x1(1:$-1)<%x0) & (%x1(2:$)>=%x0))
    if %kkk <>[] then
        %xx0=%x0;
        for %kk=1:%kkk
            %x(%kk)=-intg(%xx0,%x1(%kk),%func,%ea,%er);
            %xx0=%x1(%kk);
        end
    end
    %xx0=%x0;
    for %kk=1:size(%x1,"*")
        %x(%kk)=intg(%xx0,%x1(%kk),%func,%ea,%er);
        %xx0=%x1(%kk);
    end
    %x=cumsum(%x)
    %x=matrix(%x(%ks),size(%x1));

endfunction
