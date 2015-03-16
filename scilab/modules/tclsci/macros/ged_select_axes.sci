function axes =  ged_select_axes(x,y)
    // x and y are pixel coord.
    f=gcf()
    nb_axes = size(f.children,"*") // for now Iconsider that children of a figure are of type Axes
    axes_size = f.axes_size // given in pixels
    axes_size = [axes_size axes_size];

    for i=1:nb_axes
        axes = f.children(i);
        cur_axes_bounds = axes.axes_bounds;
        rect = cur_axes_bounds.*axes_size; // rectangle in pixels (margins inside)

        rect(3) = rect(3) + rect(1);
        rect(4) = rect(4) + rect(2);
        if (x>rect(1) & x<rect(3) & y>rect(2) & y<rect(4)) then
            return
        end
    end
    axes=[]
endfunction
