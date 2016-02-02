// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2010-2011 - INRIA - Serge Steer <serge.steer@inria.fr>
//
// Copyright (C) 2012 - 2016 - Scilab Enterprises
//
// This file is hereby licensed under the terms of the GNU GPL v2.0,
// pursuant to article 5.3.4 of the CeCILL v.2.1.
// This file was originally licensed under the terms of the CeCILL v2.1,
// and continues to be available under such terms.
// For more information, see the COPYING file which you should have received
// along with this program.

function setStringPosition(tip_handle,pt)
    //Computes the position of the lower left corner of the string box.
    //
    //These coordinates must be expressed in user coordinates units but their
    //computation takes into account the pixel dimensions of the text boxes
    //when they are not located in the upper right position
    ax=tip_handle.parent
    while ax.type<>"Axes" then ax=ax.parent,end
    string_handle=tip_handle.children(2);
    point_handle=tip_handle.children(1);


    params=string_handle.user_data
    orient=params(1);
    if size(params,"*")>1 then //for compatibility
        style=params(2)
    else
        style=1
    end
    pt=pt(1,:)

    r=xstringl(0,0,string_handle.text);r=r(3:4)';
    if style==1 then
        d=mark_usersize(point_handle);
    else
        d=[0 0]
    end
    orient=string_handle.user_data(1);
    if orient==0 then //automatic

        //compute the slope and curvature at the tip point
        curve_handle=point_handle.user_data(1)
        k=point_handle.user_data(2)
        np=size(curve_handle.data,1)
        T=curve_handle.data([max(k-1,1) k min(np,k+1)],:)


        if size(T,2)==3 then
            [Tx,Ty]=geom3d(T(:,1),T(:,2),T(:,3))
            T=[Tx Ty];
        end
        d1=sum(diff(T,1,1),1)/2;
        d2=diff(T,2,1);
        c=(d1(1)*d2(2)-d1(2)*d2(1))

        //set position according to the slope and curvature sign
        if d1(1)>0 then
            if  d1(2)>0 then
                if c<0 then//upper left
                    orient=1
                else //lower right
                    orient=4
                end
            else
                if c<0 then//upper right
                    orient=2
                else //lower left
                    orient=3
                end
            end
        else
            if  d1(2)>0 then
                if c>0 then //upper right
                    orient=2
                else //lower left
                    orient=3
                end
            else
                if c>0 then//upper left
                    orient=1
                else //lower right
                    orient=4
                end

            end
        end
        //    mprintf("d1=[%f %f], c=%f, orient=%d\n",d1(1),d1(2),c,orient)
        if ax.zoom_box==[]
            box=ax.data_bounds
        else
            box=[ax.zoom_box(1:2);ax.zoom_box(3:4)];
        end
        //take care of  the plot boundaries
        //    mprintf("Y:%f< [%f %f]<%f\n",box(1,2),pt(2)-r(2),pt(2)+r(2),box(2,2))
        //    mprintf("X:%f< [%f %f]<%f\n",box(1,1),pt(1)-r(1),pt(1)+r(1),box(2,1))
        if or(orient==[1 2])&pt(2)+r(2)>box(2,2) then//upper bound on Y
            orient=5-orient
        end
        if or(orient==[3 4])&pt(2)-r(2)<box(1,2) then//lower bound on Y
            orient=5-orient
        end
        if or(orient==[2 4])&pt(1)+r(1)>box(2,1) then//right bound on X
            orient=orient-1
        end
        if or(orient==[1 3])&pt(1)-r(1)<box(1,1) then//left bound on X
            orient=orient+1
        end
    end
    select orient
    case 1 then //upper left
        dx=-d(1)-r(1);dy=d(2);
    case 2 then //upper right
        dx=d(1);dy=d(2)
    case 3 then //lower left
        dx=-d(1)-r(1);dy=-d(2)-r(2)
    case 4 then //lower right
        dx=d(1);dy=-d(2)-r(2)
    end

    if ax.view=="3d" then
        angles=ax.rotation_angles*%pi/180;
        t=angles(2);a=angles(1);
        st=sin(t);ct=cos(t);sa=sin(a);ca=cos(a)
        c=sum(ax.data_bounds,1)/2
        x=pt(1)-c(1)
        y=pt(2)-c(2)
        z=pt(3)-c(3)
        pos=c+[(st*x-ct*y-dx)/st,0,(st*dy+st*sa*z-ca*y-ct*ca*dx)/(sa*st)]
    else
        pos=pt+[dx dy]

    end
    string_handle.data=pos
endfunction
function usize=mark_usersize(m)
    pix_size=10*(m.mark_size+1)
    [x1,y1]=xchange([0 pix_size],[0 pix_size],"i2f");
    usize=abs([x1(2)-x1(1) y1(2)-y1(1)])/20;
endfunction
