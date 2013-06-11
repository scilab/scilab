function plot_potts2d(Var, Level)
    Size = size(Var,1);
    drawlater;
    f=gcf();
    f.color_map = graycolormap(2*Level+1);
    a = gca();
    a.data_bounds = [0,0;Size+1,Size+1];
    Z = zeros(Size*Size,3);
    Index = 0;
    for i=1:Size
        for j=1:Size
            Index = Index + 1;
            Z(Index,1) = i;
            Z(Index,2) = j;
            Z(Index,3) = Var(i,j);
        end
    end
    param3d1(Z(:,1),Z(:,2),list(Z(:,3),-9));
    xtitle("Potts 2D model");
    drawnow;
endfunction
