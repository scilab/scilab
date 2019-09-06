// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2019 - Stéphane Mottelet
//
// This file is hereby licensed under the terms of the GNU GPL v2.0,
// For more information, see the COPYING file which you should have received
// along with this program.
//
// A brickout game without bricks
// Idea from Philipp Muehlmann
// Code by Stéphane Mottelet
//

function pong()

    jimport java.awt.MouseInfo;

    clf
    f = gcf();
    f.resize = "off";
    f.toolbar_visible=%f;

    plot(0,0,1,1)
    isoview
    //paddle
    wpx = 0.2;
    wph = 0.05;
    xrect(0, wph, wpx, wph)
    hp = gce()

    //ball
    xrect(0.5, 0.5, wph, wph)
    hb = gce()
    VMAX = 1e-3;
    vb = [VMAX VMAX];

    k=2;
    score=0;

    messagebox(msprintf("Click OK to release ball %d",3-k),"","","OK","modal")

    while %t
        M = MouseInfo.getPointerInfo();
        X=double(M.getLocation().x)-gcf().figure_position(1);

        MOUSE_X = xchange(X,0,"i2f")
        if is_handle_valid(f)
            paddle_x = hp.data(1) + 1e-1*(MOUSE_X-wpx/2-hp.data(1));
            paddle_x = min(max(0,paddle_x),1-wpx);
            ball = hb.data(1:2) + vb;
        else
            break;
        end
        if ball(1) > 1-wph || ball(1) < 0
            vb(1) = -vb(1)
        end
        if ball(2) > 1
            vb(2) = -vb(2)
            score = score+1;
            VMAX = VMAX*1.1;
            xtitle(msprintf("Score = %d, balls left = %d", score, k))
        end
        if ball(2) < 2*wph
            if ball(2) < 0
                k = k-1;
                if k < 0
                    break
                end
                hb.data(1:2) = [.5,.5];
                VMAX=1e-3;
                vb=[VMAX VMAX];
                messagebox(msprintf("Click OK to release ball %d",3-k),"","","OK","modal")
            elseif ball(1) + wph >  paddle_x && ball(1) < paddle_x+wpx
                d = (ball(1)+wph/2) - (paddle_x+wpx/2);
                vb= VMAX*[max(-1,min(1,d/(wpx/2-wph/2))),1];
            end
        end
        if is_handle_valid(f)
            hp.data(1) = paddle_x;
            hb.data(1:2) = hb.data(1:2) + vb;
        end
        sleep(1)
    end
    messagebox("GAME OVER !","","","OK","modal")
end

pong();
clear pong;

