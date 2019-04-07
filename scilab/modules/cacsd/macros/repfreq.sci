// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 1984 - 2011 - INRIA - Serge STEER
//
// Copyright (C) 2012 - 2016 - Scilab Enterprises
//
// This file is hereby licensed under the terms of the GNU GPL v2.0,
// pursuant to article 5.3.4 of the CeCILL v.2.1.
// This file was originally licensed under the terms of the CeCILL v2.1,
// and continues to be available under such terms.
// For more information, see the COPYING file which you should have received
// along with this program.

function [frq,rep,splitf]=repfreq(sys,fmin,fmax,pas)

    pas_def="auto";
    l10=log(10);
    [lhs,rhs]=argn(0)
    //discretization
    if and(typeof(sys)<>[ "rational" "state-space" "zpk"]) then
        args=["sys","fmin","fmax","pas"];
        ierr=execstr("[frq,rep,splitf]=%"+typeof(sys,"overload")+"_repfreq("+strcat(args(1:rhs),",")+")","errcatch")
        if ierr<>0 then
            msg = gettext("%s: Wrong type for input argument #%d: Linear dynamical system expected.\n")
            error(msprintf(msg, "repfreq", 1))
        end
        return
    end
    if typeof(sys)=="zpk" then sys=zpk2tf(sys);end
    dom=sys.dt
    if dom==[] | dom==0 then
        msg = gettext("%s: Argument #%d: Undefined time domain.\n")
        error(msprintf(msg, "repfreq", 1));
    end
    if dom=="d" then dom=1;end

    select  rhs
    case 1 then
        pas=pas_def
        if dom=="c" then fmax=1.d3; else fmax=1/(2*dom),end
        fmin=0
    case 2 then
        if type(fmin)==10 then
            rhs=1
            pas=pas_def
            if dom=="c" then fmax=1.d3; else fmax=1/(2*dom),end
            if fmin=="sym" then
                fmin=-fmax
            else
                fmin=0
            end
        else
            frq=fmin
        end
    case 3 then
        pas=pas_def
    case 4 then ,
    else
        msg = gettext("%s: Wrong number of input arguments: %d to %d expected.\n")
        error(msprintf(msg, "repfreq", 1, 4))
    end;
    splitf=1
    if rhs<>2 then
        if fmin==[] then
            fmin=0,
        end
        if fmax==[]|fmax==%inf then
            if dom=="c" then
                fmax=1.d3;
            else
                fmax=1/(2*dom);
            end
        end

        if type(pas)==1 then
            splitf=1
            eps=1.e-14
            if fmin<0&fmax>=0 then
                frq=- [exp(l10*((log(eps)/l10):pas:(log(-fmin)/l10))) -fmin];
                if fmax>eps then
                    frq1=[exp(l10*((log(eps)/l10):pas:(log(fmax)/l10))) fmax];
                    frq=[frq($:-1:1) frq1]
                else
                    frq=frq($:-1:1);
                end
            elseif fmin<0&fmax<0 then
                frq= [exp(l10*((log(-fmax)/l10):pas:(log(-fmin)/l10))) -fmin];
                frq=-frq($:-1:1);
            elseif fmin >= fmax then
                msg = gettext("%s: Wrong value for input arguments #%d and #%d: %s < %s expected.\n")
                error(msprintf(msg, "repfreq", 2, 3, "fmin", "fmax"));
            else
                fmin=max(eps,fmin);
                frq=[exp(l10*((log(fmin)/l10):pas:(log(fmax)/l10))) fmax];
            end
        else
            [frq,bnds,splitf]=calfrq(sys,fmin,fmax)
        end;
    end
    //
    typ=sys(1)
    select typ(1)
    case "r" then
        [n,d]=sys(["num","den"]),
        [mn,nn]=size(n)
        if nn<>1 then
            msg = gettext("%s: Argument #%d: %s expected.\n")
            error(msprintf(msg, "repfreq", 1, _("SISO system")))
        end
        if dom=="c" then
            rep=freq(n,d,2*%pi*%i*frq),
        else
            rep=freq(n,d,exp(2*%pi*%i*dom*frq)),
        end;
    case "lss" then
        [a,b,c,d]=abcd(sys)
        [mn,nn]=size(d)
        if nn<>1 then
            msg = gettext("%s: Argument #%d: %s expected.\n")
            error(msprintf(msg, "repfreq", 1, _("SISO system")))
        end

        if dom=="c" then
            if a==[] then
                rep=(d*2*%pi*%i)*frq
            else
                rep=freq(a,b,c,d,2*%pi*%i*frq)
            end
        else
            if a==[] then
                rep=d*exp(2*%pi*%i*dom*frq)
            else
                rep=freq(a,b,c,d,exp(2*%pi*%i*dom*frq))
            end
        end;
    else
        msg = gettext("%s: Argument #%d: A system in state space or transfer matrix form expected.\n")
        error(msprintf(msg, "repfreq", 1))
    end;
    //representation
    if lhs==1 then frq=rep,end
endfunction
