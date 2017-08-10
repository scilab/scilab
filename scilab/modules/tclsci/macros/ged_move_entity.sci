function ged_move_entity()
    [btn,xc,yc]=xclick()
    pos=[xc,yc]
    [xc,yc]=xchange(xc,yc,"f2i")
    [r,ax]=ged_getobject([xc,yc])
    cur_ax=gca(),sca(ax)
    [xc,yc]=xchange(xc,yc,"i2f");pos=[xc,yc]
    if r==[] return,end
    drawlater();
    rep(3)=-1
    select r.type
    case "Rectangle" then
        while rep(3)==-1 do
            rep=xgetmouse([%t %t])
            r.data(1:2)= r.data(1:2)+(rep(1:2)-pos)
            pos=rep(1:2)
            drawnow();
        end
    case "Segs" then //Segment
        while rep(3)==-1 do
            rep=xgetmouse([%t %t])
            r.data=r.data+ones(2,1)*(rep(1:2)-pos)
            pos=rep(1:2)
            drawnow();
        end
    case "Polyline" then //Polyline
        while rep(3)==-1 do
            rep=xgetmouse([%t %t])
            r.data(:,1:2)=r.data(:,1:2)+ones(r.data(:,1))*(rep(1:2)-pos)
            pos=rep(1:2)
            drawnow();
        end
    case "Arc" then //Circle
        while rep(3)==-1 do
            rep=xgetmouse([%t %t])
            r.data(1:2)= r.data(1:2)+(rep(1:2)-pos)
            pos=rep(1:2)
            drawnow();
        end
    case "Text" then
        while rep(3)==-1 do
            rep=xgetmouse([%t %t])
            r.data(1:2)= r.data(1:2)+(rep(1:2)-pos)
            pos=rep(1:2)
            drawnow();
        end
    case "Label" then
        while rep(3)==-1 do
            rep=xgetmouse([%t %t])
            r.position= r.position+(rep(1:2)-pos)
            r.auto_position = "off"
            pos=rep(1:2)
            drawnow();
        end

    end
    sca(cur_ax)
endfunction
