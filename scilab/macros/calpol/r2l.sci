function [Es] = r2l(Fs)
//Kwakernaak's r2l : a left kernel to Fs

// Copyright INRIA
[F,degF]=pol2mat(Fs);
[rF,cF] = size(Fs);
if cF > rF then
   error('r2l: Input has more columns than rows');
end
[roF,coF] = size(F);
to = 1e-6;                 // tolerance


// INITIALIZE

rs = coF;
SS = [];
rowsS = 0;
normF = norm(F,'inf');
j = 0;         // row number of S
jj = 0;        // row number within block
k = 0;         // block number

// COMPUTATION OF E

E = [];
rows = (degF*cF+1)*rF;       // upper bound number of rows of S
rowsE = 0;
rE = rF-cF; cE = rF;
sel = 1:rows;
i = 2;

while rowsE < rE

   // ADD ROWS TO S

   while j < sel(1,i)
      j = j+1; jj = jj+1;
      if jj > rF then
         jj = jj-rF; k = k+1;
         rs = rs + cF;
         SS = [SS 0*ones(rowsS,cF)];
      end
      s = 0*ones(1,rs);
      s(1,k*cF+1:k*cF+coF) = F(jj,:);
      SS = [SS; s];
      rowsS = rowsS+1;
   end

   // FIND THE NEXT PRIMARY DEPENDENT ROW OF S

   b = SS(sel(1,i),:); BB = SS(sel(1,1:i-1),:);
   c = b/BB;
   err = b-c*BB;
   if norm(err,2) <= to*normF then

      // FORM THE NEW MATRIX E

      c = [-c 1]; c = c/norm(c,2);      // normalize c
      [rc,cc] = size(c);
      C1 = 0*ones(1:cc);
      C1(1,sel(1:i)) = c;
      [rrE,ccE] = size(E); [rC,cC] = size(C1);
      czeros = cC-ccE;
      E = [E 0*ones(rrE,czeros); C1];
      rowsE = rowsE+1;

      // DELETE THE DEPENDENT ROWS OF S

      ii = i; idel = sel(1,i);
      while ii <= rows
         if sel(1,ii) == idel then
            sel = [sel(1,1:ii-1) sel(1,ii+1:rows)];
            rows = rows-1;
            idel = idel+rF;
         end
         ii = ii+1;
      end

      i = i-1;
   end
   i = i+1;
end


// SUPPLEMENT E WITH ZERO COLUMNS

sizeE = size(E)
degE = round(sizeE(1,2)/cE-1)
supE = (degE+1)*cE-sizeE(1,2);
E = [E 0*ones(rE,supE)];
Es=clean(mat2pol(E,degE));


endfunction
function [As]=mat2pol(A,degA)
[na,ma]=size(a);long=ma/(degA+1);
nb=degA+1;debut=1;fin=long;As=0*ones(na,fin);
s=poly(0,'s');
for k=1:nb ,As=As+s^(k-1)*A(:,debut:fin),debut=fin+1,fin=fin+long,end

endfunction
function [F,degF]=pol2mat(Fs)
degf=maxi(degree(Fs));
F=coeff(Fs);
endfunction
