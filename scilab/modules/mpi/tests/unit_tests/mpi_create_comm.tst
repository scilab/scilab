// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2016 - Scilab Enterprises - Cedric Delamarre
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================
//
// <-- MPI TEST -->
//

MPI_Init();

worldRnk  = MPI_Comm_rank();
worldSize = MPI_Comm_size();

assert_checkequal(worldSize, 4);

evenNodes = 0:2:(worldSize-1);
evenComm  = MPI_Create_comm(evenNodes);
oddNodes  = 1:2:(worldSize-1);
oddComm   = MPI_Create_comm(oddNodes);

// Comm rank / size
evenRnk  = MPI_Comm_rank(comm=evenComm);
oddRnk   = MPI_Comm_rank(comm=oddComm);
evenSize = MPI_Comm_size(comm=evenComm);
oddSize  = MPI_Comm_size(comm=oddComm);

// I am an even node
isEven = evenRnk <> -1;
// I am an odd node
isOdd  = oddRnk <> -1;
// I am an even master
isEvenMaster = evenRnk == 0;
// I am an odd master
isOddMaster  = oddRnk == 0;

if isEven then
    assert_checkequal(evenSize, 2);
    assert_checkequal(worldRnk/2,  evenRnk);
else
    assert_checkequal(oddSize,  2);
    assert_checkequal(floor(worldRnk/2),  oddRnk);
end


// Bcast
a=0;
if isEvenMaster then
    a=42;
end
b=MPI_Bcast(a, 0, comm=evenComm);

if isEven then
    assert_checkequal(b, 42);
else
    assert_checkequal(b, []);
end


// Send / Recv
if isOddMaster then
    for i=1:(oddSize-1)
        MPI_Send(10+i, i, comm=oddComm);
    end
else
    val=MPI_Recv(0, 0, comm=oddComm);
    if isOdd then
        assert_checkequal(val, 10+oddRnk);
    else
        assert_checkequal(val, []);
    end
end

// Isend / Irecv
req = 26;
if isEvenMaster then
    for i=1:(evenSize-1)
        MPI_Isend(10+i, i, req, comm=evenComm);
    end
else
    MPI_Irecv(0, 0, req, comm=evenComm);
    val = MPI_Wait(req, comm=evenComm);
    if isEven then
        assert_checkequal(val, 10+evenRnk);
    else
        assert_checkequal(val, []);
    end
end

disp("MPI_Create_comm test DONE.")

MPI_Finalize();
exit()
