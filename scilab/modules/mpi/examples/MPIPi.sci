function MPIPi(N,mod)
    // Pi_seq: Compute PI (sequential) by num.integr. of arctan(x) in [0..1]
    //
    //	Pi_seq [ (N) ]
    //
    //  N	[1E7]	#subdivisions of the [0, 1] interval
    // mod	['s']	communication modality:  (s)end (r)educe
    //  printed results struct contains
    //	pi	estimated pi value
    //	err	error
    //	time	from argument xmit to pi computed
    //
    // Examples:
    //
    //  octave:1> Pi_seq
    //  results =
    //  {
    //    pi =  3.1416
    //    err = -6.4837e-14
    //    time =  2.7761
    //  }
    //

    ////////////////////
    // ArgChk //
    ////////////////////
    if argn(2)<1,	N=1E7;	end
    if argn(2)<2,  mod="s";	end
    if argn(2)>2,	error("usage MPIPi(N,mod)"); end
    flag=0;
    [%H,%ierr] = evstr(string(N));
    flag=flag | size(N,"*")<>1 | %ierr<>0;
    flag=flag  |   fix(N)~=N   |           N<1;
    if flag,	error("usage MPIPi( <int> N>0, <char> mod==''s|r'' )"), end

    ////////////////////////////////////
    // Results struct //
    ////////////////////////////////////
    results.pi  =0;
    results.err =0;
    results.time=0;

    ////////////
    // PARALLEL / initialization, include MPI_Init time in measurement
    ////////////
    //  T=clock; /
    ////////////
    MPI_Init();			// should have lambooted first
    rnk =	MPI_Comm_rank();	// let it abort if it fails
    siz =	MPI_Comm_size();

    SLV = rnk;			// handy shortcuts, master is rank 0
    MST = ~ SLV;			// slaves are all other

    if MST
        disp("Master here")
    else
        disp("Slave here")
    end

    ////////////
    // PARALLEL / computation (depends on rank/size)
    ////////////			// vectorized code, equivalent to
    width=1/N; lsum=0;		// for i=rnk:siz:N-1
    i=rnk:siz:N-1;		//   x=(i+0.5)*width;
    x=(i+0.5)*width;		//   lsum=lsum+4/(1+x^2);
    lsum=sum(4 ./(1+x.^2));	// end
    // mem-bound, N=1E7 => size(i)=8E7/siz (80MB!!!)
    ////////////
    // PARALLEL / reduction and finish
    ////////////
    select mod
    case "s",			TAG=7;	// Any tag would do
        if SLV				// All slaves send result back
            MPI_Send(lsum,             0,TAG);
        else				// Here at master
            Sum =lsum;			// save local result
            for slv=1:siz-1			// collect in any order
                //	MPI_Recv(lsum,MPI_ANY_SOURCE,TAG,MPI_COMM_WORLD);
                MPI_Recv(lsum,TAG);
                Sum = Sum + lsum;			// and accumulate
            end				// order: slv or MPI_ANY_SOURCE
        end
    case "r",		Sum=0;		// reduction master = rank 0 @ WORLD
        MPI_Reduce(lsum,Sum, MPI_SUM,  0,MPI_COMM_WORLD);
    end

    MPI_Finalize;

    //////////////////////////////////////////////////////////////////
    //results.time = etime(clock,T);	//
    //////////////////////////////////////////////////////////////////
    results.err  = Sum-%pi;
    results.pi   = Sum // ;


    //////////////////////////////
    // FINAL CHECK //
    //////////////////////////////
    if abs(results.err)>5E-10
        warning("pretty nice pi value! go fix it")
    end
    disp(results)
endfunction


