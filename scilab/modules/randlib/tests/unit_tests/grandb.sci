function tstbot()
	// Copyright INRIA
	//**********************************************************************
	//     A test program for the bottom level routines
	//     Scilab implementation of tstbot.f 
	//**********************************************************************
	NB=10
	NR=1000
	answer=ones(NB,NR);
	genlst=[1,5,10,20,32]
	nbad = 0
	str=['For five virtual generators of the 101';...
	' This test generates'+string(NB)+' numbers then resets the block';...
	'      and does it again';...
	' Any disagreements are reported -- there should be none'];
	write (%io(2),str)
	//
	//     Set up Generators
	//
	grand('setall',12345,54321,6789,9876);
	//
	//     For a selected set of generators
	//
	for ixgen = 1:5
	igen = genlst(ixgen)
	grand('setcgn',igen);
	write(%io(2),' Testing generator '+string(igen));
	//
	//     Use NB blocks
	//
	grand('initgn',-1);
	SD=grand('getsd');iseed1=SD(1);iseed2=SD(2);
	for iblock = 1:NB
		//     Generate NR numbers
		answer(iblock,1:NR)= grand(1,NR,'lgi');
		grand('initgn',1);
	end 
	grand('initgn',-1);
	//
	//     Do it again and compare answers
	//
	SD=grand('getsd');iseed1=SD(1);iseed2=SD(2);
	//
	//     Use NB blocks
	//
	for iblock = 1:NB
		//     Generate NR numbers
		itmp = grand(1,NR,'lgi');
		if itmp<>answer(iblock,:) then 
			str=[' Disagreement on regeneration of numbers';...
			' Block '+string(iblock)+' N within Block '];
			write(%io(2),str);
		end
		grand('initgn',1);
	end
	write (%io(2), ' Finished testing generator '+string(igen));
	write (%io(2), ' Test completed successfully');
	end
endfunction