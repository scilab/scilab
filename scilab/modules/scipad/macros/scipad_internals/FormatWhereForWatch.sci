function txt = FormatWhereForWatch(startitem)
// Converts input information (provided by where()) into a single string that
// mimics the output of whereami()
// This is used for the watch window of the debugger in Scipad.
// The callstackfuns and callstacklines lists are also set inside the scipad
// interpreter by this function for later use during step by step, run to
// cursor, or to open a stack level by double-clicking in the call stack area
// Author: François Vogel, 2004-2007 (freely inspired from whereami())

// Localisation
str0=LocalizeForScipad("Breakpoint");
str1=LocalizeForScipad("called at line");
str2=LocalizeForScipad("of exec file");
str3=LocalizeForScipad("of execstr instruction");
str4=LocalizeForScipad("called under pause");
str5=LocalizeForScipad("of macro");

[linn,mac] = where()
nn  = prod(size(linn))
lm  = maxi(maxi(length(mac(2:$))),length(str0))
txt = emptystr()

// Create a Tcl list containing the calling stack functions list
macstring = strcat(mac(startitem+1:nn-1)," ")
macstring = strsubst(macstring,"$","\$")
TCL_SetVar("callstackfuns",macstring,"scipad")

// Create a Tcl list containing the calling stack line numbers list
linnstring = strcat(string(linn(startitem+1:nn-1))," ")
linnstring = strsubst(linnstring,"$","\$")
TCL_SetVar("callstacklines",linnstring,"scipad")

// Properly format the call stack and line numbers in a single string
// that will be displayed in the call stack area of the watch window
for k = 2:nn-1    // The first item is omitted since it is always "FormatWhereForWatch"
                  // The last item is omitted since it is the execstr from the ScilabEval "foo_to_debug()"
    if mac(k)=='exec' then
        txt = txt + part(mac(k-1),1:lm) + " " + str1 + " " + string(linn(k)) + " " + str2 + "\n"
    elseif mac(k)=='execstr' then
        txt = txt + part(mac(k-1),1:lm) + " " + str1 + " " + string(linn(k)) + " " + str3 + "\n"
    elseif mac(k)=='pause' then
        txt = txt + part(mac(k-1),1:lm) + " " + str4 + "\n"
    else
        txt = txt + part(mac(k-1),1:lm) + " " + str1 + " " + string(linn(k)) + " " + str5 + " " + mac(k) + "\n"
    end
end

// Remove the first startitem-1 lines (not relevant since they list the stack content
// for the pause level(s) at the breakpoint)
pos = strindex(txt,"\n")
if pos ~= [] then
    if size(pos,'c') > 1 then
        txt = part(txt,pos(startitem-1)+2:length(txt))
        txt = str0 + part(txt,length(str0)+1:length(txt))
    else
        txt = emptystr();
    end
end

// Special case : the dollar sign must be escaped otherwise what follows is
// interpreted as a Tcl variable
// A dollar can be found in txt when debugging a function whose name starts
// with $
txt = strsubst(txt,"$","\$")

endfunction
