function curgedindex_ = Get_handle_pos_in_list(h)
    global ged_cur_fig_handle

    curgedindex_ = [];

    handles = Get_handles_list(ged_cur_fig_handle)
    for i=1:size(handles,1)
        if (h==handles(i))
            curgedindex_ = i;
        end
    end

    // Other case :
    // a label has been selected (and for now they are included inside the Axes)
    if (curgedindex_==[])
        if h.type == "Label"
            h = h.parent;
            for i=1:size(handles,1)
                if (h==handles(i))
                    curgedindex_ = i;
                end
            end
        end
    end

endfunction
