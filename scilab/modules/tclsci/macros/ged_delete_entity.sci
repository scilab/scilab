function ged_delete_entity()
    [btn,xc,yc]=xclick()
    [xc,yc]=xchange(xc,yc,"f2i")
    h=ged_getobject([xc,yc])
    if h<>[] then delete(h),end
endfunction
