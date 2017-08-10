// ============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2007-2008 - Sylvestre LEDRU
//
//  This file is distributed under the same license as the Scilab package.
// ============================================================================

// <-- CLI SHELL MODE -->

// Check value provided here:
// http://math.nist.gov/MatrixMarket/data/SPARSKIT/tokamak/utm300.html
warning("off");

[A, dsc]=ReadHBSparse(SCI+"/modules/umfpack/demos/utm300.rua");
if dsc <> "utm300" then pause, end

[nl,nc] = size(A)
if nl <> 300 then pause, end
if nc <> 300 then pause, end
if nnz(A) <> 3155 then pause, end

// Diagonal
if nnz(diag(A)) <> 300 then pause, end

// A-A'
if nnz(A-A') <> 4382 then pause, end

// Heaviest diagonals
offsets = [ 0 -50 50 1 -1 -5 5 2 -2 -4 ];
nonzerosExpected = [ 300 190 186 180 166 159 158 150 134 113 ];

for j=1:length(offsets)
    if nnz(diag(A, offsets(j))) <> nonzerosExpected(j) then pause, end
end

// TODO add the rest of information

[ij]=spget(A);
e=ij(:,1)-ij(:,2);

// Bandwidths average |i-j|
m=ceil(mean(abs(e)));
if m <> 21 then pause, end

// Bandwidths std.dev.
d=ceil(stdev(abs(e)));
if d <> 23 then pause, end

lastinrow=[find(diff(ij(:,1))>0) size(ij,1)];
rowlength=diff([0 lastinrow]);

// Row Data
[longest,indlongest]=max(rowlength);
if longest <> 33 then pause, end
if indlongest <> 116 then pause, end

[shortest,indshortest]=min(rowlength);
if shortest <> 1 then pause, end
if indshortest <> 3 then pause, end

// Average nonzeros per row
m=ceil(mean(rowlength));
if m <> 11 then pause, end

d=round(stdev(rowlength)*10)/10;
if d <> 7.7 then pause, end

// Column Data
ji=gsort(ij(:,[2 1]),"lr","i");
lastincol=[find(diff(ji(:,1))>0) size(ji,1)];
collength=diff([0 lastincol]);

[longest,indlongest]=max(collength);
if longest <> 22 then pause, end
if indlongest <> 59 then pause, end

[shortest,indshortest]=min(collength);
if shortest <> 1 then pause, end
if indshortest <> 16 then pause, end

m=ceil(mean(collength));
if m <> 11 then pause, end

d=round(stdev(collength)*10)/10;
if d <> 7.1 then pause, end

// Profile Storage
lastinrow=[find(diff(ij(:,1))>0) size(ij,1)];
rowbounds= [ij([1 lastinrow(1:$-1)+1],2) ij(lastinrow,2)];
rowindex=ij(lastinrow,1);
rowbandwidth=rowbounds-rowindex*ones(1,2);
lowband=[-rowbandwidth(find(rowbandwidth(:,1)<=0),1)
-rowbandwidth(find(rowbandwidth(:,2)<=0),2)];
uppband=[rowbandwidth(find(rowbandwidth(:,1)>=0),1)
rowbandwidth(find(rowbandwidth(:,2)>=0),2)];

// lower bandwidth => max
minlow=max(lowband);
if minlow <> 74 then pause, end

// lower bandwidth => min
maxlow=min(lowband);
if maxlow <> 0 then pause, end

// upper bandwidth => max
maxupp=max(uppband);
if maxupp <> 66 then pause, end

// upper bandwidth => min
minupp=min(uppband);
if minupp <> 0 then pause, end

