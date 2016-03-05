// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2002-2004 - INRIA - Vincent COUVERT
//
// Copyright (C) 2012 - 2016 - Scilab Enterprises
//
// This file is hereby licensed under the terms of the GNU GPL v2.0,
// pursuant to article 5.3.4 of the CeCILL v.2.1.
// This file was originally licensed under the terms of the CeCILL v2.1,
// and continues to be available under such terms.
// For more information, see the COPYING file which you should have received
// along with this program.

function r=mtlb_eval(%s1,%s2)
    // Emulation function for Matlab eval()

    [lhs,rhs]=argn()

    %prot=funcprot();
    funcprot(0)

    // Init all variables useful to launch mfile2sci()
    deff("r=isanmfile(s)","r=%f");
    res_path="";
    logfile=-1;
    batch=%t;
    fnam="%fun";
    dquote="''";
    lcount=1;
    level=[0,0];
    sciexp=0;
    load("SCI/modules/m2sci/macros/kernel/lib");
    sciparam();

    // Scilab variable types
    Double=1;
    Boolean=4; // Boolean type can also be 6
    Sparse=5;
    Int=8;
    Handle=9;
    String=10;
    Cell=17;
    Struct=16;
    Void=0;
    Unknown=-1; // Unknown type or dimension
    SupToOne=-2; // Dimension >1
    NotNull=-3; // Dimension >0
    Complex=1 //"Complex"
    Real=0 //"Real"
    Units=["pixels","centimeters","points","inches","normalized"]

    margin="  "
    verbose_mode = 0;
    logfile=file("open",res_path+"m2sci_fun.log","unknown")
    global("varslist")
    varslist=m2sci_init()

    // Define a function which contains expression/instruction to eval and convert it
    deff("%fun()",%s1)
    mtlbtree=macr2tree(%fun);
    [scitree,%txt]=mtlbtree2sci(mtlbtree,%T)
    //%txt=tree2code(scitree)
    %txt(find(part(%txt,1:2)=="//"))=[]
    %txt(1)=[];
    %txt($)=[];

    %r=0
    %nold=size(who("get"),"*")
    %r=execstr(%txt,"errcatch")

    if %r<>0&rhs==2 then
        deff("%fun()",%s2)
        mtlbtree=macr2tree(%fun);
        [scitree,varslist]=mtlbtree2sci(mtlbtree,varslist)
        %txt=tree2code(scitree)
        %txt(find(part(%txt,1:2)=="//"))=[]
        %txt(1)=[];
        %txt($)=[];
        disp(%txt)

        %nold=size(who("get"),"*")
        %r=execstr(%txt,"errcatch")
    else
        %nold=%nold+1
    end

    file("close", logfile);

    nams=who("get");
    if size(nams,"*")<=%nold then
        r=0
        return
    end
    nams=nams(1:$-%nold)
    %r=funcprot(%prot);

    if nams=="ans" then
        r=ans
    else
        r=[];
        execstr("["+strcat(nams,",")+"]"+"=resume"+"("+strcat(nams,",")+")")
    end

endfunction
function [varslist]=m2sci_init()
    varslist=list()
    varslist($+1)=M2scivar("%i","i",Infer(list(1,1),Type(1,Complex)))
    varslist($+1)=M2scivar("%i","j",Infer(list(1,1),Type(1,Complex)))
    varslist($+1)=M2scivar("%nan","NaN",Infer(list(1,1),Type(1,Real)))
    varslist($+1)=M2scivar("%nan","nan",Infer(list(1,1),Type(1,Real)))
    varslist($+1)=M2scivar("%inf","Inf",Infer(list(1,1),Type(1,Real)))
    varslist($+1)=M2scivar("%inf","inf",Infer(list(1,1),Type(1,Real)))
    varslist($+1)=M2scivar("$","end",Infer(list(1,1),Type(1,Real)))
    varslist($+1)=M2scivar("%pi","pi",Infer(list(1,1),Type(1,Real)))
    varslist($+1)=M2scivar("%eps","eps",Infer(list(1,1),Type(1,Real)))
endfunction
