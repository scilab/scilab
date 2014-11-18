function simple()
    disp("simple");
    MPI_Init();
    disp(MPI_Comm_rank());	// let it abort if it fails
    disp(MPI_Comm_size());
    disp("simple 2");
    MPI_Finalize()

endfunction