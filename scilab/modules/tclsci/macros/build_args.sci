function [ini,typs]=build_args(labels)
    n=size(labels,"*")
    ini=[]
    typs=list()
    for k=1:n
        typ=type(h(labels(k)))
        execstr(["if typ==10 then"
        "   w=h."+labels(k)
        "else"
        "   w=sci2exp(h."+labels(k)+",0)"
        "end"])
        ini=[ini;w]
        if typ==10 then
            typs($+1)="str";typs($+1)=-1
        else
            typs($+1)="vec";typs($+1)=1
        end
    end
endfunction
