function AboutScicos_()
// Copyright INRIA


    Cmenu=[] ; %pt=[];

    message([strsubst(get_scicos_version(),"scicos","Scicos -")+"-";...
             "Do you want to see more ?";...
	     "     www.scicos.org      "]);

endfunction
