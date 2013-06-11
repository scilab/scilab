//** 24 May 2006:

//** Comments by Simone Mannori

function OpenSet_()

    %xc=%pt(1); %yc=%pt(2); //** last mouse position

    disablemenus() ; //** disable the "interrupts" from dynamic manu :)


    if windows(find(%win==windows(:,2)),1)==100000 then
        //** ------------------ Navigator -------------------------------
        //click in navigator
        [%Path,%kk,ok]=whereintree(%Tree,%xc,%yc)
        if ok&%kk<>[] then %Path($)=null();%Path($)=null();end
        if ~ok then %kk=[],end
    else
        //** ------------- Is not a navigator -> look for the object --------------------
        %kk = getobj(scs_m,[%xc;%yc])
        %Path = list("objs",%kk)
    end

    if %kk<>[] then //** if the double click is not in the void ------------------------
        super_path = [super_path,%kk]
        [o,modified,newparametersb,needcompileb,editedb] = clickin(scs_m(%Path))

        //** this POC potentially dangerous !!!
        if Cmenu=="Link" then
            %pt=[%xc,%yc];
            super_path($)=[]
            enablemenus()
            return ; //** ---> exit point
        end

        // in case previous window has been destroyed
        if ~or(curwin==winsid()) then //** no windows inside the list
            xset("window",curwin);
            xset("default")
            xbasc();
            if pixmap then xset("pixmap",1); end
            xset("pattern",1)
            xset("dashes",1)
            if ~set_cmap(scs_m.props.options("Cmap")) then // add colors if required
                scs_m.props.options("3D")(1)=%f //disable 3D block shape
            end

            if pixmap then xset("wwpc");end
            xbasc();xselect()

            if pixmap then
                driver("X11")
            else
                driver("Rec");
            end

            set_background()   ;

            pwindow_set_size()

            window_set_size()

            //** xset('alufunction',6)

            drawobjs(scs_m);  //** redraw all the objects !

            //** if pixmap then xset('wshow'),end

            menu_stuff()


            if ~super_block then
                delmenu(curwin,"stop")
                addmenu(curwin,"stop",list(1,"haltscicos"))
                unsetmenu(curwin,"stop")
            else
                unsetmenu(curwin,"Simulate")
            end
            //
        end  // end of redrawing deleted parent
        //--------------------------------------------------------------------------

        if needcompileb==4 then
            %kw = find(windows(:,1)==100000)
            if %kw<>[] then
                xdel(windows(%kw,2))
                %Tree=list()
            end
        end

        edited=edited|editedb

        super_path($-size(%kk,2)+1:$)=[]

        if editedb then
            scs_m_save=scs_m;nc_save=needcompile
            if ~pal_mode then
                needcompile=max(needcompile,needcompileb)
            end
            scs_m=update_redraw_obj(scs_m,%Path,o)

        end

        //note if block parameters have been modified
        if modified&~pal_mode  then
            newparameters = mark_newpars(%kk,newparametersb,newparameters)
        end
    end

    enablemenus()

    Cmenu=[];%pt=[];

endfunction
