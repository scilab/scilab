// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C)  2016 - INRIA - Serge Steer
//
// This file is licensed under the terms of the GNU GPL v2.0,
// pursuant to article 5.3.4 of the CeCILL v.2.1.
// For more information, see the COPYING file which you should have received
// along with this program.
function %zpk_p(T)
    [ny,nu]=size(T)
    M=cell(ny,nu);
    L=zeros(ny,nu);
    for l=1:ny
        for k=1:nu
            [z,mz]=nearly_multiples(T.Z{l,k})
            [p,mp]=nearly_multiples(T.P{l,k})
            g=T.K(l,k)
            if T.dt=="c" then v="s";else v="z";end
            m=[];
            if z<>[] then
                N=[];
                kr=find(abs(imag(z))<1e-6)
                zr=real(z(kr))
                m=[m;mz(kr)]
                if zr<>[] then
                    if zr(1)==0 then
                        N=[N;v],i=2
                    else
                        i=1
                    end
                    if i<=size(zr,"*") then
                        N=[N;"("+v+msprintf("%+g\n",-zr(i:$))+")"];
                    end
                end
                ki=find(imag(z)>=1e-6)
                zi=z(ki)
                m=[m;mz(ki)]
                if zi<>[] then
                    N=[N;
                    msprintf("(%s^2%+g%s%+g)\n",v+emptystr(zi),-2*real(zi),v+emptystr(zi),abs(zi)^2)];
                end
                i=find(m>1)
                if i<>[] then
                    N(i)=N(i)+"^"+string(m(i))
                end
                N=strcat(N," ")
            else
                N="1"
            end
            m=[]
            if p<>[] then
                kr=find(abs(imag(p))<1e-6)
                pr=real(p(kr))
                m=[m;mp(kr)]
                D=[]
                if pr<>[] then
                    if pr(1)==0 then
                        D=[D;v];i=2
                    else
                        i=1
                    end
                    if i<=size(pr,"*") then
                        D=[D;"("+v+msprintf("%+g\n",-pr(i:$))+")"];
                    end
                end
                ki=find(imag(p)>=1e-6)
                pi=p(ki)
                m=[m;mp(ki)]
                if pi<>[] then
                    D=[D;
                    msprintf("(%s^2%+g%s%+g)\n",v+emptystr(pi),-2*real(pi),v+emptystr(pi),abs(pi).^2)]
                end
                i=find(m>1)
                if i<>[] then
                    D(i)=D(i)+"^"+string(m(i))
                end
                D=strcat(D," ")
            else
                D="1"
            end
            n=length(N);
            d=length(D);
            K=msprintf("%g ",g)
            if K=="1 " then
                K=""
            elseif K=="-1 " then
                K="- "
            end
            lk=length(K)
            if d>=n
                s=part(" ",ones(1,floor(d-n+1)/2));
                N=s+N+s;
                f=part("-",ones(1,d));
            else
                s=part(" ",ones(1,floor(n-d+1)/2));
                D=s+D+s;
                f=part("-",ones(1,n));
            end
            s=part(" ",ones(1,lk))
            mlk=[s+N;K+f;s+D]
            llk=max(length(mlk))+1
            M{l,k}=mlk;
            L(l,k)=llk;
        end
    end
    Lc=max(L,1)+2
    for l=1:ny
        T=""
        for k=1:nu
            w=Lc(k)-L(l,k)
            ws2=floor(w/2)
            T=T+part(" ",1:ws2)+M{l,k}+part("",1:w-ws2)
        end
        mprintf("  %s\n",T)
        mprintf("\n")
    end
endfunction
