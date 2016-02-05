function ged_paste_entity()

    // check the file
    [info,err] = fileinfo(TMPDIR + "/G_Clipboard" ) ;

    if err <> 0 then
        return ;
    end

    // create the saved object
    load(TMPDIR+"/G_Clipboard") ;

    //  a=gca();b=a.data_bounds;
    //  move(r,[-1 1]*a.data_bounds/20)
endfunction
