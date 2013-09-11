MPI_Init();
rnk = MPI_Comm_rank()
sizeNodes =	MPI_Comm_size();
SLV = rnk;
Master = ~ SLV;

if Master then
    disp("Master");
    MPI_Send(rand(10,10), 1);
else
    val = MPI_Recv(0, 0);
    disp(val, rnk)
end

MPI_Finalize();
