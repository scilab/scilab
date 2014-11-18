function MPIHelloWorldDouble()
    ////////////
    // PARALLEL / initialization, include MPI_Init time in measurement
    ////////////
    MPI_Init();			// should have lambooted first
    rnk =	MPI_Comm_rank();	// let it abort if it fails
    sizeNodes =	MPI_Comm_size();

    SLV = rnk;			// handy shortcuts, master is rank 0
    Master = ~ SLV;			// slaves are all other

    if Master

        disp("We have "+string(sizeNodes) + " processors")
        for slaveId = 1:sizeNodes-1
            MPI_Send([23,42;68,62],slaveId)
        end

        for slaveId = 1:sizeNodes-1
            tag=0
            valueBack=MPI_Recv(slaveId, tag)
            disp("<=> VALUE BACK <=> "+valueBack+ " from " +string(slaveId))

        end
    else
        // slave
        disp("Processor "+string(rnk))
        rankSource=0
        tag=0
        value=MPI_Recv(rankSource, tag)
        value="(Modified by the slave) "+value
        MPI_Send(value,0)

    end

    MPI_Finalize()

endfunction
