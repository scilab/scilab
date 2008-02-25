function cmpresults(testName,nbCols,idxCols)

disp(" -= " + testName + " =-")
disp("Reading results from Scilab 4...")
A  = mgetl(testName + ".out.ref");
AA = zeros(size(A,1),nbCols);
for k = 1:size(A,1)
  AA(k,:) = msscanf(A(k),mtlb_repmat('%f ',[1 nbCols]));
end

disp("Reading results from Scilab 5...")
B  = mgetl(testName + ".out");
BB = zeros(size(B,1),nbCols);
for k = 1:size(B,1)
  BB(k,:) = msscanf(B(k),mtlb_repmat('%f ',[1 nbCols]));
end

disp("Plotting diff = f(t)...")
t = AA(:,1);
plot(t,BB(:,idxCols)-AA(:,idxCols))

