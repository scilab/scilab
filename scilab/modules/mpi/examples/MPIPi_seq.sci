function MPIPi_seq(N)
    // Pi_seq: Compute PI (sequential) by num.integr. of arctan(x) in [0..1]
    //
    //	Pi_seq [ (N) ]
    //
    //  N	[1E7]	#subdivisions of the [0, 1] interval
    //
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
    if argn(2)>1,	error("usage Pi_seq(N)"); end
    flag=0;
    [%H,%ierr] = evstr(string(N));
    flag=flag | size(N,"*")<>1 | %ierr<>0;
    flag=flag  |   fix(N)~=N   |           N<1;
    if flag,	error("usage Pi_seq( <int> N>0, <char> mod==''s|r'' )"), end

    ////////////////////////////////////
    // Results struct //
    ////////////////////////////////////
    results.pi  =0;
    results.err =0;
    results.time=0;

    ////////////////////////////
    // SEQUENTIAL // code, vectorized
    ////////////////////////////
    //    T=clock; //
    ////////////////////////////			// vectorized code, equivalent to
    width=1/N; Sum=0;		// for i=0:N-1
    i=0:N-1;			//   x=(i+0.5)*width;
    x=(i+0.5)*width;		//   Sum=Sum+4/(1+x^2);
    Sum=sum(4 ./ (1+x.^2));	// end
    Sum      = Sum/N ;		// 1/N better at end: don't lose resolution
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


