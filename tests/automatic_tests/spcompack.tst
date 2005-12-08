getf SCI/util/testexamples.sci
reinit_for_test()
%U=mopen('SCI/tests/automatic_tests/spcompack_data.ref','rb');
// A is the sparse matrix:
A = [1,0,0,0,0,0,0;
  0,1,0,0,0,0,0;
  0,0,1,0,0,0,0;
  0,0,1,1,0,0,0;
  0,0,1,1,1,0,0;
  0,0,1,1,0,1,0;
  0,0,1,1,0,1,1];
A = sparse(A);
//For this matrix, the standard adjacency representation is given by:
xadj = [1,2,3,8,12,13,15,16];
adjncy = [1,2,3,4,5,6,7,4,5,6,7,5,6,7,7];
//(see sp2adj).
// increments in vector xadj give the number of non zero entries in each column
// ie there is 2-1=1 entry in the column 1
//    there is 3-2=1 entry in the column 2
//    there are 8-3=5 entries in the column 3
//              12-8=4                      4
//etc
//The row index of these entries is given by the adjncy vector
// for instance,
// adjncy (3:7)=adjncy(xadj(3):xadj(4)-1)=[3,4,5,6,7]
// says that the 5=xadj(4)-xadj(3) entries in column 3 have row
// indices 3,4,5,6,7.
//In the compact representation, the repeated sequences in adjncy
//are eliminated.
//Here in adjncy the sequences 4,5,6,7  and 7 are eliminated.
//The standard structure (xadj,adjncy) takes the compressed form (lindx,xlindx)
lindx = [1,2,3,4,5,6,7,5,6,7];
xlindx = [1,2,3,8,9,11];
//(Columns 4 and 7 of A are eliminated).
//A can be reconstructed from (xadj,xlindx,lindx).
[xadj,adjncy,anz] = sp2adj(A);
%ans = adjncy - spcompack(xadj, xlindx, lindx);
if load_ref('%ans') then   pause,end,

xdel_run(winsid());

mclose(%U);
