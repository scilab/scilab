// CORNET Allan
// INRIA 2006

function scilab_demos()

    mode(-1)

    oldln=lines();
    lines(0)
    global demolist
    // Demos list is defined in scilab.start
    deff("[]=demoex(num)","exec(demolist(num,2),-1)")
    while %t then
        num=tk_choose(demolist(:,1),["Click to choose a demo"]);
        if num==0 then
            lines(oldln(1))
            return
        else
            demoex(num)
        end,
    end

endfunction