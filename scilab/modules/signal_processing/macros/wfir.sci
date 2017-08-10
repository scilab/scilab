// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) INRIA - 1988 - C. Bunks
//
// Copyright (C) 2012 - 2016 - Scilab Enterprises
//
// This file is hereby licensed under the terms of the GNU GPL v2.0,
// pursuant to article 5.3.4 of the CeCILL v.2.1.
// This file was originally licensed under the terms of the CeCILL v2.1,
// and continues to be available under such terms.
// For more information, see the COPYING file which you should have received
// along with this program.

function [wft,wfm,fr]=wfir(ftype,forder,cfreq,wtype,fpar)
    //[wft,wfm,fr]=wfir(ftype,forder,cfreq,wtype,fpar)
    //Macro which makes linear-phase, FIR low-pass, band-pass,
    //high-pass, and stop-band filters
    //using the windowing technique.
    //Works interactively if called with no arguments.
    //  ftype  :Filter type ('lp','hp','bp','sb')
    //  forder :Filter order (pos integer)(odd for ftype='hp' or 'sb')
    //  cfreq  :2-vector of cutoff frequencies (0<cfreq(1),cfreq(2)<.5)
    //         :only cfreq(1) is used when ftype='lp' or 'hp'
    //  wtype  :Window type ('re','tr','hm','hn','kr','ch')
    //  fpar   :2-vector of window parameters
    //         :     Kaiser window: fpar(1)>0 fpar(2)=0
    //         :     Chebyshev window: fpar(1)>0 fpar(2)<0 or
    //         :                       fpar(1)<0 0<fpar(2)<.5
    //  wft    :Time domain filter coefficients
    //  wfm    :Frequency domain filter response on the grid fr
    //  fr     :Frequency grid
    //!
    wft=[];wfm=[];fr=[]

    FT=["lp","hp","bp","sb"]

    [lhs,rhs]=argn(0);

    if rhs<=0 then,
        //if macro called with no arguments query user for values
        [ok,values,exprs]=wfir_gui()
        if ~ok then return,end
        ftype=values.ftype
        forder=values.forder
        cfreq=[values.low,values.high]/values.freq_ech;
        wtype=values.wtype
        fpar=values.fpar

        fl=values.low/values.freq_ech;
        fh=values.high/values.freq_ech;
    else
        //check arguments of macro call
        if and(ftype<>FT) then
            error(msprintf(_("%s: Wrong value for input argument #%d: Must be in the set {%s}.\n"),"wfir",1,strcat(FT,",")))
        end
        if type(forder)<>1|size(forder,"*")<>1|~isreal(forder)|size(forder,"*")<>1|int(forder)<>forder|forder<1 then
            error(msprintf(_("%s: Wrong type for input argument #%d: A positive integer expected.\n"),"wfir",2))
        end
        if or(ftype==["hp" "sb"]) then
            if 2*int(forder/2)==forder then
                error(msprintf(_("%s:  Wrong value for input argument #%d: Must be odd.\n"),"wfir",2))
            end
        end

        if type(cfreq)<>1|~isreal(cfreq) then
            error(msprintf(_("%s: Wrong type for input argument #%d: A %d-by-%d real vector expected.\n"),"wfir",3,1,2))
        end
        if or(ftype==["hp" "lp"]) then
            if size(cfreq,"*")==0| size(cfreq,"*")>2 then
                error(msprintf(_("%s: Wrong size for input argument #%d: A %d-by-%d real vector expected.\n"),"wfir",3,1,2))
            end
            if cfreq(1)<0|cfreq(1)>0.5 then
                error(msprintf(_("%s: Wrong value for input argument #%d: Must be in the interval %s.\n"),"wfir",3,"]0,0.5["))
            end
            fl=cfreq(1);fh=[]
        else
            if size(cfreq,"*")<>2 then
                error(msprintf(_("%s: Wrong size for input argument #%d: A %d-by-%d real vector expected.\n"),"wfir",3,1,2))
            end
            if or(cfreq<0|cfreq>0.5) then
                error(msprintf(_("%s: Wrong value for input argument #%d: Must be in the interval %s.\n"),"wfir",3,"]1,0.5["))
            end
            if cfreq(1)>=cfreq(2) then
                error(msprintf(_("%s: Elements of %dth argument must be in increasing order.\n"),"wfir",3))
            end
            fl=cfreq(1);
            fh=cfreq(2);
        end
    end


    //Calculate window coefficients

    [win_l,cwp]=window(wtype,forder,fpar);
    [dummy,forder]=size(win_l);

    //Get forder samples of the appropriate filter type

    hfilt=ffilt(ftype,forder,fl,fh);

    //Multiply window with sinc function

    wft=win_l.*hfilt;

    //Calculate frequency response of the windowed filter

    [wfm,fr]=frmag(wft,256);
endfunction

