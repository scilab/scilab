function [cells,fact,zzeros,zpoles]=eqiir(ftype,approx,om,deltap,deltas)
//[cells,fact,zzeros,zpoles]=eqiir(ftype,approx,om,deltap,deltas)
//Design of iir filter :interface with eqiir (syredi)
// ftype  :filter type ('lp','hp','sb','bp')
// approx :design approximation ('butt','cheb1','cheb2','ellip')
// om     :4-vector of cutoff frequencies (in radians)
//        :      om=<om1,om2,om3,om4>
//        :      0 <= om1 <= om2 <= om3 <= om4 <= pi
//        :When ftype=3 or 4, om3 and om4 are not used
//        :and may be set to 0.
// deltap :ripple in the passband.  0<= deltap <=1.
// deltas :ripple in the stopband.  0<= deltas <=1.
//Outputs :
// cells  :realization of the filter as second order cells
// fact   :normalization constant
// zzeros :zeros in the z-domain
// zpoles :poles in the z-domain
//The filter obtained is h(z)=fact*product of the elements of
//cells.  That is
//
//     hz=fact*prod(cells(2))./prod(cells(3))
//
//!
// Copyright INRIA
select part(approx,1);
   case 'b'
        iapro=1
   case 'e'
        iapro=4
   case 'c'
        last=part(approx,length(approx));
        if last=='1' then iapro=2,end;
        if last=='2' then iapro=3,end
   else write(%io(2),'iir : unknown filter approximation');
        return;
end
select ftype;
   case 'lp'
        ityp=1
   case 'hp'
        ityp=2
   case 'bp'
        ityp=3
   case 'sb'
        ityp=4
   else write(%io(2),'iir : wrong first input parameter');
     return;
   end
if maxi(size(om))==2 then
      om=matrix([matrix(om,1,2),0,0],1,4),
end
[fact,b2,b1,b0,c1,c0,zzeros,zpoles]=...
                           syredi(ityp,iapro,om,deltap,deltas);
nb=maxi(size(b0));
coeffs=[b0;b1;b2;c0;c1];
coeffs=coeffs(:,1:nb);
coeffs=[coeffs;ones(1,nb)];
cells=[];
for col=coeffs,
  nf=col(1:3);nd=col(4:6);
  [n1,d1]=simp(poly(nf,'z','c'),poly(nd,'z','c'));
    cells=[cells,syslin([],n1,d1)];
end
//crapaud...
if iapro==1 then
zzeros=[];
[k,j]=size(cells);
w=cells(2);
for k=w;
    zzeros=[zzeros,roots(k)'];
end
end
endfunction
