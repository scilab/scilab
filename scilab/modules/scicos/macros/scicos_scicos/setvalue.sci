//  Scicos
//
//  Copyright (C) INRIA - METALAU Project <scicos@inria.fr>
//                      - Alan Layec <alan.layec@inria.fr>
//  Copyright (C) 2010 - DIGITEO - Clément DAVID <clement.david@scilab.org>
//
// This program is free software; you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation; either version 2 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program; if not, write to the Free Software
// Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301, USA.
//
// See the file ../license.txt
//

function [%ok,%1,%2,%3,%4,%5,...
    %6,%7,%8,%9,%10,...
    %11,%12,%13,%14,%15,...
    %16,%17,%18,%19,%20,...
    %21,%22,%23,%24,%25,...
    %26,%27,%28,%29,%30]=setvalue(%desc,%labels,%typ,%ini)

    // To avoid infinite loops in set section of blocks during eval
    if %scicos_prob==%t then
        %ok=%f
        [%1,%2,%3,%4,%5,...
        %6,%7,%8,%9,%10,...
        %11,%12,%13,%14,%15,...
        %16,%17,%18,%19,%20,...
        %21,%22,%23,%24,%25,...
        %26,%27,%28,%29,%30]=(0,0,0,0,0,...
        0,0,0,0,0,...
        0,0,0,0,0,...
        0,0,0,0,0,...
        0,0,0,0,0,...
        0,0,0,0,0)
    return;end
    //  setvalues -  data acquisition, getvalue equivalent without dialog
    //%Syntax
    //  [%ok,x1,..,x18]=setvalue(desc,labels,typ,ini)
    //%Parameters
    //  desc    : column vector of strings, dialog general comment
    //  labels  : n column vector of strings, labels(i) is the label of
    //            the ith required value
    //  typ     : list(typ1,dim1,..,typn,dimn)
    //            typi : defines the type of the ith required value
    //                   if may have the following values:
    //                   'mat' : stands for matrix of scalars
    //                   'col' : stands for column vector of scalars
    //                   'row' : stands for row vector of scalars
    //                   'vec' : stands for  vector of scalars
    //                   'str' : stands for string
    //                   'lis' : stands for list
    //                   'pol' : stands for polynomials
    //                   'r'   : stands for rational
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
    // getvalues function uses ini strings to evaluate required args
    // with error checking,
    //%Remarks
    // All correct scilab syntax may be used as responses, for matrices
    // and vectors getvalues automatically adds [ ] around the given response
    // before numerical evaluation
    //%Example
    // labels=['magnitude';'frequency';'phase    '];
    // [ampl,freq,ph]=setvalue('define sine signal',labels,..
    //            list('vec',1,'vec',1,'vec',1),['0.85';'10^2';'%pi/3'])
    //
    //%See also
    // x_mdialog, dialog
    //!
    // 17/01/07 -Alan- - %scicos_context behavior reviewed in accordance to context_evstr macro
    //                 - pass int in field of type vec/mat/row/col (F. Nassif's Work)
    //
    // 05/02/07 -Alan- : update to %20 rhs parameters
    //
    // Copyright INRIA
    [%lhs,%rhs]=argn(0)

    %nn=prod(size(%labels))
    if %lhs<>%nn+2&%lhs<>%nn+1 then error(41),end
    if size(%typ)<>2*%nn then
        error("typ : list(''type'',[sizes],...)")
    end
    %1=[];%2=[];%3=[];%4=[];%5=[];
    %6=[];%7=[];%8=[];%9=[];%10=[];
    %11=[];%12=[];%13=[];%14=[],%15=[];
    %16=[];%17=[];%18=[];%19=[],%20=[];
    %21=[];%22=[];%23=[];%24=[],%25=[];
    %26=[];%27=[];%28=[];%29=[],%30=[];

    if %rhs==3 then  %ini=emptystr(%nn,1),end
    %ok=%t
    while %t do
        %str=%ini;
        if %str==[] then %ok=%f,break,end
        for %kk=1:%nn
            %cod=ascii(%str(%kk))
            %spe=find(%cod==10)
            if %spe<>[] then
                %semi=ascii(";")
                %cod(%spe)=%semi*ones(%spe')
                %str(%kk)=ascii(%cod)
            end
        end

        [%vv_list,%ierr_vec]=context_evstr(%str,%scicos_context,%typ);

        %noooo=0
        for %kk=1:%nn
            %vv=%vv_list(%kk)
            %ierr=%ierr_vec(%kk)
            select part(%typ(2*%kk-1),1:6)
            case "mat   "
                if %ierr<>0  then
                    %noooo=-%kk,break,
                end
                //29/12/06
                //the type of %vv is accepted if it is constant or integer
                if and(type(%vv)<>[1 8]) then %nok=-%kk,break,end
                %sz=%typ(2*%kk);if type(%sz)==10 then %sz=evstr(%sz),end
                [%mmmm,%nnnnn]=size(%vv)
                %ssss=string(%sz(1))+"-by-"+string(%sz(2)) + " matrix"
                if %mmmm*%nnnnn==0 then
                    if  %sz(1)>=0&%sz(2)>=0&%sz(1)*%sz(2)<>0 then %noooo=%kk,break,end
                else
                    if %sz(1)>=0 then if %mmmm<>%sz(1) then %noooo=%kk,break,end,end
                    if %sz(2)>=0 then if %nnnnn<>%sz(2) then %noooo=%kk,break,end,end
                end
            case "vec   "
                if %ierr<>0  then
                    %noooo=-%kk,break,
                end
                //17/01/07
                //the type of %vv is accepted if it is constant or integer
                if and(type(%vv)<>[1 8]) then %nok=-%kk,break,end
                %sz=%typ(2*%kk);if type(%sz)==10 then %sz=evstr(%sz),end
                %ssss=string(%sz(1))
                %nnnnn=prod(size(%vv))
                if %sz(1)>=0 then if %nnnnn<>%sz(1) then %noooo=%kk,break,end,end
            case "intvec"
                if %ierr<>0  then
                    %noooo=-%kk,break,
                end
                //the type of %vv is accepted if it is constant or integer
                if and(type(%vv)<>[1 8]) then %nok=-%kk,break,end
                %sz=%typ(2*%kk);if type(%sz)==10 then %sz=evstr(%sz),end
                %ssss=string(%sz(1))
                %nnnnn=prod(size(%vv))
                if %sz(1)>=0 then if %nnnnn<>%sz(1) then %noooo=%kk,break,end,end
            case "pol   "
                if %ierr<>0  then
                    %noooo=-%kk,break,
                end
                if %ierr<>0 then %noooo=-%kk;break,end
                if (type(%vv)>2 & type(%vv)<>8) then %noooo=-%kk,break,end
                %sz=%typ(2*%kk);if type(%sz)==10 then %sz=evstr(%sz),end
                %ssss=string(%sz(1))
                %nnnnn=prod(size(%vv))
                if %sz(1)>=0 then if %nnnnn<>%sz(1) then %noooo=%kk,break,end,end
            case "row   "
                if %ierr<>0  then
                    %noooo=-%kk,break,
                end
                //17/01/07
                //the type of %vv is accepted if it is constant or integer
                if and(type(%vv)<>[1 8]) then %nok=-%kk,break,end
                %sz=%typ(2*%kk);if type(%sz)==10 then %sz=evstr(%sz),end
                if %sz(1)<0 then
                    %ssss="1-by-n matrix"
                else
                    %ssss="1-by-"+string(%sz(1))+" matrix"
                end
                [%mmmm,%nnnnn]=size(%vv)
                if %mmmm<>1 then %noooo=%kk,break,end,
                if %sz(1)>=0 then if %nnnnn<>%sz(1) then %noooo=%kk,break,end,end
            case "col   "
                if %ierr<>0  then
                    %noooo=-%kk,break,
                end
                //17/01/07
                //the type of %vv is accepted if it is constant or integer
                if and(type(%vv)<>[1 8]) then %nok=-%kk,break,end
                %sz=%typ(2*%kk);if type(%sz)==10 then %sz=evstr(%sz),end
                if %sz(1)<0 then
                    %ssss="m-by-1 matrix"
                else
                    %ssss=string(%sz(1))+"-by-1 matrix"
                end
                [%mmmm,%nnnnn]=size(%vv)
                if %nnnnn<>1 then %noooo=%kk,break,end,
                if %sz(1)>=0 then if %nnnnn<>%sz(1) then %noooo=%kk,break,end,end
            case "str   "
                clear %vv
                %vv=%str(%kk)
                if type(%vv)<>10 then %noooo=-%kk,break,end
                %sz=%typ(2*%kk);if type(%sz)==10 then %sz=evstr(%sz),end
                %ssss=string(%sz(1))
                %nnnnn=prod(size(%vv))
                if %sz(1)>=0 then if %nnnnn<>1 then %noooo=%kk,break,end,end
            case "lis   "
                if %ierr<>0  then
                    %noooo=-%kk,break,
                end
                if type(%vv)<>15& type(%vv)<>16& type(%vv)<>17 then %noooo=-%kk,break,end
                %sz=%typ(2*%kk);if type(%sz)==10 then %sz=evstr(%sz),end
                %ssss=string(%sz(1))
                %nnnnn=size(%vv)
                if %sz(1)>=0 then if %nnnnn<>%sz(1) then %noooo=%kk,break,end,end
            case "r     "
                if %ierr<>0  then
                    %noooo=-%kk,break,
                end
                if type(%vv)<>16 then %noooo=-%kk,break,end
                if typeof(%vv)<>"rational" then %noooo=-%kk,break,end
                %sz=%typ(2*%kk);if type(%sz)==10 then %sz=evstr(%sz),end
                [%mmmm,%nnnnn]=size(%vv(2))
                %ssss=string(%sz(1))+"-by-"+string(%sz(2))
                if %mmmm*%nnnnn==0 then
                    if  %sz(1)>=0&%sz(2)>=0&%sz(1)*%sz(2)<>0 then %noooo=%kk,break,end
                else
                    if %sz(1)>=0 then if %mmmm<>%sz(1) then %noooo=%kk,break,end,end
                    if %sz(2)>=0 then if %nnnnn<>%sz(2) then %noooo=%kk,break,end,end
                end
            case "gen   "
                //accept all
            else
                str = gettext("%s: Type %s is not implemented.\n");
                mess = msprintf(str, arg1.gui + "(''set'')", %typ(2*%kk-1));
                if length(arg1.doc) > 0 then
                    warnBlockByUID(arg1.doc(1), mess); // arg1 is from the block interface function
                else
                    error(mess);
                end
            end
            execstr("%"+string(%kk)+"=%vv")
            clear %vv
        end
        if %noooo>0 then
            str = gettext("%s: Wrong size for block parameter ''%s'': %s expected, getting %s");
            mess = msprintf(str, arg1.gui + "(''set'')", %labels(%noooo), %ssss, %ini(%noooo));
            if length(arg1.model.uid) > 0 then
                warnBlockByUID(arg1.model.uid, mess); // arg1 is from the block interface function
            else
                disp(mess);
            end
            %ini=%str
            %ok=%f;break
        elseif %noooo<0 then
            str = gettext("%s: Wrong type for block parameter ''%s'': %s(%s) expected, getting %s");
            mess = msprintf(str, arg1.gui + "(''set'')", %labels(-%noooo), %typ(-2*%noooo-1), strcat(string(%typ(-2*%noooo))," by "), %ini(-%noooo));
            if length(arg1.model.uid) > 0 then
                warnBlockByUID(arg1.model.uid, mess); // arg1 is from the block interface function
            else
                disp(mess);
            end
            %ini=%str
            %ok=%f;break
        else
            break
        end
    end
    if %lhs==%nn+2 then
        execstr("%"+string(%lhs-1)+"=%str")
    end
endfunction
