function GetSetValue(h)
    n=size(labels,"*")
    lhs="[ok,"+strcat("x"+string(1:n),",")+"]"
    execstr(lhs+"=getvalue(t,labels,typs,ini)")
    if ok then
        for k=1:n
            execstr("if x"+string(k)+"<>h."+labels(k)+" then h."+..
            labels(k)+"=x"+string(k)+",end")
        end
    end
endfunction
