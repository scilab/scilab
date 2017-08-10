function TK_send_handles_list(h)
    iFig = 0;
    iUim = 0; // uimenu
    iUic = 0; // uicontrol
    iAxe = 0; // <=> subwindow in C code
    iAgr = 0;
    iPol = 0;
    iPl3 = 0;
    iFac = 0;
    iRec = 0;
    iTex = 0;
    iLeg = 0;
    iArc = 0;
    iSeg = 0;
    iCha = 0; // Champ
    iFec = 0;
    iGra = 0;
    iMat = 0; // forgotten object F.Leray 22.10.04
    iAxi = 0; // axis : entity created when using drawaxis method for example
    iLab = 0;

    f=getparfig(h);
    handles = Get_handles_list(f)

    ged_levels = Get_levels(handles);

    TCL_SetVar("ged_handle_list_size",string(size(handles,1)));

    for i=1:size(handles,1)
        SelObject="LEVELS("+string(i)+")";
        TCL_EvalStr("set "+SelObject+" "+string(ged_levels(i)));
    end


    for i=1:size(handles,1)
        SelObject="SELOBJECT("+string(i)+")";
        hand = handles(i);
        select  hand.type
        case "Figure"
            iFig = iFig+1;
            figname= "Figure("+string(iFig)+")";
            TCL_EvalStr("set "+SelObject+" "+figname);
        case "uimenu"
            iUim = iUim+1;
            uimname= "Uimenu("+string(iUim)+")";
            TCL_EvalStr("set "+SelObject+" "+uimname);
        case "uicontrol"
            iUic = iUic+1;
            uicname= "Uicontrol("+string(iUic)+")";
            TCL_EvalStr("set "+SelObject+" "+uicname);
        case "Axes"
            iAxe = iAxe+1;
            axename= "Axes("+string(iAxe)+")";
            TCL_EvalStr("set "+SelObject+" "+axename);
            //   case "Label"  // to see later: have labels at the same level than Axes (to have a better visibility)
            //    iLab = iLab+1;
            //    labname= "Label("+string(iLab)+")";
            //    TCL_EvalStr('set '+SelObject+" "+labname);
        case "Compound"
            iAgr = iAgr+1;
            agrname= "Compound("+string(iAgr)+")";
            TCL_EvalStr("set "+SelObject+" "+agrname);
        case "Polyline"
            iPol = iPol+1;
            polname= "Polyline("+string(iPol)+")";
            TCL_EvalStr("set "+SelObject+" "+polname);
        case "Plot3d"
            iPl3 = iPl3+1;
            pl3name= "Plot3d("+string(iPl3)+")";
            TCL_EvalStr("set "+SelObject+" "+pl3name);
        case "Fac3d"
            iFac = iFac+1;
            Facname= "Fac3d("+string(iFac)+")";
            TCL_EvalStr("set "+SelObject+" "+Facname);
        case "Rectangle"
            iRec = iRec+1;
            Recname= "Rectangle("+string(iRec)+")";
            TCL_EvalStr("set "+SelObject+" "+Recname);
        case "Text"
            iTex = iTex+1;
            Texname= "Text("+string(iTex)+")";
            TCL_EvalStr("set "+SelObject+" "+Texname);
        case "Legend"
            iLeg = iLeg+1;
            Legname= "Legend("+string(iLeg)+")";
            TCL_EvalStr("set "+SelObject+" "+Legname);
        case "Arc"
            iArc = iArc+1;
            Arcname= "Arc("+string(iArc)+")";
            TCL_EvalStr("set "+SelObject+" "+Arcname);
        case "Segs"
            iSeg = iSeg+1;
            Segname= "Segs("+string(iSeg)+")";
            TCL_EvalStr("set "+SelObject+" "+Segname);
        case "Champ"
            iCha = iCha+1;
            Chaname= "Champ("+string(iCha)+")";
            TCL_EvalStr("set "+SelObject+" "+Chaname);
        case "Fec"
            iFec = iFec+1;
            Fecname= "Fec("+string(iFec)+")";
            TCL_EvalStr("set "+SelObject+" "+Fecname);
        case "Grayplot"
            iGra = iGra+1;
            Graname= "Grayplot("+string(iGra)+")";
            TCL_EvalStr("set "+SelObject+" "+Graname);
        case "Matplot"
            iMat = iMat+1;
            Matname= "Matplot("+string(iMat)+")";
            TCL_EvalStr("set "+SelObject+" "+Matname);
        case "Axis"
            iAxi = iAxi+1;
            Axiname= "Axis("+string(iAxi)+")";
            TCL_EvalStr("set "+SelObject+" "+Axiname);
        else
            error( hand.type + " not handled");
        end
    end
endfunction
