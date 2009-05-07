num = mpi_comm_rank()

N =  mpi_comm_size()

src  = modulo(num+N-1, N)

dest = modulo(num+1, N)


if ((modulo(num,2) == 0)) then 
	mpi_send(num,dest,1001)
	res = mpi_recv(src,1001)
else 
	res = mpi_recv(src,1001)
	mpi_send(num,dest,1001)
end
