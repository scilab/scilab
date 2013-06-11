function convdat(name)
    if argn(2)<1 then name=input("Nom du fichier","s");end
    //name=input('nom du fichier','s')
    name=stripblanks(name);
    if part(name,length(name)-3:length(name))==".xls" then
        name=part(name,1:length(name)-4);
    end
    if execstr("sheet=readxls(name+''.xls'')","errcatch")~=0 then
        error("Ce n''est pas un fichier excel")
    end
    if length(sheet)~=2 then
        error("Il n''y a pas qu''une seule feuille")
    end

    s=sheet(1);
    [%u,ierr]=mopen(name+".dat","wb");
    if ierr~=0 then
        error("le fichier de sauvegarde ne peut pas être ouvert en écriture"),
    end
    names=s.text(1,:);
    s=s.value(2:$,:);
    if isnan(s) then
        error("Le fichier n''a pas le format attendu")
    end
    for i=1:size(s,2)
        ierr=execstr(names(i)+"=s(:,i)","errcatch");
        ierr=execstr("save(%u,"+names(i)+")","errcatch");
        if ierr~=0 then
            error("Impossible de sauver la variable"+names(i)),
        end
    end
    mclose(%u);
    mprintf("Le fichier Excel %s.xls a été transcrit dans le fichier Scilab %s.dat",name,name)
endfunction

