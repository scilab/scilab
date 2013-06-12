function [value,denom_com]=fixedpointgcd(v)
    // This function computes the PGCD of a double vector.
    x=log10(v);
    f=round((min(x)+max(x))/2);
    v=v./10^(f);
    [N,D]=rat(v,1d-9);
    denom_com=lcm(uint32(D));
    N=uint32(N)*denom_com./uint32(D);
    value=gcd(N);
    if f>0 then value=value*10^f;
    else denom_com=double(denom_com)*10^(-f);
    end
endfunction
