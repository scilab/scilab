function prettyprint(inputscifile, outputscifile)
    // Function used to load a .sci file and generate a pretty printed file
    // Used to test new Scilab parser
    //
    // Vincent COUVERT - DIGITEO - 2008

    // Loads the .sci file
    getf(inputscifile, "c");

    // Generate pretty printed code
    names = who("get");
    printf("%s\n", "Generate pretty printed code for: " + names(1));
    txt = fun2string(evstr(names(1)), names(1));
    txt = stripblanks(txt);
    txt(txt == "") = [];
    // Save generated code
    mputl(txt, outputscifile);
endfunction
