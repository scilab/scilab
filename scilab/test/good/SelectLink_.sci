// Copyright INRIA

//** 12 Jan 2006: Code modified/commented by Simone Mannori


//**
function SelectLink_()

    Select=[]; SelectRegion=list() ; //** init internal variable

    //  At this point Select=[]

    //** The user has clicked in the Navigator window
    //** In this case, clear the variable and exit
    if windows( find(%win==windows(:,2)), 1 )==100000 then
        Cmenu=[] ; //** this code cold be
        %pt=[]   ;
        return
    end

    //** in other cases :

    kc = find(%win==windows(:,2)) ; //** find the clicked window in the internal SCICOS
    //** active windows list datastructure

    //** case : is not an active SCICOS window
    if kc==[] then
        message("This window is not an active scicos window")
        Cmenu=[];%pt=[];return

        //** case : kc index a "negative" windows id  --> the user has clicked in a palette window
    elseif windows(kc,1) < 0 then //click dans une palette
        kpal = -windows(kc,1) ; //** recover thw windows palette id
        palette = palettes(kpal) ; //** acquire all the palette
        k = getobj(palette,%pt)  ; //** isolate the clicked block

        if k<>[] then
            Select=[k,%win]; Cmenu=[]; %pt=[]; //** there is a block on this coordinate
            return                             //** Select the block then return back
        else
            Cmenu==[];%pt=[]; return           //** no block : clear variable and return back
        end

        //** case : the user has clicked in the current editor window ( both main or superblock )
    elseif %win==curwin then // click dans la fenetre courante

        k=getobj(scs_m,%pt) //** look for the object

        if k<>[] then
            Cmenu = check_edge(scs_m.objs(k),[],%pt)
            //**
            disp("*Link="); disp(Cmenu) ; disp("Link*");
            if Cmenu==[] then
                Select=[k,%win]; Cmenu==[]; %pt=[]; return
            end
        else
            Cmenu==[]; %ppt=%pt; %pt=[];return  //position %ppt for Paste
        end


        //** case : copy object from other window
    elseif slevel>1 then
        execstr("k=getobj(scs_m_"+string(windows(kc,1))+",%pt)")
        if k<>[] then
            Select=[k,%win];Cmenu=[];return
        else
            Cmenu==[];%pt=[];return
        end
    else

        message("This window is not an active scicos window")
        Cmenu=[];%pt=[];return

    end

endfunction


