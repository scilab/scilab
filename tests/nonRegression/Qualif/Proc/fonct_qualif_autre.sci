function tests_suite_up_to_date=find_tests_suite_up_to_date(directory)
//recuperer les batteries de tests en cours sous directory dans une matrice
// verifier que tests_suite sont des repertoires quand directory=envq_work 
// et fichier quand log ???
tests_suite=""
select directory
 case envq_work then 
     tests_suite=basename(listfiles(directory)); 
     the_list=""
     j=1
     for i=1:size(tests_suite,"*")
         if isdir (envq_work+tests_suite(i)) then 
//       tests_suite(i) est un répertoire
            the_list(j) = tests_suite(i)
            j=j+1
 //           disp "boubou" 
   //         pause
         end
     end // du for
     tests_suite=the_list  
 case envq_log+envq_ver then
     tests_suite=basename(listfiles(envq_log+envq_ver+"\*.log")) 
 //      ierr=execstr(tests_suite=basename(listfiles(envq_log+envq_ver+"\*.log")),'errcatch') 
 //      disp ("111")
 //      pause
 case envq_log then
//************************************************************* 
// chercher la liste sur frioul
//************************************************************* 
    tests_suite=read(envq_log+"frioul_tests_suite_list.txt",-1,1,'(a)')  
 else
    disp ("PROBLEME")
    abort
 end // du case
//disp("tests_suite de find tests suute up to date")
//pause
tests_suite_up_to_date=sort(tests_suite)
//disp (tests_suite_up_to_date)
//disp (typeof(tests_suite_up_to_date))
//pause
endfunction

function affich_message (graph_affich, texte)
// texte = 'No tests_suite to select'
if graph_affich then
//afficher message windows pour indiquer pas de batterie dans la liste de selection
x_message(texte)
else
//afficher message prompt pour indiquer pas de batterie dans la liste de selection
disp(texte)
end // du if 
endfunction

function tests_suite_selected=graph_select_tests_suite(command, tests_suite)
//parcourir la matrice ainsi créée et charger la liste pour le menu de selection des batteries
  the_list=list()
  for i=1:size(tests_suite,"*")
     the_list($+1)=list(tests_suite(i),1,['Non','Oui'])
  end // du for
  rep=x_choices("Tests Suite Selection Menu for "+convstr(command,'u')+" command.",the_list);

  //prendre dans rep les reponses à oui (valeur 2);retrouver la batterie de tests correspondante ; et lancer son execution
  tests_suite_selected=""
  for i=1:size(tests_suite,"*")
    if rep(i)==2 then
  //     disp ("je sui sla")
  //     pause
       tests_suite_selected = tests_suite_selected + " " + tests_suite(i)
  //           disp ("je sui encore la")
  //     pause
    end // du if
  end // du for
endfunction

function tests_suite_selected=input_select_tests_suite(tests_suite)
   disp (" ");
   disp(tests_suite);
   disp (" ");
   //tests_suite_selected=input("Type tests_suite : ","string")  -- input ne prend que le premier string avant le blanc
   disp("Type tests_suite : ")
   the_list=list()
   the_list=read(%io(1),1,1,'(a)') 
   tests_suite_selected=the_list
endfunction

function graph_cmd(command,directory)
//virer graph interf des param pas  besoin  verifier
available_tests_suite=find_tests_suite_up_to_date(directory)
//disp (" toto")
//pause
if isempty(available_tests_suite) then
   affich_message (%t, "*****   No tests_suite available   *****")
else
   choosen_tests_suite=graph_select_tests_suite(command,available_tests_suite)
 //  disp (" choosen")
//pause
   if ~isempty(choosen_tests_suite) then
   // disp ("la la la")
   //pause
      exec_cmd(command, choosen_tests_suite)
   //else
   //   return
   end // du if
end // du if
endfunction

function prompt_cmd(command,param_command,directory)
 if isempty(param_command)
      // pas de parametres, saisie au prompt scilab
      available_tests_suite=find_tests_suite_up_to_date(directory)
      if isempty(available_tests_suite) then
         affich_message (%f, "*****   No tests_suite available   *****")
         // sortir de la fonction 
         return
      else
  //       disp (" titi")
  //      pause //affich_message (graph_interf, available_tests_suite)
         choosen_tests_suite=input_select_tests_suite(available_tests_suite)
  //               disp ("bababababa")
  //      pause
         // pour l'instant ne rien faire si erreur dans saisie laisser faire l OS
      end // du if
 else
         // il existe des arguments pour l'instant ne rien faire si erreur dans saisie laisser faire l OS
        choosen_tests_suite = param_command
 end // du if
        disp ("choosen_tests_suite : " + choosen_tests_suite)
      pause  
 if ~isempty(choosen_tests_suite) then
      exec_cmd(command, choosen_tests_suite);
   //else
   //   return
end // du if
endfunction

function exec_cmd(command, choosen_tests_suite)
 if MSDOS then
 //cmd="cmd.exe /e:on /v:on /c init_qualif.bat&"+command+".bat "+choosen_tests_suite
  cmd="cmd.exe /e:on /v:on /C init_qualif.bat&test.bat "+choosen_tests_suite

      disp (cmd)
      pause
 //  %SystemRoot%\system32\cmd.exe /e:on /v:on /K e:\qualif\proc\init_qualif.bat
      chdir(envq_proc)
      unix_w(cmd)
      //unix_w ("cmd.exe /e:on /v:on /c init_qualif.bat&"+command+".bat "+choosen_tests_suite)
      chdir(envq_qualif)
//      unix_w (envq_proc+"init_qualif.bat&"+command+".bat "+choosen_tests_suite)
 else
      unix_w (command+ " " +choosen_tests_suite)
 end
endfunction


function exec_cmdd(command, choosen_tests_suite)
rep=tokens(choosen_tests_suite," ")
for i=1:size(rep,"*")
    case "copyftp" then 
    tests_suite=basename(listfiles(directory)); 
     the_list=""
     j=1

     tests_suite=the_list  
    case "runtest" then
 
    case "copyrun" then

    case "clearlog" then
      system_cmd="erase /s /q "
      param_cmd=rep(i)   
    case "clearwork" then
       system_cmd="erase /s /q "
       param_cmd=rep(i)      
end // du forselect command

if MSDOS then
 //cmd="cmd.exe /e:on /v:on /c init_qualif.bat&"+command+".bat "+choosen_tests_suite
  cmd="cmd.exe /e:on /v:on /C init_qualif.bat&test.bat "+choosen_tests_suite

      disp (cmd)
      pause
 //  %SystemRoot%\system32\cmd.exe /e:on /v:on /K e:\qualif\proc\init_qualif.bat
      chdir(envq_proc)
      unix_w(cmd)
      //unix_w ("cmd.exe /e:on /v:on /c init_qualif.bat&"+command+".bat "+choosen_tests_suite)
      chdir(envq_qualif)
//      unix_w (envq_proc+"init_qualif.bat&"+command+".bat "+choosen_tests_suite)
 else
      unix_w (command+ " " +choosen_tests_suite)
 end
endfunction





function affich_date
w=getdate()
disp(date() + "  "+string(w(7))+" h. "+string(w(8))+" mn. "+string(w(9))+" s. ")
endfunction


function affich_result(pasOK, num)
// affiche FAILED ou PASSED, fonction utiliser dans les tests-suites de non regression
   // This part display result
   // le booleen pasOK est à faux
   if ~pasOK then
     disp("*****************************************  Test bug" + string(num)  +" : FAILED ! ");
   else
    disp("Test bug" + string(num)  +" : PASSED");
   end
endfunction

function my_mat = list2mat (my_list)
my_mat = ['']  
//for k=1:size(my_list,"*")
for k=1:size(my_list)
         my_mat (k) = my_list(k)
end
endfunction 


function mat_rep = sel_rep(my_mat)
  mat_rep = ['']
// cas d'une liste
mat_rep = ''
  n=1
  for k=1:size(my_mat,"*")
//  for k=1:size(my_mat)
     if isdir(my_mat(k)) then 
         mat_rep (n) = my_mat(k)
         n = n+1
     end
  end
endfunction

function copyftp(param)
if argn(2)==0 then 
   param = []
end // du if
prompt_cmd("copyftp",param,envq_log)
endfunction


function ZZZZZZ
       if argn(2)==0 then 
             //disp " "
             write(%io(2),"COPYFTP : ")
             write(%io(2),"Pour copier en local via ftp : ")
             write(%io(2),"- toutes les tests_suites faire : copyftp *")
             write(%io(2),"- certaines tests_suites faire  : copyftp ""nonreg* tests_20?? scicos""")
             write(%io(2),"- une tests_suite faire         : copyftp ""scicos""")
             disp " "
       else
             if MSDOS then
                unix_w (envq_proc+"init_qualif.bat&copyftp.bat "+my_cmd)
             end
       end
 endfunction

function ierr=runtest22(varargin)
// se positionne dans le repertoire de la tests_suite et lance son execution
ierr=0
chdir (envq_work)
disp(varargin)
// ts_work = list2mat (varargin)
ts_work = varargin
disp (typeof(ts_work))
disp (ts_work)
pause

//disp(varargin(argn(1)))
//disp(varargin(argn(2)))
//disp(varargin(argn(3)))
//pause

//ts_work = ['']
//ts_work = [my_cmd]

//disp (ts_work)
//pause

//recuperation des parametres, voir si exite un booleen pour copie via ftp
//if argn(2) < 1 then 
//   disp ("erreur pas d''argument")
//elseif argn(2) == 1 then
//disp ( varargin)
//disp (type (varargin))
//   ts_work = list2mat (varargin)
//   via_ftp = %f
//else 
// argn(2) est plus grand que 1
//disp ("varargin : "+varargin)
//disp ("varargin : "+typeof (varargin))
//disp (type(varargin(argn(2))))
//   if type(varargin(argn(2)))==4 then
//    le dernier parametre est un booleen, le mettre dans la variable via_ftp
//      via_ftp = varargin(argn(2))
//      disp (via_ftp)
      
//      varargin(argn(2)) = null()
//      ts_work = list2mat (varargin)
//      disp (ts_work)
//   else
//      ts_work = list2mat (varargin)
//      via_ftp = %f
//   end   
//end
disp (" je suis là")
pause
// copie via ftp du serveur vers env qualif de la machine cible
//if via_ftp then
// copier via ftp la (les) tests_suite(s) demandées
//   if ts_work == all then

//copy_ftp (ts_work)
//end

//disp ("toto")

// conserve uniquement les repertoires 
disp("----")

ts_work = sel_rep(ts_work)
disp ("ts_work : "+ts_work)
disp("type : "+typeof(ts_work))
pause

//if ts_work == [''] then 
if ts_work == '' then 
         disp ("")
         disp (" CE N''EST PAS UN NOM (repertoire) DE TESTS-SUITE.----------------------------")
//          disp("Error n° "+string(ierr));
//          disp(lasterror());
         disp ("coucou")
else
disp ("pas ici")
// corps de la fonction runtest proprement dite, boucle sur les tests_suite(s)
  for k=1:size(ts_work,"*")
//   for k=1:size(ts_work)
       chdir (envq_work + ts_work(k))
       ierr = exec (ts_work(k) + ".sce", 'errcatch')
       if ierr<>0 then 
         disp("")
         disp " LA TESTS-SUITE " + ts_work(k) + " S ''EST INCORRECTEMENT TERMINEE.----------------------------"
         disp("Error n° "+string(ierr));
         disp(lasterror());
         disp("")
       end
   
   end
end
endfunction


function displist (tout)

// exemple : displist(["autre?ep";"n*"])
ierr=0
//tout = ["autrerep","hif","n*"] 
the_list=[]
for i=1:size(tout,"*")
    if isdir (envq_work+tout(i)) then 
//  tout(i) est un répertoire
       the_list = [the_list;tout(i)]
    else
       the_list = [the_list;basename(listfiles(envq_work+tout(i)))]
    end
end // du for

//pause

the_list2=[]
for i=1:size(the_list,"*")
    if isdir (envq_work+the_list(i)) then 
       the_list2 = [the_list2;the_list(i)]
    end
end // du for

// pause

for k=1:size(the_list2,"*")
    chdir (envq_work + basename(the_list2(k)))
    diary (envq_log+envq_ver+"\"+the_list2(k)+".txt")
    ierr = exec (the_list2(k) + ".sce", 'errcatch')
    if ierr<>0 then 
       disp("")
       disp " LA TESTS-SUITE " + the_list2(k) + " S ''EST INCORRECTEMENT TERMINEE.----------------------------"
       disp("Error n° "+string(ierr));
       disp(lasterror());
       disp("")
    end
    diary(0)
 //       disp (basename(the_list2(k)))
end // du for


endfunction


function rruntest(varargin)

if argn(2)==0 then 
             //disp " "
             write(%io(2),"RUNTEST : ")
             write(%io(2),"- toutes les tests_suites faire : runtest (""*"")")
             write(%io(2),"- certaines tests_suites faire  : runtest ""nonreg*" , "tests_20??" , "scicos""")
             write(%io(2),"- une tests_suite faire         : runtest ""scicos""")
             disp " "
else

disp (length(varargin))

for i=1:length(varargin)
disp (varargin(i))
end // du for

my_list=varargin

pause

// exemple : displist(["autre?ep";"n*"])
ierr=0
//my_list = ["autrerep","hif","n*"] 
//my_list = listfiles(envq_work+my_list
the_list=[]
for i=1:size(my_list)
//for i=1:size(my_list,"*")
    if isdir (envq_work+my_list(i)) then 
//  my_list(i) est un répertoire
       the_list = [the_list;my_list(i)]
    else
       the_list = [the_list;basename(listfiles(envq_work+my_list(i)))]
    end
end // du for

disp("ici")
pause


the_list2=[]
for i=1:size(the_list,"*")
    if isdir (envq_work+the_list(i)) then 
       the_list2 = [the_list2;the_list(i)]
    end
end // du for

disp("là")
pause

for k=1:size(the_list2,"*")
    chdir (envq_work + basename(the_list2(k)))
    diary (envq_log+envq_ver+"\"+the_list2(k)+".txt")
    ierr = exec (the_list2(k) + ".sce", 'errcatch')
    if ierr<>0 then 
       disp("")
       disp " LA TESTS-SUITE " + the_list2(k) + " S ''EST INCORRECTEMENT TERMINEE.----------------------------"
       disp("Error n° "+string(ierr));
       disp(lasterror());
       disp("")
    end
    diary(0)
 //       disp (basename(the_list2(k)))
end // du for

end
endfunction

function test (str)
a=[]
interv=strindex(str," ")
debut=1
for i=1:length(interv)
    a=[a;part(str,[debut:(interv(i))-1])]
    debut=interv(i)+1
end
a=[a;part(str,[debut:length(str)])]
disp (a)

pause

endfunction 

function clearwork (param)
if argn(2)==0 then 
   param = []
end // du if
prompt_cmd("clearwork",param,envq_work)
endfunction
       
       
function YYYYYYYYY       
       if argn(2)==0 then 
             //disp " "
             write(%io(2),"CLEARWORK : ")
             write(%io(2),"Pour supprimer un, plusieurs ou toutes les test-suites du répertoire Work local : ")
             write(%io(2),"- toutes les tests_suites faire : clearwork *")
             write(%io(2),"- certaines tests_suites faire  : clearwork ""nonreg* tests_20?? scicos""")
             write(%io(2),"- une tests_suite faire         : clearwork ""scicos""")
             disp " "
       else
             if MSDOS then
                unix_w (envq_proc+"init_qualif.bat&clearwork.bat "+my_cmd)
                             disp (my_cmd)
else
                unix_w ("clearwork" +my_cmd)
             end
       end
endfunction

function clearlog (param)
if argn(2)==0 then 
   param = []
end // du if
prompt_cmd("clearlog",param,envq_log+envq_ver)
endfunction


function XXXXX
disp ("ici clearlog")
pause
if graph_select_global_bool == %f then
          if argn(2)==0 then 
          // pas de parametres, saisie au prompt scilab
          my_cmd = input_select_tests_suite(find_tests_suite_up_to_date(envq_log+envq_ver))
          end // du if
          // verifier que les donnees saisies au prompt ou directement tapee avec la commande sont valides (et existes)
          
end //if
//else
if MSDOS then
           unix_w (envq_proc+"init_qualif.bat&clearlog.bat "+my_cmd)
else
           unix_w ("clearlog" +my_cmd)
end
//end 
graph_select_global_bool = %f

//       if argn(2)==0 then 
//             //disp " "
//             write(%io(2),"CLEARLOG : ")
//             write(%io(2),"Pour supprimer un, plusieurs ou tous les fichiers log : ")
//             write(%io(2),"- toutes les tests_suites faire : clearlog *")
//             write(%io(2),"- certaines tests_suites faire  : clearlog ""nonreg* tests_20?? scicos""")
//             write(%io(2),"- une tests_suite faire         : clearlog ""scicos""")
//             disp " "
//       else
//             if MSDOS then
//                unix_w (envq_proc+"init_qualif.bat&clearlog.bat "+my_cmd)
//             else
//                unix_w ("clearlog" +my_cmd)
//             end
//       end
endfunction

function showlog (my_cmd)
       if argn(2)==0 then 
             //disp " "
             write(%io(2),"SHOWLOG : ")
             write(%io(2),"Pour visualiser la derniere ligne de un, plusieurs ou tous fichiers log : ")
             write(%io(2),"- toutes les tests_suites faire : showlog *")
             write(%io(2),"- certaines tests_suites faire  : showlog ""nonreg* tests_20?? scicos""")
             write(%io(2),"- une tests_suite faire         : showlog ""scicos""")
             disp " "
       else
             if MSDOS then
                unix_w (envq_proc+"init_qualif.bat&showlog.bat "+my_cmd)
             else
                unix_w ("showlog" +my_cmd)
             end
       end
endfunction


function dirlog (my_cmd)
if argn(2)==0 then 
   param = []
end // du if
prompt_cmd("clearlog",param,envq_log+envq_ver)
endfunction

function AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA
       if argn(2)==0 then 
             //disp " "
             write(%io(2),"DIRLOG : ")
             write(%io(2),"Pour visualiser la derniere ligne de un, plusieurs ou tous fichiers log : ")
             write(%io(2),"- toutes les tests_suites faire : showlog *")
             write(%io(2),"- certaines tests_suites faire  : showlog ""nonreg* tests_20?? scicos""")
             write(%io(2),"- une tests_suite faire         : showlog ""scicos""")
             disp " "
       else
             disp(sort(basename(listfiles(envq_log+envq_ver))))

      //if MSDOS then
      //               unix_w (envq_proc+"init_qualif.bat&showlog.bat "+my_cmd)
      //           else
      //               unix_w ("showlog" +my_cmd)
      //       end  
       end
endfunction

function dirwork (my_cmd)
      if argn(2)==0 then 
             //disp " "
             write(%io(2),"DIRWORK : ")
             write(%io(2),"Pour visualiser la derniere ligne de un, plusieurs ou tous fichiers log : ")
             write(%io(2),"- toutes les tests_suites faire : showlog *")
             write(%io(2),"- certaines tests_suites faire  : showlog ""nonreg* tests_20?? scicos""")
             write(%io(2),"- une tests_suite faire         : showlog ""scicos""")
             disp " "
       else
             disp(sort(basename(listfiles(envq_work+my_cmd))))

      //if MSDOS then
      //               unix_w (envq_proc+"init_qualif.bat&showlog.bat "+my_cmd)
      //           else
      //               unix_w ("showlog" +my_cmd)
      //       end  
       end
endfunction


function aide(varargin)
// fonction executer au lancement de l'environnement de qualification scilab sous window
   if argn(2)==1 then
                  if (varargin(1)=="all") | (varargin(1)=="ALL" )then 
                 disp (" ")
                 copyftp()
                 runtest()
//               copyrun()
                 clearwork()
                 clearlog()
                 disp (" ")
                 end
   else 
                 disp (" ")
                 write(%io(2),"   ENVIRONNEMENT DE QUALIFICATION SCILAB")
                 disp (" ")
                 write(%io(2),"   commandes disponibles : COPYFTP() - RUNTEST() - COPYRUN() - CLEARWORK() - CLEARLOG()")
                 write(%io(2)," ")
                 write(%io(2),"   pour une aide faire : AIDE (""all"") ou lancer une commande sans parametres ")
                 disp (" ")
   end
endfunction


function copyrun(param)
if argn(2)==0 then  
             copyftp()
             runtest()
else
    copyftp (param)
    runtest (""""+param+"""")
end
endfunction




function runtest(tout)

if argn(2)==0 then 
             //disp " "
             write(%io(2),"RUNTEST : ")
             write(%io(2),"Pour executer en local : ")
             write(%io(2),"- toutes les tests_suites faire : runtest (""*"")")
             write(%io(2),"- certaines tests_suites faire  : runtest (""nonreg* tests_20?? scicos"")")
             write(%io(2),"- une tests_suite faire         : runtest (""scicos"")")
             disp " "
else

//disp (length(varargin))

//for i=1:length(varargin)
//disp (varargin(i))
//end // du for

my_list=tokens(tout," ")

//pause

// exemple : displist(["autre?ep";"n*"])
ierr=0
//my_list = ["autrerep","hif","n*"] 
//my_list = listfiles(envq_work+my_list
the_list=[]
//for i=1:size(my_list)
for i=1:size(my_list,"*")
    if isdir (envq_work+my_list(i)) then 
//  my_list(i) est un répertoire
       the_list = [the_list;my_list(i)]
    else
       the_list = [the_list;basename(listfiles(envq_work+my_list(i)))]
    end
end // du for

//disp("ici")
//pause


the_list2=[]
for i=1:size(the_list,"*")
    if isdir (envq_work+the_list(i)) then 
       the_list2 = [the_list2;the_list(i)]
    end
end // du for

//disp("là")
//pause

for k=1:size(the_list2,"*")
    bibi = the_list2(k)
    chdir (envq_work + basename(the_list2(k)))
    unix_w ("erase "+envq_log+envq_ver+"\test_en_cours.txt")
    diary (envq_log+envq_ver+"\test_en_cours.txt")
    ierr=0
//    diary (envq_log+envq_ver+"\"+the_list2(k)+".txt")
    ierr = exec (the_list2(k) + ".sce", 'errcatch')
    if ierr<>0 then 
       disp("")
       disp " LA TESTS-SUITE " + the_list2(k) + " S ''EST INCORRECTEMENT TERMINEE.----------------------------"
       disp("Error n° "+string(ierr));
       disp(lasterror());
       disp("")
    end
    diary(0)
    if MSDOS then
//disp ("msdos")    
//pause
//    disp ("bibi = " + bibi)
//pause
//unix_w ("type "+envq_log+envq_ver+"\test_en_cours.txt >> "+envq_log+envq_ver+"\"+the_list2(k)+".txt")
unix_w ("type "+envq_log+envq_ver+"\test_en_cours.txt >> "+envq_log+envq_ver+"\"+bibi+".txt")
//    unix_w ("erase "+envq_log+envq_ver+"\test_en_cours.txt")
    else
// monde linux utiliser commande cat
    end
//       disp (basename(the_list2(k)))
end // du for

end
endfunction
