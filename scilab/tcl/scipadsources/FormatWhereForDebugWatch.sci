function txt = FormatWhereForDebugWatch(startitem)
// Converts input information (provided by where()) into a single string that
// mimics the output of whereami()
// This is used for the watch window of the debugger in Scipad.
// Author: François Vogel, 2004 (freely inspired from whereami())

global LANGUAGE
select LANGUAGE
case "eng"
    str0 = "Breakpoint"
    str1 = " called at line ";
    str2 = " of exec file";
    str3 = " of execstr instruction";
    str4 = " called under pause";
    str5 = " of macro ";
case "fr"
    str0 = "Point d''arrêt"
    str1 = " appelé à la ligne ";
    str2 = " dans un fichier exec";
    str3 = " dans une instruction execstr";
    str4 = " appelé sous pause";
    str5 = " de la macro ";
end

[linn,mac] = where()
nn  = prod(size(linn))
lm  = maxi(maxi(length(mac(2:$))),length(str0))
txt = emptystr()

for k = 2:nn-1    // The last one is omitted since it is the execstr from the ScilabEval "foo_to_debug()"
    if mac(k)=='exec' then
        txt = txt + part(mac(k-1),1:lm) + str1 + string(linn(k)) + str2 + "\n"
    elseif mac(k)=='execstr' then
        txt = txt + part(mac(k-1),1:lm) + str1 + string(linn(k)) + str3 + "\n"
    elseif mac(k)=='pause' then
        txt = txt + part(mac(k-1),1:lm) + str4 + "\n"
    else
        txt = txt + part(mac(k-1),1:lm) + str1 + string(linn(k)) + str5 + mac(k) + "\n"
    end
end

// Remove the first startitem-1 lines (not relevant since they list the stack content
// for the current function (FormatWhereForDebugWatch), and for the pause level(s) at
// the breakpoint)
pos = strindex(txt,"\n")
if pos ~= [] then
    if size(pos,'c') > 1 then
        txt = part(txt,pos(startitem-1)+2:length(txt))
        txt = str0 + part(txt,length(str0)+1:length(txt))
    else
        txt = emptystr();
    end
end

endfunction
