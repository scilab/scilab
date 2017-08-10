// Search the depth level for each handle
// Usefull for new hierarchical graphic tree.
function ged_levels = Get_levels(handles);

    ged_levels = 1; // for Figure, always 1

    f = handles(1);

    for i=2:size(handles,1)
        ged_levels(i) = Get_Depth(f,handles(i));
    end

    //disp("les levels sont:")xb

    //disp(ged_levels);

endfunction
