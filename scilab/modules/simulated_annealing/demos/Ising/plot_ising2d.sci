function plot_ising2d(Var)
    drawlater;
    a = gca();
    Size = size(Var,1);
    a.data_bounds = [0.5,0.5;Size+0.5,Size+0.5];
    for i=1:Size
        for j=1:Size
            if (Var(i,j)==1) then
                plot(i,j,"go");
            else
                plot(i,j,"r+");
            end
        end
    end
    xtitle("Ising 2D model");
    drawnow;
endfunction
