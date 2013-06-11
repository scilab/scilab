//iso_crayon3
clear ;
close ;

A=(60/(4180*1000))/(2.824*0.001);//K/T réifié
for fl=1:1:4;

    if fl==1 ;load cray1.dat;tinit=300;end;
    if fl==2 ;load cray2.dat;tinit=22; end;
    if fl==3 ;load cray3.dat;tinit=300;end;
    if fl==4 ;load cray4.dat;tinit=22; end;

    FIT1=va01;FIT2=va07;FIT3=va06;TIT1=va02;TIT3=va04;TIT5=va05;...
    TIT8=va03;jic=va08;//cons=va09;
    tech=1;tf=length(w);
    //w=1:1:tf;
    tfin=tf;

    scf(1);
    a=gca();a.auto_clear="on";
    plot(w,TIT1,"k",w,TIT3,"r",w,TIT5,"b",w,TIT8,"m",w,FIT1/10,"k",...
    w,FIT2/10,"b",w,FIT3/1000,"r");
    xgrid;
    tee=TIT5;tse=TIT3;tsa=TIT8;tep=TIT1;Qech=FIT3-FIT2;

    lambda=mean((tee-tse)./(tsa-tse));
    teem=lambda*tsa+(1-lambda)*tse;


    scf(2);
    a=gca();a.auto_clear="on";
    plot(w,TIT3,"g",w,TIT5,"r",w,TIT8,"b",w,teem,"k",w,Qech/100,"c",w,jic,"m");//,w,cons,'k')
    xgrid;


    scf(3);
    a=gca();a.auto_clear="on";
    plot(w,jic,"g",w,TIT1,"r");
    xgrid;

    tinit=300;
    nivs=TIT1(tinit);nive=jic(tinit);e=jic-nive;sp=TIT1-nivs;sm=sp;
    taupb=10;af=exp(-tech/taupb);bf=1-af;
    tauph=300;ah=exp(-tech/tauph);bh=1-ah;

    //filtrage parallèle
    ef=e;sf=sp;eff=ef;sff=sf;epb=e;spb=sp;

    eph1=epb*0;sph1=spb*0;
    //FILTRAGE parallèle
    //passe bas
    //taub=input('taub (20) ');//%constante de temps du filtre  ( omegcoupure=1/tau  , ne pas oublier..!)
    taub=20;
    afb=exp(-tech/taub) ; bfb=1-afb;//% passage en discret
    //passe haut
    //tauh=input('tauh (1200)');//%constante de temps du filtre  ( omegcoupure=1/tau  , ne pas oublier..!)
    tauh=300;
    afh=exp(-tech/tauh) ; bfh=1-afh;//% passage en discre
    for ii=2:1:tf;
        //  filtre passe bas
        epb(ii)=epb(ii-1)*afb+bfb*e(ii);
        spb(ii)=spb(ii-1)*afb+bfb*sp(ii);

        //filtre passe-haut
        eph1(ii)=eph1(ii-1)*afh+bfh*epb(ii);
        sph1(ii)=sph1(ii-1)*afh+bfh*spb(ii);

        //%----------
        ef(ii) = epb(ii)-eph1(ii);
        spf(ii)= spb(ii)-sph1(ii);
    end

    scf(4);
    a=gca();a.auto_clear="on";
    plot(w,ef,"g",w,spf,"r",w,e,"m",w,sp,"c");
    xgrid;

    e=ef;sp=sf;
    r=11;

    //------------------
    VPP=0;NIV =0;
    niv=.11;//----------------------------------------niveau iso
    for ii=2:1:tf;
        if ii>tinit & ii<tfin;
        VPP=VPP+sp(ii)*sp(ii);NIV=NIV+niv*niv;end;
    end;
    taumin=5;taumax=50;
    Cmin=11111111;
    //-----------------------------------------------------------------------------
    for j =1:1:90;
        taum=taumin+(taumax-taumin)*(j-1)/89;
        am=exp(-tech/taum);bm=1-am;
        sm=sp;t(j)=taum;
        VMM=0;VMP=0;
        for ii=2+r:1:tf;
            sm(ii)=sm(ii-1)*am+bm*e(ii-1-r);
            if ii>tinit & ii<tfin;
            VMM=VMM+sm(ii)*sm(ii); VMP=VMP+sm(ii)*sp(ii);end;
        end;
        Vmm(j)=VMM;Vmp(j)=VMP;
        K(j)=VMP/VMM;
        C=K(j)*K(j)*VMM-2*K(j)*VMP+VPP;
        if C<Cmin; Cmin=C;kopt=K(j); tauopt=t(j);Copt=C;end;
    end;
    for j=1:1:90;
        det = Vmp(j)*Vmp(j)-Vmm(j)*(VPP-Copt-NIV);
        if det>0;k1(j)=(Vmp(j)-sqrt(det))/Vmm(j);k2(j)=(Vmp(j)+sqrt(det))/Vmm(j);
        else; k1(j)=Vmp(j)/Vmm(j);k2(j)=k1(j);end;
    end;
    //-----------------------------------------------------------------------------------

    FIT1m=mean(FIT1);

    // à réifier sur le volume   : V m3/ F m3/s   constante de temps tau= V/f
    for u=1:1:100;tt(u)=(u-1)*60/u;kk(u)=tt(u)*A;end// le rapport K/tau doit être constant,
    tauREIF=2.824*0.001/(FIT1m/(3600*1000));
    kREIF=60/(4180*1000*(FIT1m/(3600*1000)));
    kopt;
    tauopt;
    am=exp(-tech/tauopt);bm=1-am;
    //tracé modelopt;
    for ii=2+r:1:tf;sm(ii)=sm(ii-1)*am+bm*e(ii-1-r)*kopt;end;

    scf(5);
    a=gca();a.auto_clear="on";
    plot(w, sp,"r",w,sm,"k",w,e,"b");
    xgrid;

    Crit=mean(abs(sm(tinit:tf)-sp(tinit:tf)));
    scf(6);
    a=gca();
    //a.auto_clear='on';
    plot(t,k1,"r",t,k2,"r",t,K,"b",tauopt,kopt,"r*",tt,kk,"g",tauREIF,kREIF,"k*");
    //xstring(tauopt,kopt+0.005,'FIT1='+string(floor(FIT1m));
    fit=floor(FIT1m);
    xstring(tauopt,kopt+0.005,"F="+string(fit));
    xgrid ;
    //set(gca(),"data_bounds",[0,0;60,0.3]);;
    xstring(5,0.26,"  GAIN = 51/F");
    xstring(5,0.28,"  TAU = 10450/F");
    xtitle(" ISO - DISTANCE  /GAIN  / CONST TEMPS ");

    a.x_label.text="tau";
    a.y_label.text="K";
    a.x_label.font_size=4;
    a.y_label.font_size=4;
end;
//xstring(taup*1.01, kp*.97,'Process(4,30)' )
//string(tauopt,Kopt*1.025,'tau=' + string(T))
//tring(tauopt,Kopt/1.020,'K=' + string(K))
//nn=//4 // nn = nombre de xstring +1
//a.children(nn-1).font_size=2;// correspond au premier xstring
//a.children(nn-2).font_size=4;// correspond au deuxieme xstring
//a.children(nn-3).font_size=4;// correspond au troisieme xstring
//xtitle(' ISO-DISTANCE   tau / K ')

//------REIFICATION


//tau=10450/F   gain= 51/F    F : litre/heure--------------------modèle interne PFC
