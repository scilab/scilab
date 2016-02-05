function ged_copy_entity()
    [btn,xc,yc]=xclick()
    [xc,yc]=xchange(xc,yc,"f2i")
    r=ged_getobject([xc,yc])
    if r==[] return,end
    twinkle(r,1);
    save(TMPDIR+"/G_Clipboard",r)
    //make the axes containning the clicked point the current one
    sca(ged_select_axes(xc,yc))
endfunction
