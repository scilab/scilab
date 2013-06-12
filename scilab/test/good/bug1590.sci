if MSDOS then
    if ( ~or(sciargs()==["-nw"]) & ~or(sciargs()==["-nwni"]) ) then
        a="put in clipboard";
        clipboard("copy",a);
        a1=clipboard("paste");
        if (a<>a1) then affich_result(%F,1590),return,end;

        b=123.4;
        clipboard("copy",b);
        b1=clipboard("paste");
        if (string(b)<>b1) then affich_result(%F,1590),return,end;

        c=["put","in","clipboard"];
        clipboard("copy",c);
        c1=clipboard("paste");
        if ("put in clipboard "<>c1) then affich_result(%F,1590),return,end;
    end

    scf(2);
    plot();
    clipboard(2,"EMF");
    if ( ~or(sciargs()==["-nw"]) & ~or(sciargs()==["-nwni"]) ) then
        clipboard("do","empty");
    end

    scf(3);
    plot3d();
    clipboard(3,"DIB");
    if ( ~or(sciargs()==["-nw"]) & ~or(sciargs()==["-nwni"]) ) then
        clipboard("do","empty");
    end


    if ( ~or(sciargs()==["-nw"]) & ~or(sciargs()==["-nwni"]) ) then
        d=rand(3,5);
        clipboard("copy",d);
        d1=clipboard("paste");
        da=strcat(string(d)'+" ");
        db=strsubst(d1',char(10),"");
        if (da<>db)  then affich_result(%F,1590),return,end;
    end
end

affich_result(%T,1590);
