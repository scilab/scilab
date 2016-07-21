function [blocks] = anim_pen(blocks,flag)
    //** 7 Jan 2009 : updated for Scilab 5.x
    win = 20000+curblock();
    if flag<>4 then
        H=scf(win)
    end
    xold = blocks.z
    rpar = blocks.rpar
    plen = rpar(1); csiz = rpar(2); phi = rpar(3);

    //** INIT
    if flag==4 then
        H = scf(win)
        clf
        isoview(H, "on")
        Axe = H.children
        Axe.data_bounds=rpar(4:7)
        S=[cos(phi),-sin(phi);sin(phi),cos(phi)]
        XY=S*[rpar(4),rpar(5);-csiz/2,-csiz/2]

        drawlater();
        xsegs(XY(1,:),XY(2,:))
        x=0; theta=0;
        x1=x-csiz/2;x2=x+csiz/2;y1=-csiz/2;y2=csiz/2
        XY=S*[x1 x2 x2 x1 x1;y1,y1,y2,y2,y1]
        xpoly(XY(1,:),XY(2,:),"lines",1)
        XY=S*[x,x+plen*sin(theta);0,0+plen*cos(theta)]
        xsegs(XY(1,:),XY(2,:));
        drawnow();

        //** UPDATE
    elseif flag==2 then
        Axe = H.children
        x = blocks.inptr(1)(1)
        theta = blocks.inptr(2)(1)
        drawlater();
        XY=Axe.children(2).data' + [cos(phi)*(x-xold);sin(phi)*(x-xold)]*ones(1,5);
        Axe.children(2).data=XY' ;
        x1=x*cos(phi);y1=x*sin(phi) ;
        XY=[x1,x1+plen*sin(theta);y1,y1+plen*cos(theta)] ;
        Axe.children(1).data=XY' ;
        blocks.z = x ;
        drawnow();
    end
endfunction
