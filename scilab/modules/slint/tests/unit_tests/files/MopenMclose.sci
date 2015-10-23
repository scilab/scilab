function MopenMclose(a)
    fd = mopen(a)
endfunction

function MopenMclose1(a)
    fd = mopen(a)
    mclose("all")
endfunction