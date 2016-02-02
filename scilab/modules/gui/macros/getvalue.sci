// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) INRIA
// Copyright (C) 2010 - DIGITEO - Allan CORNET
// Copyright (C) 2010 - DIGITEO - Clément DAVID <clement.david@scilab.org>
// Copyright (C) 2011 - DIGITEO - Bruno JOFRET
//
// Copyright (C) 2012 - 2016 - Scilab Enterprises
//
// This file is hereby licensed under the terms of the GNU GPL v2.0,
// pursuant to article 5.3.4 of the CeCILL v.2.1.
// This file was originally licensed under the terms of the CeCILL v2.1,
// and continues to be available under such terms.
// For more information, see the COPYING file which you should have received
// along with this program.

function [%ok,%1,%2,%3,%4,%5,...
    %6,%7,%8,%9,%10,...
    %11,%12,%13,%14,%15,...
    %16,%17,%18,%19,%20,...
    %21,%22,%23,%24,%25,...
    %26,%27,%28,%29,%30]=getvalue(%desc,%labels,%typ,%ini)
    //  getvalues - %window dialog for data acquisition
    //%Synta%
    //  [%ok,%1,..,%30]=getvalue(desc,labels,typ,ini)
    //%Parameters
    //  desc    : column vector of strings, dialog general comment
    //  labels  : n column vector of strings, labels(i) is the label of
    //            the ith required value
    //  typ     : list(typ1,dim1,..,typn,dimn)
    //            typi : defines the type of the ith required value
    //                   if may have the following values:
    //                   'mat'    : stands for matrix of scalars
    //                   'col'    : stands for column vector of scalars
    //                   'row'    : stands for row vector of scalars
    //                   'vec'    : stands for  vector of scalars
    //                   'intvec' : stands for  vector of integers i.e int(x) == x
    //                   'str'    : stands for vector of strings
    //                   'lis'    : stands for list
    //                   'pol'    : stands for polynomials
    //                   'r'      : stands for rational
    //            dimi : defines the size of the ith required value
    //                   it must be
    //                    - an integer or a 2-vector of integers (-1 stands for
    //                      arbitrary dimension)
    //                    - an evaluatable character string
    //  ini     : n column vector of strings, ini(i) gives the suggested
    //            response for the ith required value
    //  %ok      : boolean ,%t if %ok button pressed, %f if cancel button pressed
    //  xi      : contains the ith required value if %ok==%t
    //%Description
    // getvalues macro encapsulate x_mdialog function with error checking,
    // evaluation of numerical response, ...
    //%Remarks
    // All correct scilab syntax may be used as responses, for matrices
    // and vectors getvalues automatically adds [ ] around the given response
    // before numerical evaluation
    //%Example
    // labels=['magnitude';'frequency';'phase    '];
    // [ampl,Freq,ph]=getvalue('define sine signal',labels,..
    //            list('vec',1,'vec',1,'vec',1),['0.85';'10^2';'%pi/3'])
    //
    //%See also
    // x_mdialog, x_dialog

    //!
    // 01/09/10 Clément DAVID: update to %30 rhs parameters
    // 17/01/07 -Alan- This version of getvalue is different of scilab 4.1 :
    //         - %scicos_context behavior reviewed in accordance to context_evstr macro
    //         - (u)int(8/16/32) allowed in field of type vec/mat/row/col (F. Nassif's Work)
    //
    // 05/02/07 -Alan- : update to %20 rhs parameters
    //
    // 12/02/07 -Alan- : fix (variable evaluation of %scicos_context)
    //
    // Copyright INRIA
    [%lhs, %rhs] = argn(0)
    if %rhs < 3 then
        error(sprintf(_("%s: Wrong number of input argument(s): %d to %d expected.\n"), "getvalue", 3, 4));
    end

    %nn=prod(size(%labels))
    if %lhs<>%nn+2&%lhs<>%nn+1 then error(41),end
    if size(%typ)<>2*%nn then
        error("%typ : list(''type'',[sizes],...)")
    end
    %1=[];%2=[];%3=[];%4=[];%5=[];
    %6=[];%7=[];%8=[];%9=[];%10=[];
    %11=[];%12=[];%13=[];%14=[];%15=[];
    %16=[];%17=[];%18=[];%19=[];%20=[];
    %21=[];%22=[];%23=[];%24=[];%25=[];
    %26=[];%27=[];%28=[];%29=[];%30=[];

    if exists("%scicos_context") then
        %mm=getfield(1,%scicos_context)
        for %mi=%mm(3:$)
            if execstr(%mi+"=%scicos_context(%mi)","errcatch")<>0 then
                disp(lasterror())
                %ok=%f
                return
            end
        end
    end

    if %rhs==3 then  %ini=emptystr(%nn,1),end
    %ok=%t
    while %t do
        %str=x_mdialog(%desc,%labels,%ini)
        if %str==[] then %ok=%f,%str=[];break,end
        for %kk=1:%nn
            %cod=ascii(%str(%kk))
            %spe=find(%cod==10)
            if %spe<>[] then
                %semi=ascii(";")
                %cod(%spe)=%semi*ones(%spe')
                %str(%kk)=ascii(%cod)
            end
        end
        %nok=0
        for %kk=1:%nn
            select part(%typ(2*%kk-1),1:6)
            case "mat   "
                %ierr=execstr("%vv=["+%str(%kk)+"]","errcatch");
                if %ierr<>0 then %nok=-%kk;break,end
                //29/12/06
                //the type of %vv is accepted if it is constant or integer
                if and(type(%vv)<>[1 8]) then %nok=-%kk,break,end
                %sz=%typ(2*%kk);if type(%sz)==10 then %sz=evstr(%sz),end
                [%mv,%nv]=size(%vv)
                %ssz=string(%sz(1))+" x "+string(%sz(2))
                if %mv*%nv==0 then
                    if  %sz(1)>=0&%sz(2)>=0&%sz(1)*%sz(2)<>0 then %nok=%kk,break,end
                else
                    if %sz(1)>=0 then if %mv<>%sz(1) then %nok=%kk,break,end,end
                    if %sz(2)>=0 then if %nv<>%sz(2) then %nok=%kk,break,end,end
                end
            case "vec   "
                %ierr=execstr("%vv=["+%str(%kk)+"]","errcatch")
                if %ierr<>0 then %nok=-%kk;break,end
                //17/01/07
                //the type of %vv is accepted if it is constant or integer
                if and(type(%vv)<>[1 8]) then %nok=-%kk,break,end
                %sz=%typ(2*%kk);if type(%sz)==10 then %sz=evstr(%sz),end
                %ssz=string(%sz(1))
                %nv=prod(size(%vv))
                if %sz(1)>=0 then if %nv<>%sz(1) then %nok=%kk,break,end,end
            case "intvec"
                %ierr=execstr("%vv=["+%str(%kk)+"]","errcatch")
                if %ierr<>0 then %nok=-%kk;break,end
                //17/01/07
                //the type of %vv is accepted if it is constant or integer
                if and(type(%vv)<>[1 8]) then %nok=-%kk,break,end
                %sz=%typ(2*%kk);if type(%sz)==10 then %sz=evstr(%sz),end
                if and(int(%vv) == %vv) == %f then
                    %nok=-%kk;
                    break;
                end
                %ssz=string(%sz(1))
                %nv=prod(size(%vv))
                if %sz(1)>=0 then if %nv<>%sz(1) then %nok=%kk,break,end,end
            case "pol   "
                %ierr=execstr("%vv=["+%str(%kk)+"]","errcatch");
                if %ierr<>0 then %nok=-%kk;break,end
                if type(%vv)>2 then %nok=-%kk,break,end
                %sz=%typ(2*%kk);if type(%sz)==10 then %sz=evstr(%sz),end
                %ssz=string(%sz(1))
                %nv=prod(size(%vv))
                if %sz(1)>=0 then if %nv<>%sz(1) then %nok=%kk,break,end,end
            case "row   "
                %ierr=execstr("%vv=["+%str(%kk)+"]","errcatch");
                if %ierr<>0 then %nok=-%kk;break,end
                //17/01/07
                //the type of %vv is accepted if it is constant or integer
                if and(type(%vv)<>[1 8]) then %nok=-%kk,break,end
                %sz=%typ(2*%kk);if type(%sz)==10 then %sz=evstr(%sz),end
                if %sz(1)<0 then
                    %ssz="1 x *"
                else
                    %ssz="1 x "+string(%sz(1))
                end
                [%mv,%nv]=size(%vv)
                if %mv<>1 then %nok=%kk,break,end,
                if %sz(1)>=0 then if %nv<>%sz(1) then %nok=%kk,break,end,end
            case "col   "
                %ierr=execstr("%vv=["+%str(%kk)+"]","errcatch");
                if %ierr<>0 then %nok=-%kk;break,end
                //17/01/07
                //the type of %vv is accepted if it is constant or integer
                if and(type(%vv)<>[1 8]) then %nok=-%kk,break,end
                %sz=%typ(2*%kk);if type(%sz)==10 then %sz=evstr(%sz),end
                if %sz(1)<0 then
                    %ssz="* x 1"
                else
                    %ssz=string(%sz(1))+" x 1"
                end
                [%mv,%nv]=size(%vv)
                if %nv<>1 then %nok=%kk,break,end,
                if %sz(1)>=0 then if %mv<>%sz(1) then %nok=%kk,break,end,end
            case "str   "
                %sde=%str(%kk)
                %spe=find(ascii(%str(%kk))==10)
                %spe($+1)=length(%sde)+1
                %vv=[];%kk1=1
                for %kkk=1:size(%spe,"*")
                    %vv(%kkk,1)=part(%sde,%kk1:%spe(%kkk)-1)
                    %kk1=%spe(%kkk)+1
                end
                %sz=%typ(2*%kk);if type(%sz)==10 then %sz=evstr(%sz),end
                %ssz=string(%sz(1))
                %nv=prod(size(%vv))
                if %sz(1)>=0 then if %nv<>%sz(1) then %nok=%kk,break,end,end
            case "lis   "
                %ierr=execstr("%vv="+%str(%kk),"errcatch");
                if %ierr<>0 then %nok=-%kk;break,end
                if type(%vv)<>15& type(%vv)<>16 then %nok=-%kk,break,end
                %sz=%typ(2*%kk);if type(%sz)==10 then %sz=evstr(%sz),end
                %ssz=string(%sz(1))
                %nv=size(%vv)
                if %sz(1)>=0 then if %nv<>%sz(1) then %nok=%kk,break,end,end
            case "r     "
                %ierr=execstr("%vv=["+%str(%kk)+"]","errcatch");
                if %ierr<>0 then %nok=-%kk;break,end
                if type(%vv)<>16 then %nok=-%kk,break,end
                if typeof(%vv)<>"rational" then %nok=-%kk,break,end
                %sz=%typ(2*%kk);if type(%sz)==10 then %sz=evstr(%sz),end
                [%mv,%nv]=size(%vv(2))
                %ssz=string(%sz(1))+" x "+string(%sz(2))
                if %mv*%nv==0 then
                    if  %sz(1)>=0&%sz(2)>=0&%sz(1)*%sz(2)<>0 then %nok=%kk,break,end
                else
                    if %sz(1)>=0 then if %mv<>%sz(1) then %nok=%kk,break,end,end
                    if %sz(2)>=0 then if %nv<>%sz(2) then %nok=%kk,break,end,end
                end
            else
                error(msprintf(_("%s: Type %s is not implemented.\n"),"getvalue", %typ(2*%kk-1)))
            end
            execstr("%"+string(%kk)+"=%vv")
        end
        if %nok>0 then
            messagebox(msprintf(_("Answer given for %s \n has invalid dimension: \n waiting for dimension %s.\n"), %labels(%nok), %ssz), "modal");
            %ini=%str
        elseif %nok<0 then
            if %ierr==0 then
                messagebox(msprintf(_("Answer given for %s \n has incorrect type %s.\n"), %labels(-%nok), %typ(-2*%nok-1)), "modal");
            else
                messagebox(msprintf(_("Answer given for %s \n is incorrect: %s.\n"), %labels(-%nok), lasterror()), "modal");
            end
            %ini=%str
        else
            break
        end
    end
    if %lhs==%nn+2 then
        execstr("%"+string(%lhs-1)+"=%str")
    end
endfunction
