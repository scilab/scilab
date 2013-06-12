//** Comments by Simone Mannori
//
// Copyright INRIA

function hilite_image = select_hilite2(Select)

    winback = xget("window") //** save the window

    if Select<>[] then //** if there is some element selected

        hilite_image=list() //** clear the data struct

        for i=1:size(Select,1)

            [junk, win, o] = get_selection(Select(i,:))

            if or(winsid()==win) then
                xset("window",win)
                alu=xget("alufunction")  //** save the state
                // xset('alufunction',6) //** eliminating the XOR mode
            else
                return
            end

            //**------------------------------------------------
            dr=driver()
            if dr=="Rec" then driver("X11"),end
            //**------------------------------------------------

            if typeof(o)=="Block" then
                //** ------------- Blocks ---------------------------------
                graphics=o.graphics;
                [orig,sz]=(graphics.orig,graphics.sz)
                thick=xget("thickness")
                thickness=4*thick
                xset("thickness",thickness);
                xrect(orig(1),orig(2)+sz(2),sz(1),sz(2));
                if pixmap then xset("wshow"),end
                xset("thickness",thick);
                hilite_image($+1)=list(win,"rect",orig,sz,thickness)


            elseif typeof(o)=="Link" then
                //**-------------- Links ---------------------------------
                if o.thick(2)>=0 then
                    thick=xget("thickness");d=xget("dashes")
                    thickness=maxi(o.thick(1),1)*maxi(o.thick(2),1)*4,
                    xset("thickness",thickness)
                    dashes=o.ct(1)
                    xset("dashes",dashes)
                    xx=o.xx,yy=o.yy,
                    xpoly(xx,yy,"lines")
                    xset("dashes",d)
                    xset("thickness",thick)
                    hilite_image($+1)=list(win,"poly",xx,yy,dashes,thickness)
                end

                if pixmap then xset("wshow"),end

            end //** end the for(..) loop

            driver(dr) //** to delete

            if rhs>1 then
                xset("alufunction",alu)
                xset("window",winback)
            end

        end

    else //** if the Select is empty

        hilite_image=list()

    end

    xset("window",winback)

endfunction
