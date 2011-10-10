// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) INRIA - 1988 - C. Bunks
//
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt

function [ftype,forder,cfreq,wtype,fpar]=wfir_gui()
// Graphical user interface to get the wfir parameters interactively.
// this function is called by wfir
ftype=[];forder=[];cfreq=[];wtype=[];fpar=[];
//Query user for filter type and filter length
nc=x_choose([gettext("low pass");
        gettext("high pass");
        gettext("band pass");
        gettext("stop band")],gettext("Choose type of filter to be designed"))
//Select filter type and Query user for cut-off frequencies
flag=0;
select nc
case 0 then //user selected "Cancel"
    return
case 1 then //low pass
    while %t
        [ok,fl,forder]=getvalue(gettext("Input filter characteristics"),..
                [gettext("cut-off frequency (0.<frequ<.5):");
                gettext("filter length")],..
                list("vec",1,"vec",1),["0.2";"127"])
        if ~ok then return,end //user selected "Cancel"
        forder=round(forder)
        if forder<2 then
            messagebox(gettext("Incorrect filter order"),"modal","error");
            ok=%f
        end

        if fl<=0|fl>=0.5 then
            messagebox(gettext("Incorrect cut-off frequency"),"modal","error");
            ok=%f
        end
        if ok then break,end
    end
    ftype="lp"
    fh=0;
case 2 then //high pass
    while %t
        [ok,fl,forder]=getvalue(gettext("Input filter characteristics"),..
                [gettext("cut-off frequency (0.<frequ<.5):");
                gettext("filter length (odd value)")],..
                list("vec",1,"vec",1),["0.2";"127"])
        if ~ok then return,end //user selected "Cancel"
        forder=round(forder)
        if forder<2 then
            messagebox(gettext("Incorrect filter order"),"modal","error");
            ok=%f
        end
        if fl<=0|fl>=0.5 then
            messagebox(gettext("Incorrect cut-off frequency"),"modal","error");
            ok=%f
        end
        if ok then break,end
    end
    fh=0;
    flag=1;
    ftype="hp"
case 3 then //band pass
    while %t
        [ok,fl,fh,forder]=getvalue(gettext("Input filter characteristics"),..
                [gettext("low cut-off frequency (0.<flow<.5):");
                gettext("high cut-off frequency (0.<flow<fhi<.5):");
                gettext("filter length")],..
                list("vec",1,"vec",1,"vec",1),["0.1";"0.3";"127"])
        if ~ok then return,end //user selected "Cancel"
        forder=round(forder)
        if forder<2 then
            messagebox(gettext("Incorrect filter order"),"modal","error");
            ok=%f
        end
        if fl<=0|fl>=0.5 then
            messagebox(gettext("Incorrect low cut-off frequency"),"modal","error");
            ok=%f
        end
        if fh<=0|fh>=0.5|fh<fl then
            messagebox(gettext("Incorrect high cut-off frequency"),"modal","error");
            ok=%f
        end
        if ok then break,end
    end
    ftype="bp"
case 4 then //stop band
    while %t
        [ok,fl,fh,forder]=getvalue(gettext("Input filter characteristics"),..
                [gettext("low cut-off frequency (0.<flow<.5):");
                gettext("high cut-off frequency (0.<flow<fhi<.5):");
                gettext("filter length (odd value)")],..
                list("vec",1,"vec",1,"vec",1),["0.1";"0.2";"127"])
        if ~ok then return,end //user selected "Cancel"
        forder=round(forder)
        if forder<2 then
            messagebox(gettext("Incorrect filter order"),"modal","error");
            ok=%f
        end
        if fl<=0|fl>=0.5 then
            messagebox(gettext("Incorrect low cut-off frequency"),"modal","error");
            ok=%f
        end
        if fh<=0|fh>=0.5|fh<fl then
            messagebox(gettext("Incorrect high cut-off frequency"),"modal","error");
            ok=%f
        end
        if ok then break,end
    end
    flag=1;
    ftype="sb"
else
    return
end
cfreq=[fl fh];
if flag==1 then
    if forder-2*int(forder/2)==0 then
        messagebox([gettext("Even length high pass and stop band filters not allowed");
                gettext("---Filter order is being incremented by 1")],"modal","error");
        forder=forder+1;
    end
end

//Query user for window type and window parameters
nc=x_choose([gettext("Kaiser");
        gettext("Chebyshev");
        gettext("Rectangular");
        gettext("Triangular");
        gettext("Hamming") ],gettext("Input window type"))

select nc
case 0 then
    return; //user selected "Cancel"
case 1 then
    while %t
        [ok,Beta]=getvalue(gettext("Input window characteristics"),..
                ["beta>0"],list("vec",1)," ")
        if ~ok then return,end //user selected "Cancel"
        if Beta<=0 then
            messagebox(gettext("Incorrect beta value"),"modal","error");
            ok=%f
        end
        if ok then break,end
    end
    wtype="kr"
    fpar(1)=Beta
    fpar(2)=0;
case 2 then
    while %t
        [ok,name,value]=getvalue([gettext("Input window characteristics:");
                " ";
                gettext("dp (dp>0)   : the maximum value of the window side-lobe height");
                gettext("df (0<df<.5): the width of the window main lobe")
                " ";
                gettext("only one of this two values is to be defined,")
                gettext("the other one is automatically deduced")],..
                [gettext("name of specified value") gettext("value")],...
                list("str",-1,"vec",1),["dp","0.3"])
        if ~ok then return,end //user selected "Cancel"
        if or(part(name,1:2)<>["dp" "df"]) then
            messagebox(gettext("Incorrect name of specified value"),"modal","error");
            ok=%f
        elseif part(name,1:2)=="dp" then
            if value<=0 then
                messagebox(gettext("Incorrect  value"),"modal","error");
                ok=%f
            else
                fpar=[value,-1]
            end
        elseif part(name,1:2)=="df" then
            if value<=0|value>=0.5 then
                messagebox(gettext("Incorrect  value"),"modal","error");
                ok=%f
            else
                fpar=[-1,value]
            end
        end
        if ok then break,end
    end
    wtype="ch" 
case 3 then
    wtype="re"
    fpar=[0 0];
case 4 then
    wtype="tr"
    fpar=[0 0];
case  5 then
    wtype="hm"
    fpar=[0 0];
case  6 then
    wtype="hn"
    fpar=[0 0];
else
    return
end
endfunction
