// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2015 - 2016 - INRIA - Serge STEER
//
// This file is hereby licensed under the terms of the GNU GPL v2.0,
// pursuant to article 5.3.4 of the CeCILL v.2.1.
// For more information, see the COPYING file which you should have received
// along with this program.
function [kk,ol_poles,ol_zeros,cl_poles,AsymptotesOrigin,AsymptotesAngles]=rlocus(sys,kmax)
    //Calling sequence analysis
    rhs=argn(2)

    kmax_ind=0; //for error messages
    sltyp=typeof(sys)
    if and(sltyp<>["state-space" "rational" "zpk"]) then
        args=["sys","kmax"];
        ierr=execstr("[kk,ol_poles,ol_zeros,cl_poles,AsymptotesOrigin,AsymptotesAngles]=%"+overloadname(sys)+"_rlocus("+strcat(args(1:rhs),",")+")","errcatch")
        if ierr<>0 then
            error(msprintf(_("%s: Wrong type for input argument #%d: Linear dynamical system expected.\n"),"rlocus",1))
        end
        return
    end
    if or(size(sys)<>1) then
        error(msprintf(gettext("%s: Wrong value for input argument #%d: Single input, single output system expected.\n"),"rlocus",1));
    end

    if sltyp=="rational" then
        den=sys.den;
        num=sys.num;
        ol_zeros=roots(num,"e");
        ol_poles=roots(den,"e");
    elseif sltyp=="state-space" then
        [ol_zeros,ol_poles,k]=ss2zp(sys);
        den=real(poly(ol_poles,"x","r"))
        num=real(poly(ol_zeros,"x","r"))*k
    elseif  sltyp=="zpk" then
        ol_zeros=sys.Z{1};
        ol_poles=sys.P{1}
        k=sys.K
        den=real(poly(ol_poles,"x","r"))
        num=real(poly(ol_zeros,"x","r"))*k
    end


    if rhs<2 then
        kmax=0,
    else
        if type(kmax)<>1|~isreal(kmax)|kmax<0 then
            error(msprintf(gettext("%s: Wrong type for input argument #%d: A positive real expected.\n"),"rlocus",2));
        end
    end

    inproper=degree(num)>degree(den)
    if inproper then
        [num, den]=(den, num);
        [ol_zeros,ol_poles]=(ol_poles,ol_zeros);
    end


    [AsymptotesOrigin,AsymptotesAngles]=rlocusAsymptotes(ol_zeros,ol_poles)
    if size(kmax,"*")>1 then // discretization of k given
        kk=gsort(kmax,"g","i");
        cl_poles=rlocusClpoles(num,den,kk)
    else // Computed discretization
        [cl_poles,kk]=rlocusGainAndClpoles(num,den,ol_zeros,ol_poles,AsymptotesOrigin,AsymptotesAngles,kmax)
    end  //end of locus computation
    if inproper then
        kk(kk==0)=[];
        kk=1 ./ kk,
    end
endfunction

function ord=rlocusOrderRoots(ref,r)
    //ref : reference roots
    //r   : roots to be ordered
    //ord  : orders such as max(abs(r(ord(i))-ref(i))) is minimum
    n=size(r,"*")
    not_yet=1:n
    for k=1:n
        //find the nearest root in ref
        [mn,l]=min(abs(r(k)-ref(not_yet)))
        kmn=not_yet(l);
        not_yet(l)=[];
        ord(kmn)=k;
        if not_yet==[] then break,end
    end
endfunction

function ok=rlocusCheckEnd(RootsAtK,OpenLoopZeros,AsymptotesAngles,AsymptotesOrigin,Threshold)
    // Threshold : relative distance threshold used to decides if current
    //             roots are sufficiently close to open loops zeros and
    //             asymptotes
    // if x0=AsymptotesOrigin(1), y0=AsymptotesOrigin(2) and a=AsymptotesAngles(k)
    // The kth asymptote equation is given by: -sin(a)*y+cos(a)*x -y0*cos(a)+x0*sin(a)=0
    ok=%f
    // Check distance to open loop zeros
    if OpenLoopZeros<>[] then
        w=abs([OpenLoopZeros;RootsAtK]);
        ratio=max(max(w)-min(w))
        //ratio=abs(OpenLoopZeros);ratio(ratio<1d-10)=1;
        nz=size(OpenLoopZeros,"*");
        nr=size(RootsAtK,"*");

        Tz(nz)=%f;
        Rz(nr)=%f;
        for kz=1:nz
            d=abs(OpenLoopZeros(kz)-RootsAtK)./ratio//(kz);
            lok=find(d< Threshold);
            if lok==[] then return,end
            Tz(kz)=%t;
            Rz(lok)=%t;
        end
        if ~and(Tz) then return,end
    else
        Rz=%f(ones(RootsAtK));
        ratio=max(abs(RootsAtK))
    end

    // Check distance with asymptotes
    na=size(AsymptotesAngles,"*")
    if na==0 then ok=%t,return;end
    Ta(na)=%f;

    //consider only current roots which did not match an open loop zero
    R=RootsAtK(~Rz) - AsymptotesOrigin
    //ratio=abs(R);ratio(ratio<1d-10)=1;
    a=exp(%i*AsymptotesAngles);
    d=abs(imag(R*a))/ratio;// d(i,j) =distance from roots(i) to the asymptotes(j)
    //lambda=real(R*conj(a))   //factors along asymptotes direction vectors
    [i,j]=find(d<Threshold)//&lambda>0)
    ok=size(unique(j),"*")>=na
endfunction

function cl_poles=rlocusClpoles(num,den,kk)
    nk=size(kk,"*")
    cl_poles=zeros(degree(den),nk);
    cl_poles(:,1)=roots(den+kk(1)*num,"e");
    for i=2:nk
        r=roots(den+kk(i)*num,"e");
        // look for order that minimize the distance
        ord=rlocusOrderRoots(cl_poles(:,i-1),r);
        cl_poles(:,i)=r(ord);
    end
endfunction

function [cl_poles,kk]=rlocusGainAndClpoles(num,den,ol_zeros,ol_poles,AsymptotesOrigin,AsymptotesAngles,kmax)
    smax=0.02;smin=smax/3;//boundaries on relative closed roots distances
    //for 2 consecutive gain values.
    nptmax=5000 //nbre maxi de pt de discretisation en k

    //Find  gains  where locus branches intersect
    klim=gsort(krac2(rlist(num,den,"c")),"g","i");
    nlim=size(klim,"*");
    dmax=max(degree(num),degree(den))
    nrm=max([norm(ol_poles,1),norm(ol_zeros,1)]) //normalization ratio

    ord=1:size(ol_poles,"*");
    nr=max(size(ol_poles,"*"),size(ol_zeros,"*"))
    k=0;//initialize current gain (open loop)
    kk=k; //initialize  gain discretization array
    cl_poles=ol_poles; //initialize closed loop poles array
    k_step=0.99; //initialize gain step
    ilim=1;

    if degree(num)==degree(den) then
        ksing=abs(coeff(den,degree(den))/coeff(num,degree(num)))
    else
        ksing=%inf
    end

    while %t then
        k=k+k_step;
        if (kmax>0&k>kmax) then break;end

        if k>0.95*ksing then

            //jump over the singularity
            k=1.05*ksing
            cl_poles(:,$+1)=%nan;
            cl_poles(:,$+1)=roots(den+k*num,"e")
            kk=[kk %nan k]
            k=k+k_step;
            ksing=%inf
        end
        point=%f;
        r=roots(den+k*num,"e");
        r=r(ord);
        mx=max(abs(cl_poles(:,$)))
        if mx>0 then
            dist=max(abs(cl_poles(:,$)-r))./mx

            if dist > smax //Try to find a best closed loop poles order
                ord1=rlocusOrderRoots(cl_poles(:,$),r)
                r=r(ord1)
                ord=ord(ord1)
                // recompute the distance
                //dist=max(abs(cl_poles(:,$)-r))/nrm
                dist=max(abs(cl_poles(:,$)-r))./max(abs(cl_poles(:,$)))
            end
        else
            dist=smax
        end
        if dist <= smax then //step small enough
            if dist<smin then //Too small step, ignore point and enlarge step size
                k=k-k_step
                k_step=2*k_step;
                if k_step>1e5 then break;end
            else //step ok
                point=%t
                if nlim>0&(ilim<=nlim&k>=klim(ilim)&klim(ilim)<>0) then
                    //rlocus intersection detected, add a point exactly at the intersection
                    k=klim(ilim),
                    ilim=ilim+1
                    r=roots(den+k*num,"e");
                    ord1=rlocusOrderRoots(cl_poles(:,$),r)
                    r=r(ord1)
                    ord=ord(ord1)
                end
                cl_poles=[cl_poles,r]; //store current closed loop poles
                kk=[kk,k]  //store current gain

                //check if k is large enough
                if kmax==0 then
                    if rlocusCheckEnd(r,ol_zeros,AsymptotesAngles,AsymptotesOrigin,smax) then break,end
                elseif k>kmax then
                    break
                end
                if size(kk,"*")>nptmax then
                    warning(msprintf(_("%s: Curve truncated to the first %d discretization points.\n"),"rlocus",nptmax))
                    break
                end
            end
        else //Too big step, /reduce the step size
            k=k-k_step,k_step=k_step/2.5
        end

    end //end of discretization loop

endfunction

function  [AsymptotesOrigin,AsymptotesAngles]=rlocusAsymptotes(ol_zeros,ol_poles)
    //Find the asymptotes equations
    //see http://en.wikibooks.org/wiki/Control_Systems/Root_Locus#Root_Locus_Equations
    na=size(ol_poles,"*")-size(ol_zeros,"*"); //number of asymptotes
    if  na>0 then
        AsymptotesOrigin=(sum(ol_poles)-sum(ol_zeros))/na; //real axis with asymptotes intersection point
        AsymptotesAngles=%pi/na*(2*(0:na-1)+1); //slope of the asymptotes
        //the kth asymptote parametric equation in complex plane is
        //s=AsymptotesOrigin+t*exp(%i*AsymptotesAngles(k)) for t>0
    else
        AsymptotesAngles=[]
        AsymptotesOrigin=%nan
    end
endfunction
