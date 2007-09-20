function AboutScicos_()
//   INRIA
//** 21 Nov 2006
//** Comments by Simone Mannori

    Cmenu=[] ; %pt=[];

    message([strsubst(get_scicos_version(),"scicos","Scicos -")+"-";...
             "Do you want to see more ?";...
	     "     www.scicos.org      "]);

endfunction
