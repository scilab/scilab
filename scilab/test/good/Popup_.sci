function Popup_()
    //** INRIA

    // gh_curwin = gh_current_window ;
    //gh_curwin.pixmap ='on'; //set the pixmap mode

    //** at this point I need to build the [scs_m] <-> [gh_window] datastructure
    //** I need an equivalent index for the graphics

    // o_size = size (gh_curwin.children.children ) ; //** the size:number of all the object

    //** "k" is the object index in the data structure "scs_m"
    //** compute the equivalent "gh_k" for the graphics datastructure
    // gh_k = o_size(1) - k + 1 ; //** semi empirical equation :)
    // gh_blk = gh_curwin.children.children(gh_k);

    kc = find( %win==windows(:,2) );

    //**------------------------------------------------------------------------------
    if kc==[] then

        message("This window is not an active scicos window")
        Cmenu = []; %pt=[]; %ppt=[]; Select=[]; return ; //** ---> Exit point

    elseif windows(kc,1)<0 then //click inside a palette window

        gh_curwin = scf(%win) ;
        o_size = size(gh_curwin.children.children)

        kpal = -windows(kc,1)    ;
        palette = palettes(kpal) ;
        k = getobj(palette,%pt)  ;

        if k<>[] then
            gh_k = o_size(1) - k + 1 ;
            gh_blk = gh_curwin.children.children(gh_k);
            twinkle(gh_blk,2);

            // hilite_obj(palette.objs(k),%win)
            // xpause(200000)
            // unhilite_obj(palette.objs(k),%win)

            j=3 ; Select = [k,%win];
        else

            Cmenu==[];%pt=[];%ppt=[];Select=[];return
        end

    elseif %win==curwin then //click inside the current window

        gh_curwin = scf(%win) ;
        o_size = size(gh_curwin.children.children)

        k = getobj(scs_m,%pt)

        if k<>[] then
            j = 1

            gh_k = o_size(1) - k + 1 ;
            gh_blk = gh_curwin.children.children(gh_k);
            twinkle(gh_blk,2);

            // hilite_obj(scs_m.objs(k),%win);
            // xpause(200000)
            // hilite_obj(scs_m.objs(k),%win);
            Select = [k,%win]
        else
            j = 2; %ppt = %pt    // for pasting
        end

    elseif slevel>1 then
        execstr("k=getobj(scs_m_"+string(windows(kc,1))+",%pt)")

        gh_curwin = scf(%win) ;
        o_size = size(gh_curwin.children.children)

        if k<>[] then

            gh_k = o_size(1) - k + 1 ;
            gh_blk = gh_curwin.children.children(gh_k);
            twinkle(gh_blk,2);

            // execstr('hilite_obj(scs_m_'+string(windows(kc,1))+'.objs(k),%" + "win)')
            // xpause(200000)
            // execstr('hilite_obj(scs_m_'+string(windows(kc,1))+'.objs(k),%" + "win)')
            Select=[k,%win];j=3
        else
            Cmenu==[];%pt=[];%ppt=[];Select=[];return
        end

    else

        message("This window is not an active scicos window")
        Cmenu=[];%pt=[];%ppt=[];Select=[];return

    end //** end of the main if() switch case structure

    Cmenu = mpopup(%scicos_lhb_list(j)) ;

    if Cmenu==[] then %pt=[];%ppt=[];Select=[];end

endfunction
