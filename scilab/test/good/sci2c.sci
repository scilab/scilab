//
//  -*- Scilab -*-
//
// sci2c.sci
// Made by  Bruno JOFRET <bruno.jofret@inria.fr>
//
// Started on  Fri Nov 17 09:21:40 2006 jofret
// Last update Fri Nov 17 10:08:44 2006 jofret
//
// Copyright INRIA 2006
//

function res=sci2c(sciFile, resPath)
    // This function perform translation of a Scilab file into C code
    //   - sciFile : The file to translate
    //   - resPath [optional] : The path to generate C file (default is ./)

    res=0

    // Get default arguments
    [lhs,rhs]=argn(0)

    if rhs < 2 then
        resPath="./"
    end
    if rhs < 1 then
        error("Usage : sci2c(<sci_file_name>, [path_to_store_C_generated_file])")
    end

    // Moving \ into / for Windows users
    if MSDOS then
        sciFile=strsubst(sciFile,"\","/")
        resPath=strsubst(resPath,"\","/")
    end





    [l,mac]=where()
    disp("l=")
    disp(l)
    disp("mac=")
    disp(mac)

    disp("Begin of a Scilab 2 C Tool...")
    disp("Need a better name for further sexy behaviour...")
    return


endfunction