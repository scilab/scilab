function change_help_language(language)

    // =========================================================================================
    // Author : Pierre MARECHAL
    // Scilab team
    // Copyright INRIA
    // Date : August 1st 2006
    //
    // change_help_language(language)
    // =========================================================================================

    global %helps;

    // Vérification des paramètres
    // -----------------------------------------------------------------------------------------
    [lhs,rhs]=argn(0);
    if rhs <> 1 then error(39); end
    if type(language) <> 10 then error(55,1); end

    // Sauvegarde du chemin dans lequel l'on se trouve
    // -----------------------------------------------------------------------------------------
    current_directory = pwd();

    dirs = %helps(:,1);
    titles = %helps(:,2);

    // Parcours des répertoires listés dans %helps
    for k=1:size(dirs,"*")

        // Vérification que le whatis.htm existe
        if fileinfo(pathconvert(dirs(k)+"/../"+language+"/whatis.htm",%f,%f)) <> [] then

            // Construction du nouveau répertoire
            chdir(pathconvert(dirs(k)+"/../"+language,%f,%f));
            if MSDOS then
                new_dir = getlongpathname(pwd());
            else
                new_dir = pwd();
            end

            // Construction du nouveau titre
            whatis = mgetl("whatis.htm");
            title_row = grep(convstr(whatis,"l"),"<title>");
            if strindex(convstr(whatis(title_row),"l"),"</title>") <> [] then

                start_title = strindex(convstr(whatis(title_row),"l"),"<title>") + 7;
                end_title = strindex(convstr(whatis(title_row),"l"),"</title>") - 1;
                new_title = stripblanks(part(whatis(title_row),start_title:end_title));

                // Modification de %helps
                %helps(k,1) = new_dir;
                %helps(k,2) = new_title;
            end
        end
    end

    chdir(current_directory);

endfunction
