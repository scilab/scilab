//----------------------------------------------------------------------------
// marge-gain.sce
// Auteur : Jacques Richalet
// 2007
// marge de  gain pour différents retard du processus
//----------------------------------------------------------------------------
clear("all") ;
close() ;
xdel(winsid());
mode(-1);
//----------------------------------------------------------------------------
tech=.1;
tau= 80;
a=exp(-tech/tau);
b=1-a;//processus nominal= moèle interne
trbo=tau*3;//boucle ouverte

for z=1:1:7;
    z= 7+z;
    r=z/tech;//retard modèle= process nominal
    //trbo
    lh=1-exp(-tech*3/trbo);
    q=b/lh;

    omegpomp=%pi/(2*tech*r);
    Tp=2*%pi/omegpomp;
    MGtrbo= tau*omegpomp*q;//marge de gain

    //marge de gain pour différents trbf
    for j= 1:1:100; // for (2)
        trbf(j)=2+(j-1)*1.1*trbo/99;
        lh=1-exp(-tech*3/trbf(j));
        qq=b/lh;
        omegpomp=%pi/(2*tech*r);
        MG(j)= tau*omegpomp*qq;
    end // fin for (2)

    scf(1);
    a=gca();
    a.auto_clear="off";
    plot( trbf, MG,"r",trbo,MGtrbo,"b*", trbo,0,"b*");
    xgrid();
    xstring( trbf(j), MG(j),"R= " +string(z));
    xstring (10,17, "Marge Gain");
    xstring( 220, 0.5, "TRBO");
    xstring( 270, 0.5, "TRBF");
    xstring(25,15, "RETARD MODELE=7 ");
    xtitle( " MARGE DE GAIN / RETARD PROCESSUS  Rsec / Tau=80sec");
end // fin for (1)
//----------------------------------------------------------------------------

