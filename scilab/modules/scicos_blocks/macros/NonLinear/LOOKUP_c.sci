//  Scicos
//
//  Copyright (C) INRIA - METALAU Project <scicos@inria.fr>
//
// This program is free software; you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation; either version 2 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program; if not, write to the Free Software
// Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301, USA.
//
// See the file ../license.txt
//

function [x,y,typ]=LOOKUP_c(job,arg1,arg2)
    // Masoud Najafi 01/2008 --------
    // origine: serge Steer, Habib Jreij INRIA 1993
    // Copyright INRIA

    x=[];
    y=[];
    typ=[];
    select job
    case "set" then

        x=arg1
        model=arg1.model
        graphics=arg1.graphics
        exprs=graphics.exprs
        ok=%f;
        SaveExit=%f
        while %t do
            Ask_again=%f
            [ok,Method,xx,yy,extrapo,graf,exprs]=scicos_getvalue("Lookup table parameters",..
            ["Spline Interpolation method (0..9)";..
            "x";"y";"Extrapolate method (0,1)";"Launch graphic window(y/n)?"],..
            list("vec",1,"vec",-1,"vec",-1,"vec",1,"str",1),exprs)
            // 9 : nearest
            // 8 : above
            // 0:  below
            // extra: 0:hold; 1: use end values
            //

            if ~ok then
                break;
            end
            PeriodicOption="n";

            if PeriodicOption=="y" | PeriodicOption=="Y" then,
                PO=1;
            else,
                PO=0;
            end
            mtd=int(Method);
            if mtd<0 then
                mtd=0;
            end;
            if mtd>9 then
                mtd=9;
            end;
            METHOD=getmethod(mtd);
            extrapo=int(extrapo);
            if extrapo<0 then
                extrapo=0;
            end;
            if extrapo>1 then
                extrapo=1;
            end;


            if ~Ask_again then
                xx=xx(:);
                yy=yy(:);
                [nx,mx]=size(xx);
                [ny,my]=size(yy);
                if ~((nx==ny)&(mx==my)) then,
                    messagebox("Incompatible size of x and y");
                    Ask_again=%t;
                end
            end

            if ~Ask_again then//+++++++++++++++++++++++++++++++++++++++
                xy=[xx,yy];
                [xy]=cleandata(xy);// just for sorting to be able to compare data before and after poke_point(.)
                N= size(xy,"r");
                exprs(5)="n";// exprs.graf='n'
                if graf=="y" | graf=="Y" then //_______Graphic editor___________
                    ipar=[N;mtd;PO;extrapo];
                    rpar=[];
                    if ~exists("curwin") then
                        gh=gcf();
                        curwin=gh.figure_id
                    end
                    save_curwin=curwin;
                    curwin=max(winsid())+1;
                    [orpar,oipar,ok]=poke_point(xy,ipar,rpar);
                    curwin=save_curwin;
                    if ~ok then
                        break;
                    end;//  exit without save

                    // verifying the data change
                    N2=oipar(1);
                    xy2=[orpar(1:N2),orpar(N2+1:2*N2)];
                    New_methhod=oipar(2);
                    DChange=%f;
                    METHOD=getmethod(New_methhod);
                    if or(xy(:,1)<>xy2(:,1)) then,
                        DChange=%t;
                    end
                    if or(xy(1:N-1,2)<>xy2(1:N2-1,2)) then,
                        DChange=%t;
                    end
                    if (xy(N,2)<>xy2(N2,2) & (METHOD<>"periodic")) then,
                        DChange=%t;
                    end
                    if DChange then
                        exprs(2)=strcat(sci2exp(xy2(:,1)))
                        exprs(3)=strcat(sci2exp(xy2(:,2)))
                    end
                    exprs(1)=sci2exp(New_methhod);
                    exprs(4)=sci2exp(oipar(4));
                    if oipar(3)==1 then,
                        perop="y";
                    else,
                        perop="n";
                    end
                    SaveExit=%t
                else//_____________________No graphics__________________________
                    [Xdummy,Ydummy,orpar]=Do_Spline(N,mtd,xy(:,1),xy(:,2),xy($,1),xy(1,1),0);
                    if (METHOD=="periodic") then // periodic spline
                        xy(N,2)=xy(1,2);
                    end
                    if (METHOD=="order 2" | METHOD=="not_a_knot"|METHOD=="periodic" | METHOD=="monotone"| METHOD=="fast" | METHOD=="clamped") then
                        orpar=[xy(:,1);xy(:,2);orpar];
                    else
                        if (METHOD=="zero order-below"|METHOD=="linear"|METHOD=="zero order-above"|METHOD=="zero order-nearest") then
                            orpar=[xy(:,1);xy(:,2);]
                        end
                    end
                    exprs(1)=sci2exp(mtd);// pour le cas methode>7 | method<0
                    oipar=[N;mtd;PO;extrapo]
                    SaveExit=%t
                end //___________________________________________________________
            end //++++++++++++++++++++++++++++++++++++++++++++++++++++++

            if (SaveExit) then
                xp=find(orpar(1:oipar(1))>=0);
                if (xp<>[]) then
                    model.firing=orpar(xp(1)); //first positive event
                else
                    model.firing=-1;
                end
                model.rpar=orpar
                model.ipar=oipar
                graphics.exprs=exprs;
                x.model=model
                x.graphics=graphics
                break
            end
        end
    case "define" then
        model=scicos_model()

        xx=[-1;0.5;1;1.5;2.5]
        yy=[-6;-1;-3;3;-4]
        N=length(xx);
        Method=1;
        Graf="n"
        model.sim=list("lookup_c",4)
        model.in=-1
        model.in2=-2
        model.outtyp=-1

        model.out=-1
        model.out2=-2
        model.outtyp=-1

        model.rpar=[xx(:);yy(:)]
        model.ipar=[N;Method;0;0]
        model.blocktype="c"
        model.dep_ut=[%t %f]
        model.evtin=[]
        model.evtout=[]
        model.firing=0
        exprs=[sci2exp(Method);sci2exp(xx);sci2exp(yy);sci2exp(0);Graf]

        gr_i=[]
        x=standard_define([2 2],model,exprs,gr_i)
    end
endfunction



function [rpar,ipar,ok]=poke_point(ixy,iparin,rparin)
    [lhs,rhs]=argn(0)
    //in line definition of get_click
    deff("[btn,xc,yc,win,Cmenu]=get_click(flag)",[
    "if ~or(winsid() == curwin) then   Cmenu = ''Quit'';return,end,";
    "if argn(2) == 1 then";
    "  [btn, xc, yc, win, str] = xclick(flag);";
    "else";
    "  [btn, xc, yc, win, str] = xclick();";
    "end;";
    "if btn == -100 then";
    "  if win == curwin then";
    "    Cmenu = ''Quit'';";
    "  else";
    "    Cmenu = ''Open/Set'';";
    "  end,";
    "  return,";
    "end";
    "if btn == -2 then";
    "  xc = 0;yc = 0;";
    "  try "    // added to handle unwanted menu actions in french version
    "    execstr(''Cmenu='' + part(str, 9:length(str) - 1));";
    "    execstr(''Cmenu='' + Cmenu);";
    "  catch"
    "    Cmenu=[]"
    "  end "
    "  return,";
    "end";
    "Cmenu=[]"])

    ok=%f
    if rhs==0 then
        ixy=[];
    end;
    if size(xy,"c")<2 then
        xinfo(" No y provided");
        return
    end

    [xy]=cleandata(ixy)
    N=size(xy,"r");

    if rhs<=1 then
        NOrder=1;
        PeridicOption=0;
        extrapo=0
        ipar=[N;NOrder;PeridicOption;extrapo]
        rpar=[]
    elseif rhs==2 then
        NOrder=iparin(2);
        PeridicOption=iparin(3);
        extrapo=iparin(4);
        ipar=iparin;
        rpar=[]
    elseif rhs==3 then
        NOrder=iparin(2);
        PeridicOption=iparin(3);
        extrapo=iparin(4);
        ipar=iparin;
        rpar=rparin
    end

    Amp=[];
    wp=[];
    phase=[];
    offset=[];
    np1=[];
    Sin_exprs=list(string(Amp),string(wp), string(phase),string(offset),string(np1));
    sAmp=[];
    sTp=[];
    sdelay=[];
    Sawt1_exprs=list(string(sAmp),string(sTp),string(sdelay));
    sAmp2=[];
    sTp2=[];
    Sawt2_exprs=list(string(sAmp2),string(sTp2));

    Amp3=[];
    Tp3=[];
    Pw3=[];
    Pd3=[];
    Bias3=[];
    Pulse_exprs=list(string(Amp3), string(Tp3),string(Pw3),string(Pd3),string(Bias3))

    mean4=[];
    var4=[];
    seed4=[];
    sample4=[];
    np4=[];
    random_n_exprs=list(string(mean4),string(var4), string(seed4),string(sample4),string(np4))

    min5=[];
    max5=[];
    seed5=[];
    sample5=[];
    np5=[];
    random_u_exprs=list(string(min5), string(max5), string(seed5),string(sample5),string(np5))

    // bornes initiales du graphique
    xmx=max(xy(:,1));
    xmn=min(xy(:,1)),
    ymx=max(xy(:,2));
    ymn=min(xy(:,2));
    dx=xmx-xmn;
    dy=ymx-ymn
    if dx==0 then
        dx=max(xmx/2,1),
    end;
    xmx=xmx+dx/50;
    if dy==0 then
        dy=max(ymx/2,1),
    end;
    ymn=ymn-dy/50;
    ymx=ymx+dy/50;

    rect=[xmn,ymn;xmx,ymx];
    //===================================================================
    f=scf();

    if getos() <> "Windows" then
        delmenu(curwin,"3D Rot.")
        delmenu(curwin,"Edit")
        delmenu(curwin,"File")
        delmenu(curwin,"Insert")
    else
        hidetoolbar(curwin)
        // French
        delmenu(curwin,"&Fichier")
        delmenu(curwin,"&Editer")
        delmenu(curwin,"&Outils")
        delmenu(curwin,"&Inserer")
        // English
        delmenu(curwin,"&File")
        delmenu(curwin,"&Edit")
        delmenu(curwin,"&Tools")
        delmenu(curwin,"&Insert")
    end
    //menuss=menus;menuss(1)=menus(1)(2:$);menubar(curwin,menuss)

    menu_r=[];
    menu_s=[];
    menu_o=["zero order-below","linear","order 2","not_a_knot","periodic","monotone","fast","clamped","zero order-above","zero order-nearest"]
    menu_d=["Clear","Data Bounds","Load from text file","Save to text file","Load from Excel","Extrapolation"]
    menu_t=["sine","sawtooth1","sawtooth2","pulse","random normal","random uniform"]
    menu_e=["Help","Exit without save","Save/Exit"]
    MENU=["Autoscale","Spline","Data","Standards","Exit"];
    menus=list(MENU,menu_s,menu_o,menu_d,menu_t,menu_e);

    scam="menus(1)(1)"
    w="menus(3)(";r=")";
    Orderm=w(ones(menu_o))+string(1:size(menu_o,"*"))+r(ones(menu_o))
    w="menus(4)(";r=")";
    Datam=w(ones(menu_d))+string(1:size(menu_d,"*"))+r(ones(menu_d))
    w="menus(5)(";r=")";
    Standm=w(ones(menu_t))+string(1:size(menu_t,"*"))+r(ones(menu_t))
    w="menus(6)(";r=")";
    Exitm=w(ones(menu_e))+string(1:size(menu_e,"*"))+r(ones(menu_e))

    execstr("Autoscale_"+string(curwin)+"=scam")
    execstr("Spline_"+string(curwin)+"=Orderm")
    execstr("Data_"+string(curwin)+"=Datam")
    execstr("Standards_"+string(curwin)+"=Standm")
    execstr("Exit_"+string(curwin)+"=Exitm")

    addmenu(curwin,MENU(1))
    addmenu(curwin,MENU(2),menu_o)
    addmenu(curwin,MENU(3),menu_d)
    addmenu(curwin,MENU(4),menu_t)
    addmenu(curwin,MENU(5),menu_e)
    //===================================================================
    //initial draw
    drawlater();
    a=gca(f);
    a.data_bounds=rect;
    a.axes_visible="on";
    a.clip_state="on";
    xtitle( "", "time", "Output" ) ;
    a.title.font_size=2;
    a.title.font_style=4;
    a.title.foreground=2;

    a.grid=[2 2];
    xpolys(xy(:,1),xy(:,2),[-1]);   //children(2)
    xpolys(xy(:,1),xy(:,2),[5]);    //children(1)
    splines=a.children(1).children
    points=a.children(2).children
    //---------------------------------------

    [rpar,ipar]=AutoScale(a,xy,ipar,rpar)
    drawnow();
    // -- boucle principale
    lines(0);
    while %t then //=================================================
        N=size(xy,"r");
        [btn,xc,yc,win,Cmenu]=get_click();
        if ((win>0) & (win<>curwin)) then
            Cmenu="Mouse click is Offside!";
        end
        if Cmenu==[] then Cmenu="edit",end
        if (Cmenu=="Exit") |(Cmenu=="Quit" ) then,
            ipar=[];
            rpar=[];
            ok=%f;return;
        end
        //-------------------------------------------------------------------
        if ((Cmenu=="zero order-below") | (Cmenu=="linear") | (Cmenu=="order 2")| ...
            (Cmenu=="not_a_knot")| (Cmenu=="periodic")| (Cmenu=="monotone")| ...
            (Cmenu=="fast")| (Cmenu=="clamped") |(Cmenu=="zero order-above")|(Cmenu=="zero order-nearest")) then

            select  Cmenu
            case "zero order-below" then
                NOrder=0;
            case "linear" then
                NOrder=1;
            case "order 2" then
                NOrder=2;
            case "not_a_knot" then
                NOrder=3;
            case "periodic" then
                NOrder=4;
            case "monotone" then
                NOrder=5;
            case "fast" then
                NOrder=6;
            case "clamped" then
                NOrder=7;
            case "zero order-above" then
                NOrder=8;
            case "zero order-nearest" then
                NOrder=9;
            end
            ipar(2)=NOrder;
            [rpar,ipar]=AutoScale(a,xy,ipar,rpar)
        end
        //-------------------------------------------------------------------
        select Cmenu
        case "Data Bounds" then
            rectx=findrect(a);
            [mok,xmn1,xmx1,ymn1,ymx1]=scicos_getvalue("Enter new bounds",["xmin";"xmax"; ...
            "ymin";"ymax"],list("vec",1,"vec",1,"vec",1,"vec",1), ...
            string(rectx))
            //drawlater();
            if mok then
                if (xmn1>xmx1|ymn1>ymx1) then
                    xinfo("Incorrect bounds")
                    mok=%f;
                end
                if mok then
                    a.data_bounds=[xmn1,ymn1;xmx1,ymx1];
                end
            end
            //drawnow();
            //-------------------------------------------------------------------
        case "Autoscale" then
            [rpar,ipar]=AutoScale(a,xy,ipar,rpar)
            //-------------------------------------------------------------------
        case "Extrapolation" then
            //extrapo
            if extrapo==1 then,
                ans0="1",
            else,
                ans0="0",
            end;
            [mok,myans]=scicos_getvalue("Extrapolation method (just for Method 1)",["0: hold end values, 1: extrapolation"],list("vec",1),list(ans0));
            if (mok==%t) then
                extrapo=int(myans);
                if extrapo<0 then
                    extrapo=0;
                end;
                if extrapo>1 then
                    extrapo=1;
                end;
                ipar(4)=extrapo;
                [rpar,ipar]=AutoScale(a,xy,ipar,rpar)
            end
            //-------------------------------------------------------------------
        case "sine" then
            [mok,Amp,wp,phase,offset,np1,Sin_exprs2]=scicos_getvalue(" Sine parameters", ...
            ["Amplitude";"Frequency(rad/sec)"; ...
            "Phase(rad)";"Bias";"number of points"],list("vec",1,"vec",1,"vec",1, ...
            "vec",1,"vec",1),Sin_exprs)
            if np1< 2 then
                np1=2;
            end
            if mok & wp>0  then
                NOrder=3;
                ipar(2)=NOrder;
                phase=atan(tan(phase));
                xt=linspace(0,%pi*2/wp,np1)';
                yt=Amp*sin(wp*xt+phase)+offset;
                xy=[xt,yt];
                [rpar,ipar]=AutoScale(a,xy,ipar,rpar)
                Sin_exprs=Sin_exprs2
            end
            //-------------------------------------------------------------------
        case "sawtooth1" then
            [mok,sAmp,sTp,sdelay,Sawt1_exprs2]=scicos_getvalue("Sawtooth signal parameters", ...
            ["Amplitude";"Period";"delay"], ...
            list("vec",1,"vec",1,"vec",1),Sawt1_exprs)
            if mok & sTp>0 then
                NOrder=1;
                ipar(2)=NOrder;
                if sdelay<sTp then
                    xt=[0;sdelay;sTp];
                    yt=[0;0;sAmp];
                else
                    xt=[0];
                    yt=[0];
                end
                xy=[xt,yt];
                [rpar,ipar]=AutoScale(a,xy,ipar,rpar);
                Sawt1_exprs=Sawt1_exprs2
            end
            //-------------------------------------------------------------------
        case "sawtooth2" then
            [mok,sAmp2,sTp2,Sawt2_exprs2]=scicos_getvalue("Sawtooth signal parameters", ...
            ["Amplitude";"Period"],list("vec",1,"vec",1),Sawt2_exprs)
            if mok & sTp2>0 then
                NOrder=1;
                ipar(2)=NOrder;
                xt=[0;sTp2];
                yt=[sAmp2;-sAmp2];
                xy=[xt,yt];
                [rpar,ipar]=AutoScale(a,xy,ipar,rpar);
                Sawt2_exprs=Sawt2_exprs2
            end
            //-------------------------------------------------------------------
        case "pulse" then
            [mok,Amp3,Tp3,Pw3,Pd3,Bias3,Pulse_exprs2]=scicos_getvalue("Square wave pulse signal", ...
            ["Amplitude";"Period (sec)";"Pulse width(% of period)";"Phase delay (sec)";"Bias"],list("vec",1, ...
            "vec",1,"vec",1,"vec",1,"vec", ...
            1),Pulse_exprs)
            if mok & Tp3>0  then
                NOrder=0;
                ipar(2)=NOrder;
                if (Pd3>0) then
                    xt=0;
                    yt=Bias3;
                else
                    xt=[];
                    yt=[];
                end
                //otherwise there	would be double	points at 0
                if Pd3<Tp3 then
                    if Pw3>0 then
                        xt=[xt;Pd3; Pw3*Tp3/100+Pd3;Tp3];
                        yt=[yt;Amp3+Bias3;Bias3;Bias3];
                    else
                        xt=[0;Tp3];
                        yt=[Bias3;Bias3];
                    end
                else
                    xt=[0;Tp3];
                    yt=[Bias3;Bias3];
                end

                xy=[xt,yt];
                [rpar,ipar]=AutoScale(a,xy,ipar,rpar);
                Pulse_exprs=Pulse_exprs2;
            end
            //-------------------------------------------------------------------
        case "random normal" then
            [mok,mean4,var4,seed4,sample4,np4,random_n_exprs2]=scicos_getvalue("Normal (Gaussian) random signal", ...
            ["Mean";"Variance";"Initial seed";"Sample time";"Number of points"],list("vec",1, ...
            "vec",1,"vec",1,"vec", ...
            1,"vec",1),random_n_exprs)
            if mok & sample4>0 then
                NOrder=0;
                ipar(2)=NOrder;
                rand("normal");
                rand("seed",seed4);
                xt=0:sample4:sample4*(np4-1);
                xt=xt(:);
                yt=mean4+sqrt(var4)*rand(np4,1);
                xy=[xt,yt];
                [rpar,ipar]=AutoScale(a,xy,ipar,rpar);
                random_n_exprs2=random_n_exprs;
            end
            //-------------------------------------------------------------------
        case "random uniform" then
            [mok,min5,max5,seed5,sample5,np5,random_u_exprs2]=scicos_getvalue("Uniform random signal", ...
            ["Minimum";"Maximum";"Initial seed";"Sample time";"Number of points"],list("vec",1, ...
            "vec",1,"vec",1,"vec", ...
            1,"vec",1),random_u_exprs)
            if mok & sample5>0 then
                NOrder=0;
                ipar(2)=NOrder;
                rand("uniform");
                rand("seed",seed5);
                xt=0:sample5:sample5*(np5-1);
                xt=xt(:);
                yt=min5+(max5-min5)*rand(np5,1);
                xy=[xt,yt];
                [rpar,ipar]=AutoScale(a,xy,ipar,rpar);
                random_u_exprs2=random_u_exprs;

            end
            //-------------------------------------------------------------------
        case "Save/Exit" then
            NOrder=ipar(2);
            PeridicOption=ipar(3);

            METHOD=getmethod(NOrder);
            if (METHOD=="periodic") then // periodic spline
                xy(N,2)=xy(1,2);
            end

            if (METHOD=="order 2" | METHOD=="not_a_knot"|METHOD=="periodic" | METHOD=="monotone"| METHOD=="fast" | METHOD=="clamped") then
                rpar=[xy(:,1);xy(:,2);rpar];
            else
                if (METHOD=="zero order-below"|METHOD=="linear"|METHOD=="zero order-above"|METHOD=="zero order-nearest")
                    rpar=[xy(:,1);xy(:,2);]
                end
            end

            ok=%t
            delete(f);
            return
            //-------------------------------------------------------------------
        case "Exit without save" then
            ipar=[];
            rpar=[];
            ok=%f
            delete(f);
            return
            //-------------------------------------------------------------------
        case "Clear" then
            xy=[0,0];
            NOrder=0;
            ipar(2)=NOrder;
            [rpar,ipar]=AutoScale(a,xy,ipar,rpar)
            //----------------------------------------------------------------
        case "Edit text data NOT IN USE" then
            //  editvar xy;
            [mok,xt,yt]=scicos_getvalue("Enter x and y data",["x";"y"],list("vec",-1,"vec",-1),list(strcat(sci2exp(xy(:,1))),strcat(sci2exp(xy(:,2)))));
            if mok then,
                xy=[xt,yt];
                [xy]=cleandata(xy),
                [rpar,ipar]=AutoScale(a,xy,ipar,rpar)
            end
            //---------------------------------------------------------------
        case "Help" then
            t1="Mouse-left click: adding a new point"
            t2="Mouse-right click: remove a point"
            t3="Mouse-left double click: edit a point''s coordinates"
            t4="Mouse-left button press/drag/release: move a  point"
            t5="Change the window size: ''Data'' menu -> ''Databounds''"
            messagebox([t1;t2;t3;t4;t5]);
            //---------------------------------------------------------------
        case "Load from Excel" then
            [tok,xytt]=ReadExcel()
            if tok then
                xy=xytt;
                NOrder=1
                ipar(2)=NOrder;
                [rpar,ipar]=AutoScale(a,xy,ipar,rpar)
            end
            //---------------------------------------------------------------
        case "Load from text file" then
            [tok,xytt]=ReadFromFile()
            if tok then
                xy=xytt;
                NOrder=1
                ipar(2)=NOrder;
                [rpar,ipar]=AutoScale(a,xy,ipar,rpar)
            end
            //---------------------------------------------------------------
        case "Save to text file" then
            [sok]=SaveToFile(xy)
            //---------------------------------------------------------------
        case "Replot" then
            if xy<>[] then
                drawlater();
                points.data=xy;
                [rpar,ipar]=drawSplin(a,xy,ipar,rpar);
                drawnow()
            end
            //----------------------------------------------------------
        case "edit" then
            HIT=%f
            if N<>0 then
                xt=xy(:,1);
                yt=xy(:,2);
                dist=((xt-ones(N,1)*xc))^2+((yt-ones(N,1)*yc))^2
                [dca,k]=min(dist);
                rectx=a.data_bounds;
                ex=abs(rectx(2,1)-rectx(1,1))/80;
                ey=abs(rectx(2,2)-rectx(1,2))/80;
                if (abs(xc-xt(k))<ex & abs(yc-yt(k))<ey) then
                    HIT=%t
                end
            end

            //_________________________
            //  if ~((NOrder==-1|NOrder==-2|NOrder==-3|NOrder==-4)) then
            if (~HIT)&(btn==0 | btn==3) then    // add point
                xy=[xy;xc,yc];
                [xtt,k2]=gsort(xy(:,1),"r","i");xy=xy(k2,:)
                drawlater();
                points.data=xy;
                [rpar,ipar]=drawSplin(a,xy,ipar,rpar);
                drawnow()
            end

            if (HIT)&(btn==2 | btn==5) then  //   remove point
                xy(k,:)=[];
                drawlater();
                points.data=xy;
                [rpar,ipar]=drawSplin(a,xy,ipar,rpar);
                drawnow()
            end

            if (HIT)&(btn==0) then             // move point
                [xy,rpar,ipar]=movept(a,xy,ipar,rpar,k)
            end

            if (HIT)&(btn==10) then             // change data:: double click
                [mok,xt,yt]=scicos_getvalue("Enter new x and y",["x";"y"],list("vec", ...
                1,"vec",1),list(sci2exp(xy(k,1)),sci2exp(xy(k,2))));
                if mok then
                    xy(k,:)=[xt,yt];
                    [xy]=cleandata(xy)
                    drawlater();
                    points.data=xy;
                    [rpar,ipar]=AutoScale(a,xy,ipar,rpar)
                    drawnow()
                end
            end

            //  end
            //_________________________________

        end
        //----------------------------------------------------------
    end
endfunction
//========================================================================
function [orpar,oipar]=drawSplin(a,xy,iipar,irpar)
    N=size(xy,"r");// new size of xy
    x=xy(:,1);
    y=xy(:,2);
    points=a.children(2).children
    splines=a.children(1).children
    order=iipar(2);
    periodicoption=iipar(3);
    extrapo=iipar(4);
    orpar=irpar;

    METHOD=getmethod(order);

    if periodicoption==1 then
        PERIODIC="periodic, T="+string(x(N)-x(1));
    else
        PERIODIC="aperiodic";
    end
    a.title.text=[string(N)+" points,  "+"Method: "+METHOD+",  "+PERIODIC];

    if (N==0) then,
        return;
    end
    if (N==1) then,
        order=0;
    end
    //  NP=50;// number of intermediate points between two data points

    xmx=max(points.data(:,1));
    xmn=min(points.data(:,1));
    xmx1=max(a.x_ticks.locations);
    xmn1=min(a.x_ticks.locations)
    xmx=max(xmx,xmx1);
    xmn=min(xmn,xmn1);
    [X,Y,orpar]=Do_Spline(N,order,x,y,xmx,xmn,extrapo);

    if (periodicoption==1) then
        X=[X;X($)];
        Y=[Y;Y(1)];
    else
        //X=[X;XMX];
        //Y=[Y;Y($)];
    end
    //!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
    splines.data=[X,Y];
    oipar=[N;iipar(2);periodicoption;extrapo]
endfunction
//=============================================================
function [xyt,orpar,oipar]=movept(a,xy,iipar,irpar,k)
    //on bouge un point existant
    points=a.children(2).children
    splines=a.children(1).children
    oipar=iipar
    orpar=irpar
    order=iipar(2);
    x=xy(:,1);
    y=xy(:,2);

    x(k)=[];
    y(k)=[];

    btn=-1

    while ~(btn==3 | btn==0| btn==10| btn==-5)
        rep=xgetmouse([%t %t]);
        xc=rep(1);
        yc=rep(2);
        btn=rep(3);

        xt=[x;xc];
        yt=[y;yc];
        [xt,k2]=gsort(xt,"r","i");yt=yt(k2)
        xyt=[xt,yt];

        drawlater();
        points.data=xyt;
        [orpar,oipar]=drawSplin(a,xyt,oipar,orpar);
        drawnow()
    end

endfunction

//==========================================================
function   rectx=findrect(a)
    splines=a.children(1).children
    points=a.children(2).children

    if (points.data==[]) then
        rectx=a.data_bounds;
        return;
    end


    ymx1=max(splines.data(:,2));
    ymn1=min(splines.data(:,2))

    xmx=max(points.data(:,1));
    xmn=min(points.data(:,1));
    ymx=max(points.data(:,2));
    ymn=min(points.data(:,2));


    XMX=max(xmx);
    XMN=max(xmn);
    YMX=max(ymx,ymx1);
    YMN=min(ymn,ymn1);

    dx=XMX-XMN;
    dy=YMX-YMN
    if dx==0 then
        dx=max(XMX/2,1),
    end;
    XMX=XMX+dx/50
    if dy==0 then
        dy=max(YMX/2,1),
    end;
    YMN=YMN-dy/50;
    YMX=YMX+dy/50;
    rectx=[XMN,YMN;XMX,YMX];
endfunction

//============================================================
function [tok,xyo]=ReadExcel()
    TA=["A";"B";"C";"D";"E";"F";"G";"H";"I";"J";"K";"L";"M";"N";"O";"P"; ...
    "Q";"R";"S";"T";"U";"V";"W";"X";"Y";"Z";"a";"b";"c";"d";"e";"f"; ...
    "g";"h";"i";"j";"k";"l";"m";"n";"o";"p";"q";"r";"s";"t";"u";"v"; ...
    "w";"x";"y";"z"];
    TN=["0","1","2","3","4","5","6","7","8","9"];
    xyo=[];
    tok=%f;
    while %t
        [zok,filen,sheetN,xa,ya]=scicos_getvalue("Excel data file ",["Filename";"Sheet # ";"X[start:Stop]";"Y[start:stop]"],list("str",1, ...
        "vec",1,"str",1, ...
        "str",1), ...
        list(["Classeur1.xls"],["1"],["C5:C25"],["D5:D25"]));
        if ~zok then
            break,
        end

        try
            [fd,SST,Sheetnames,Sheetpos] = xls_open(filen);
        catch
            xinfo("Scicos cannot find the excel file:"+filen);
            break;
        end
        try
            N=size(Sheetnames,"*");
            if ((sheetN<=N) &(sheetN>0)) then
                [Value,TextInd] = xls_read(fd,Sheetpos(sheetN))
                mclose(fd)
            end
            xa=strsubst(xa," ","");
            px=strindex(xa,":");
            ya=strsubst(ya," ","");
            py=strindex(ya,":");
            x1=part(xa,1:px-1);
            x2=part(xa,px+1:length(xa));
            y1=part(ya,1:py-1);
            y2=part(ya,py+1:length(ya));

            x1p=min(strindex(x1,TN));
            if x1p==[] then,
                xinfo("Bad address in X:"+x1);
                break,
            end
            x11=part(x1,1:x1p-1);
            x12=part(x1,x1p:length(x1));

            x2p=min(strindex(x2,TN));
            if x2p==[] then,
                xinfo("Bad address in X:"+x2);
                break,
            end
            x21=part(x2,1:x2p-1);
            x22=part(x2,x2p:length(x2));

            y1p=min(strindex(y1,TN));
            if y1p==[] then,
                xinfo("Bad address in Y:"+y1);
                break,
            end
            y11=part(y1,1:y1p-1);
            y12=part(y1,y1p:length(y1));

            y2p=min(strindex(y2,TN));
            if y2p==[] then,
                xinfo("Bad address in Y:"+y2);
                break,
            end
            y21=part(y2,1:y2p-1);
            y22=part(y2,y2p:length(y2));

            // x11 x12: x21 x22

            lx11=length(x11);
            lx21=length(x21);
            ly11=length(y11);
            ly21=length(y21)
            xstC=0;
            for i=1:lx11,
                xstC=xstC+modulo(find(TA==part(x11,lx11-i+1)),26)*26^(i-1);
            end
            xenC=0;
            for i=1:lx21,
                xenC=xenC+modulo(find(TA==part(x21,lx21-i+1)),26)*26^(i-1);
            end
            ystC=0;
            for i=1:ly11,
                ystC=ystC+modulo(find(TA==part(y11,ly11-i+1)),26)*26^(i-1);
            end
            yenC=0;
            for i=1:ly11,
                yenC=yenC+modulo(find(TA==part(y21,ly21-i+1)),26)*26^(i-1);
            end

            xstR=evstr(x12);
            xenR=evstr(x22);
            ystR=evstr(y12);
            yenR=evstr(y22);

            [mv,nv]=size(Value)

            if ~(xstR<=mv & xstR>0 & xenR<=mv & xenR>0&ystR<=mv & ystR>0&yenR<=mv&yenR>0 ) then
                xinfo("error in Row data addresses");
                break
            end
            if ~(xstC<=nv & xstC>0 & xenC<=nv & xenC>0&ystC<=nv & ystC>0&yenC<=nv&yenC>0 ) then
                xinfo("error in Column data addresses");
                break
            end

            xo=Value(min(xstR,xenR):max(xstR,xenR),min(xstC,xenC):max(xstC,xenC));
            yo=Value(min(ystR,yenR):max(ystR,yenR),min(ystC,yenC):max(ystC,yenC));
            [nx,mx]=size(xo);// adjusting the x and y size
            [ny,my]=size(yo);
            N=min(nx,ny);
            xo=xo(1:N,:);
            yo=yo(1:N,:);

            xyo=[xo,yo];
            [xyo]=cleandata(xyo)

            tok=%t;
            break,
        catch
            xinfo(" Scicos cannot read your Excel file, please verify the parameters ");
            break
        end
    end

endfunction
//---------------------------------------------------------------
function [xyo]=cleandata(xye)
    xe=xye(:,1)
    ye=xye(:,2)

    [nx,mx]=size(xe);// adjusting the x and y size
    [ny,my]=size(ye);
    N=min(nx,ny);
    xe=xe(1:N,:);
    ye=ye(1:N,:);

    // checking for NULL data
    for i=1:N
        if (xe(i)<>xe(i)) then
            xinfo("x contains no data:x("+string(i)+")");
            return;
        end
        if (ye(i)<>ye(i)) then
            xinfo("Y contains no data:y("+string(i)+")");
            return;
        end
    end

    [xo,k2]=gsort(xe,"r","i");
    yo=ye(k2)

    xyo=[xo,yo];
endfunction
//---------------------------------------------------------------
function  [orpar,oipar]=AutoScale(a,xy,inipar,inrpar)
    drawlater();
    oipar=inipar
    orpar=inrpar
    points=a.children(2).children
    splines=a.children(1).children
    points.data=xy;
    splines.data=xy;
    [orpar,oipar]=drawSplin(a,xy,oipar,orpar);
    rectx=findrect(a);
    a.data_bounds=rectx;
    drawnow()
endfunction
//============================
function METHOD=getmethod(order)
    select order
    case 0 then,
        METHOD="zero order-below"
    case 1 then,
        METHOD="linear"
    case 2 then,
        METHOD="order 2"
    case 3 then,
        METHOD="not_a_knot"
    case 4 then,
        METHOD="periodic"
    case 5 then,
        METHOD="monotone"
    case 6 then,
        METHOD="fast"
    case 7 then,
        METHOD="clamped"
    case 8 then,
        METHOD="zero order-above"
    case 9 then,
        METHOD="zero order-nearest"
    end
endfunction
//=======================================
function [sok,xye]=ReadFromFile()
    xye=[];
    sok=%f;
    while %t
        [sok,filen,Cformat,Cx,Cy]=scicos_getvalue("Text data file ",["Filename";"Reading [C] format";"Abscissa column";"Output column"],list("str",1,"str",1,"vec",1,"vec",1), ...
        list(["mydatafile.dat"],["%g %g"],["1"],["2"]));
        if ~sok then
            break,
        end
        px=strindex(Cformat,"%");
        NC=size(px,"*");
        if NC==[] then,
            xinfo("Bad format in reading data file");
            sok=%f;
            break;
        end
        Lx=[];
        try
            fd=mopen(filen,"r");
            Lx=mfscanf(-1,fd,Cformat);
            mclose(fd);
        catch
            xinfo("Scicos cannot open the data file:"+filen);
            break;
        end

        [nD,mD]=size(Lx);
        if ((mD==0) | (nD==0)) then,
            xinfo("No data read");
            sok=%f;
            break;
        end
        if (mD<>NC) then,
            xinfo("Bad format");
            sok=%f;
            break;
        end

        xe=Lx(:,Cx);ye=Lx(:,Cy);
        xye=[xe,ye];
        [xye]=cleandata(xye)
        sok=%t;
        break,
    end
endfunction
//=======================================
function [sok]=SaveToFile(xye)
    xe=xye(:,1)
    ye=xye(:,2)
    sok=%f;
    while %t
        [sok,filen,Cformat]=scicos_getvalue("Text data file ",["Filename";"Writing [C] format"],list("str",1,"str",1), ...
        list(["mydatafile.dat"],["%g %g"]));
        if ~sok then
            break,
        end
        px=strindex(Cformat,"%");
        NC=size(px,"*");
        if NC<>2 then,
            xinfo("Bad format in writing data file");
            sok=%f;
            break;
        end

        Cformat=Cformat+"\n";

        try
            fd=mopen(filen,"w");
            mfprintf(fd,Cformat,xe,ye);
            mclose(fd);
        catch
            xinfo("Scicos cannot open the data file:"+filen);
            break;
        end

        sok=%t;
        break,
    end
endfunction
//=========================================================
function [X,Y,orpar]=Do_Spline(N,order,x,y,xmx,xmn,extrapo)
    X=[];
    Y=[];
    orpar=[];

    METHOD=getmethod(order);

    if (METHOD=="zero order-below") then
        X=[xmn;x(1)];
        Y=[y(1);y(1)];
        for i=1:N-1
            X=[X;x(i+1);x(i+1)];
            Y=[Y;y(i);y(i+1)];
        end
        X=[X;xmx];
        Y=[Y;y(N)];
        return
    end
    if (METHOD=="zero order-above") then
        X=[xmn;x(1)];
        Y=[y(1);y(1)];
        for i=1:N-1
            X=[X;x(i);x(i+1)];
            Y=[Y;y(i+1);y(i+1)];
        end
        X=[X;xmx];
        Y=[Y;y(N)];
        return
    end
    if (METHOD=="zero order-nearest") then
        X=[xmn;x(1)];
        Y=[y(1);y(1)];
        for i=1:N-1
            X=[X;(x(i)+x(i+1))/2;(x(i)+x(i+1))/2];
            Y=[Y;y(i);y(i+1)];
        end
        X=[X;xmx];
        Y=[Y;y(N)];
        return
    end
    //!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
    if (METHOD=="linear") then

        if N<=1 then
            return;
        end
        if extrapo==0 then
            X=[xmn];
            Y=[y(1)];
        end
        if extrapo==1 then
            X=[xmn];
            Y=y(1)+(xmn-x(1))*(y(1)-y(2))/(x(1)-x(2));
        end
        for i=1:N
            X=[X;x(i)];
            Y=[Y;y(i)];
        end
        if extrapo==0 then
            X=[X;xmx];
            Y=[Y;y(N)];
        end
        if extrapo==1 then
            X=[X;xmx];
            Y=[Y;y(N)+(xmx-x(N))*(y(N)-y(N-1))/(x(N)-x(N-1))];
        end
        return
    end
    //!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
    if (N<25) then
        NP=10;
    elseif (N<50) then
        NP=5;
    elseif (N<100) then
        NP=2;
    elseif (N<200) then
        NP=1;
    else
        NP=0;
    end
    for i=1:N-1
        X=[X;linspace(x(i),x(i+1),NP+2)']; // pour tous sauf "linear" et "zero order"
    end
    if extrapo==1 then
        X=[linspace(xmn,x(1),NP+2)';X;linspace(x(N),xmx,NP+2)'];
    end
    //!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
    if (N>2) & (METHOD=="order 2") then
        Z=ORDER2(x,y);
        A=Z(1:N-1);
        B=Z(N:2*N-2);
        C=Z(2*N-1:3*N-3);
        for j=1:size(X,"*")
            for i=N-1:-1:1
                if X(j)>=x(i) then,
                    break;
                end
            end
            Y(j)=A(i)*(X(j)-x(i))^2+B(i)*(X(j)-x(i))+C(i);
        end
        orpar=matrix(Z,-1,1)
    end
    //!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
    if (METHOD=="not_a_knot") then
        try
            d = splin(x, y, METHOD);
            Y = interp(X, x, y, d);
            orpar=d(:);
        catch
            xinfo("ERROR in SPLINE: "+METHOD)
        end

    end
    //!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
    if (METHOD=="periodic") then
        if y(1)<>y(N) then
            y(N)=y(1)
        end
        try
            d = splin(x, y,METHOD);
            Y = interp(X, x, y, d);
            orpar=d(:);
        catch
            xinfo("ERROR in SPLINE: "+METHOD)
        end
    end
    //!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
    if (METHOD=="monotone" ) then
        try
            d = splin(x, y, METHOD);
            Y = interp(X, x, y, d);
            orpar=d(:);
        catch
            xinfo("ERROR in SPLINE: "+METHOD)
        end

    end
    //!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
    if (METHOD=="fast") then
        try
            d = splin(x, y, METHOD);
            Y = interp(X, x, y, d);
            orpar=d(:);
        catch
            xinfo("ERROR in SPLINE:  "+METHOD)
        end
    end
    //!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
    if (METHOD=="clamped") then
        try
            d = splin(x, y, METHOD,[0;0]);
            Y = interp(X, x, y, d);
            orpar=d(:);
        catch
            xinfo("ERROR in SPLINE: "+METHOD)
        end
    end
    if extrapo==0 then
        X=[X;xmx];
        Y=[Y;y(N)];
    end

endfunction
//=================================================
function [Z]=ORDER2(x,y)
    N=size(x,"*")-1;
    A=zeros(3*N-1,N*3);
    B=zeros(3*N-1,1);
    for i=1:N
        j=3*(i-1)+1;
        A(j,i+2*N)=1;
        B(j)=y(i);
        A(j+1,i)=(x(i+1)-x(i))^2;
        A(j+1,i+N)=x(i+1)-x(i);
        A(j+1,i+2*N)=1;
        B(j+1)=y(i+1);
    end

    for i=1:N-1
        j=3*(i-1)+1;
        A(j+2,i)=2*(x(i+1)-x(i));
        A(j+2,i+N)=1;
        A(j+2,i+N+1)=-1;
    end

    Q=zeros(3*N,3*N);
    for i=1:N
        Q(i,i)=4*(x(i+1)-x(i))^2
        Q(i,i+N)=2*(x(i+1)-x(i))
        Q(i+N,i)=2*(x(i+1)-x(i))
        Q(i+N,i+N)=1;
    end

    At=[Q,A';A,zeros(3*N-1,3*N-1)]
    Bt=[zeros(3*N,1);B]
    Zt=At\Bt;
    Z=Zt(1:3*N,1)
endfunction
//===================================================



