function importgui_cbselect()

    path = pwd();
    f = get("importgui_filename", "string");
    if ~isempty(f) then
        path = fileparts(f);
    end
    
    
	path = uigetfile(["*.txt" "Text files";"*.csv" "CSV files"], path, "Choose a file", %f);
    if ~isempty(path) then
        set("importgui_filename", "string", path);
        delim = importgui_analyze(path, 10);
        if delim then
            set("importgui_delim", "value", delim);
            if delim == 1 then
                set("importgui_decimal", "value", 1);
            end
        end

        importgui_preview();
    end
endfunction
