function importgui_progress(val)
    pos = get("importgui_progressbar", "position");
    pos(3) = 800 * val;
    set("importgui_progressbar", "position", pos);
endfunction
