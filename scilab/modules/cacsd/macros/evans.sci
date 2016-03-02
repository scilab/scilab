// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2010 - INRIA - Serge STEER
// Copyright (C) 2012 - 2016 - Scilab Enterprises
//
// This file is hereby licensed under the terms of the GNU GPL v2.0,
// pursuant to article 5.3.4 of the CeCILL v.2.1.
// This file was originally licensed under the terms of the CeCILL v2.1,
// and continues to be available under such terms.
// For more information, see the COPYING file which you should have received
// along with this program.
function evans(n,d,kmax)
    // Seuil maxi et mini (relatifs) de discretisation en espace
    // Copyright INRIA

    smax=0.002;smin=smax/3;
    nptmax=2000 //nbre maxi de pt de discretisation en k

    //Check syntax

    [lhs,rhs]=argn(0)

    if rhs <= 0 then   // demonstration
        n=real(poly([0.1-%i 0.1+%i,-10],"s"));
        d=real(poly([-1 -2 -%i %i],"s"));
        evans(n,d,80);
        return
    end

    select typeof(n)
    case "polynomial"  then
        if rhs==2 then kmax=0,end
    case "rational" then
        if rhs==2 then kmax=d,else kmax=0,end
        [n,d]=n(2:3)
    case "state-space" then
        if rhs==2 then kmax=d,else kmax=0,end
        n=ss2tf(n);
        [n,d]=n(2:3);n=clean(n);d=clean(d);
    else
        error(msprintf(_("%s: Wrong type for input argument #%d: A linear dynamical system or a polynomial expected.\n"),"evans",1));
    end
    if prod(size(n))<>1 then
        error(msprintf(_("%s: Wrong value for input argument #%d: Single input, single output system expected.\n"),"evans",1));
    end
    if degree(n)==0&degree(d)==0 then
        error(msprintf(_("%s: The given system has no poles and no zeros.\n"),"evans"));
    end

    if kmax<=0 then
        nm=min([degree(n),degree(d)])
        fact=norm(coeff(d),2)/norm(coeff(n),2)
        kmax=round(500*fact),
    end
    //
    //Compute the discretization for "k" and the associated roots
    nroots=roots(n);racines=roots(d);
    if nroots==[] then
        nrm=max([norm(racines,1),norm(roots(d+kmax*n),1)])
    else
        nrm=max([norm(racines,1),norm(nroots,1),norm(roots(d+kmax*n),1)])
    end
    md=degree(d)
    //
    ord=1:md;kk=0;nr=1;k=0;pas=0.99;fin="no";
    klim=gsort(krac2(rlist(n,d,"c")),"g","i")
    ilim=1
    while fin=="no" then
        k=k+pas
        r=roots(d+k*n);r=r(ord)
        dist=max(abs(racines(:,nr)-r))/nrm
        //
        point=%f

        if dist <smax then //pas correct
            if k-pas<klim(ilim)& k>klim(ilim) then,
                k=klim(ilim);
                r=roots(d+k*n);r=r(ord)
            end
            if k>klim(ilim) then ilim=min(ilim+1,size(klim,"*"));end
            point=%t
        else //Too big step or incorrect root order
            // look for a root order that minimize the distance
            ix=1:md
            ord1=[]
            for ky=1:md
                yy=r(ky)
                mn=10*dist*nrm
                for kx=1:md
                    if ix(kx)>0 then
                        if  abs(yy-racines(kx,nr)) < mn then
                            mn=abs(yy-racines(kx,nr))
                            kmn=kx
                        end
                    end
                end
                ix(kmn)=0
                ord1=[ord1 kmn]
            end
            r(ord1)=r
            dist=max(abs(racines(:,nr)-r))/nrm
            if dist <smax then
                point=%t,
                ord(ord1)=ord
            else
                k=k-pas,pas=pas/2.5
            end
        end
        if dist<smin then
            //KToo small step
            pas=2*pas;
        end
        if point then
            racines=[racines,r];kk=[kk,k];nr=nr+1
            if k>kmax then fin="kmax",end
            if nr>nptmax then fin="nptmax",end
        end
    end
    //draw the axis
    x1 =[nroots;matrix(racines,md*nr,1)];
    xmin=min(real(x1));xmax=max(real(x1))
    ymin=min(imag(x1));ymax=max(imag(x1))
    dx=abs(xmax-xmin)*0.05
    dy=abs(ymax-ymin)*0.05
    if dx<1d-10, dx=0.01,end
    if dy<1d-10, dy=0.01,end
    legs=[],lstyle=[];lhandle=[]
    rect=[xmin-dx;ymin-dy;xmax+dx;ymax+dy];
    f=gcf();
    immediate_drawing= f.immediate_drawing;
    f.immediate_drawing = "off";
    a=gca();
    if a.children==[]
        a.data_bounds=[rect(1) rect(2);rect(3) rect(4)];
        a.axes_visible="on";
        a.title.text=_("Evans root locus");
        a.x_label.text=_("Real axis");
        a.y_label.text=_("Imaginary axis");
        axes.clip_state = "clipgrf";
    else //enlarge the boundaries
        a.data_bounds=[min(a.data_bounds(1,:),[rect(1) rect(2)]);
        max(a.data_bounds(2,:),[rect(3) rect(4)])];

    end
    if nroots<>[] then
        xpoly(real(nroots),imag(nroots))
        e=gce();e.line_mode="off";e.mark_mode="on";
        e.mark_size_unit="point";e.mark_size=7;e.mark_style=5;
        legs=[legs; _("open loop zeroes")]
        lhandle=[lhandle; e];
    end
    if racines<>[] then
        xpoly(real(racines(:,1)),imag(racines(:,1)))
        e=gce();e.line_mode="off";e.mark_mode="on";
        e.mark_size_unit="point";e.mark_size=7;e.mark_style=2;
        legs=[legs;_("open loop poles")]
        lhandle=[lhandle; e];
    end
    dx=max(abs(xmax-xmin),abs(ymax-ymin));
    //plot the zeros locations


    //computes and draw the asymptotic lines
    m=degree(n);q=md-m
    if q>0 then
        la=0:q-1;
        so=(sum(racines(:,1))-sum(nroots))/q
        i1=real(so);i2=imag(so);
        if prod(size(la))<>1 then
            ang1=%pi/q*(ones(la)+2*la)
            x1=dx*cos(ang1),y1=dx*sin(ang1)
        else
            x1=0,y1=0,
        end
        if md==2,
            if coeff(d,md)<0 then
                x1=0*ones(2),y1=0*ones(2)
            end,
        end;
        if max(k)>0 then
            xpoly(i1,i2);
            e=gce();
            legs=[legs;_("asymptotic directions")]
            lhandle=[lhandle; e];

            a.clip_state = "clipgrf";
            for i=1:q,xsegs([i1,x1(i)+i1],[i2,y1(i)+i2]),end,
            //      a.clip_state = "off";
        end
    end;

    [n1,n2]=size(racines);

    // assign the colors for each root locus
    cmap=f.color_map;cols=1:size(cmap,1);
    if a.background==-2 then
        cols(and(cmap==1,2))=[]; //remove white
    elseif a.background==-1 then
        cols(and(cmap==0,2))=[]; //remove black
    else
        cols(a.background)=[];
    end
    cols=cols(modulo(0:n1-1,size(cols,"*"))+1);

    //draw the root locus
    xpolys(real(racines)',imag(racines)',cols)
    //set info for datatips
    E=gce();

    for k=1:size(E.children,"*")
        E.children(k).display_function = "formatEvansTip";
        E.children(k).display_function_data = kk;
    end
    c=captions(lhandle,legs($:-1:1),"in_upper_right")
    c.background=a.background;

    f.immediate_drawing = immediate_drawing;

    if fin=="nptmax" then
        warning(msprintf(gettext("%s: Curve truncated to the first %d discretization points.\n"),"evans",nptmax))
    end
endfunction

function str=formatEvansTip(curve)
    //this function is called by the datatip mechanism to format the tip
    //string for the evans root loci curves
    ud = curve.parent.display_function_data;
    pt = curve.data(1:2);
    [d,ptp,i,c]=orthProj(curve.parent.data, pt);
    K=ud(i)+(ud(i+1)-ud(i))*c;
    str=msprintf("r: %.4g %+.4g i\nK: %.4g", pt,K);
endfunction
