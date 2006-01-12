# Scipad texts in Polish
# in sync at least with v5.55
# Created by Bartosz Kosiorek <gang65@poczta.onet.pl>, 2005

# commons
::msgcat::mcset pl "Cancel" "Anuluj"
::msgcat::mcset pl "Cance&l" "&Anuluj"
::msgcat::mcset pl "OK" "OK"
::msgcat::mcset pl "Scilab working" "Scilab pracuje"
::msgcat::mcset pl "All files" "Wszystkie pliki"
::msgcat::mcset pl "Add/Change" "Dodaj/Zmie\u0144"
::msgcat::mcset pl "Untitled" "BezNazwy"
::msgcat::mcset pl "Close" "Zamknij"
::msgcat::mcset pl "&Close" "&Zamknij"

# buffernavigation.tcl
::msgcat::mcset pl "Goto Line?" "I\u015B\u0107 do linii?"
::msgcat::mcset pl "Go to:" "Id\u017A do:"
::msgcat::mcset pl "&logical line" "&linia logiczna"
::msgcat::mcset pl "&physical line" "linia &fizyczna"
::msgcat::mcset pl "in &function" "w &funkcji"
::msgcat::mcset pl "in &current file" "w bie\u017C\u0105&cym pliku"
::msgcat::mcset pl "Outside of function definition"\
                   "Poza definicj\u0105 funkcji"


# colorize.tcl
::msgcat::mcset pl "Wait seconds while recolorizing file"\
                   "Prosz\u0119 czeka\u0107, dop\u00F3ki\
                   nie przekoloruje pliku"


# completion.tcl
::msgcat::mcset pl "No possible completion" "Brak mo\u017Cliwego zako\u0144czenia"
::msgcat::mcset pl "possible completions" "mo\u017Cliwe zako\u0144czenia"
::msgcat::mcset pl "Possible completions" "Mo\u017Cliwe zako\u0144czenie"


# db_argsmanagement.tcl
::msgcat::mcset pl "Variable:" "Zmienna :"
::msgcat::mcset pl "Value:" "Warto\u015B\u0107 :"
::msgcat::mcset pl "Get current value from Scilab"\
                   "Pobiera bierz\u0105c\u0105\
                   warto\u015B\u0107 z programu Scilab"


# db_configureexec.tcl
::msgcat::mcset pl "Configure execution" "Konfiguracja wykonania"
::msgcat::mcset pl "Function name:" "Nazwa funkcji :"
::msgcat::mcset pl "Obtain" "Otrzymano"
::msgcat::mcset pl "Input arguments:" "Parametry wej\u015Bciowe :"
::msgcat::mcset pl "Remove" "Usu\u0144"
::msgcat::mcset pl "Function name or input arguments do\
                   not match definition of the function"\
                   "Nazwa funkcji lub jej argumenty, nie\
                   pasuj\u0105 do definicji funkcji"
::msgcat::mcset pl "in the file!\n\nCheck function name\
                   and arguments (names, order) in the configuration dialog.\
                   \nArguments order can be changed using drag and drop with\
                   right mouse button in the arguments listbox."\
                   "w pliku!\n\n Sprawd\u017A nazw\u0119\
                   i argumenty funkcji w oknie konfiguracji.\
                   \nKolejno\u015B\u0107 argument\u00F3w mo\u017Ce\
                   zosta\u0107 zmieniona przy pomocy metody\
                   przeci\u0105gnij i upu\u015B\u0107 (prawym klawiszem\
                   myszy na li\u015Bcie argument\u00F3w)"
::msgcat::mcset pl "Error in selected function name or arguments"\
                   "B\u0142\u0105d w nazwie zaznaczonej\
                   funkcji, lub jej parametrach"
::msgcat::mcset pl "Debugger is not yet developed for sce scripts!"\
                   "Odpluskwiacz nie jest jeszcze\
                   rozwijany dla skrypt\u00F3w sce!"


# db_debugsession.tcl
::msgcat::mcset pl "Scilab is working, wait for the\
                   prompt to issue a debugger command."\
                   "Scilab pracuje, poczekaj na znak\
                   zach\u0119ty, aby wyda\u0107 polecenie odpluskwiaczowi."
::msgcat::mcset pl "Cursor position is out of reach!"\
                   "Pozycja kursora jest poza zasi\u0119giem!"
::msgcat::mcset pl "Cursor must be in a function"\
                   "Kursor musi znajdowa\u0107 si\u0119 w funkcji"
::msgcat::mcset pl "No effect - The debugged file is not stuck"\
                   "Brak efektu - odpluskwiany plik nie\
                   ma nieprawid\u0142owo\u015Bci"
::msgcat::mcset pl "Scilab execution error" "B\u0142\u0105d wykonywania Scilab"
::msgcat::mcset pl "The shell reported an error while trying to execute "\
                   "Pow\u0142oka wys\u0142a\u0142a\
                   b\u0142\u0105d, podczas pr\u00F3by wykonywania "
::msgcat::mcset pl " at line " " w linii "
::msgcat::mcset pl " of " " na "
::msgcat::mcset pl "Execution aborted!" "Wykonywanie przerwane!"


# db_init.tcl
::msgcat::mcset pl "Please wait..." "Prosz\u0119 czeka\u0107..."


# db_states.tcl
::msgcat::mcset pl "Currently no debug session"\
                   "Teraz nie jest uruchomione odpluskwianie"
::msgcat::mcset pl "Ready to start debug"\
                   "Gotowy do uruchamiania odpluskwiania"
::msgcat::mcset pl "Debug in progress" "Trwa odpluskwianie"


# db_watch.tcl
::msgcat::mcset pl "Watch" "Obserwowane"
::msgcat::mcset pl "Watch variables:" "Zmienne obserwowane :"
::msgcat::mcset pl "Remove" "Usu\u0144"
::msgcat::mcset pl "Call stack:" "Stos wywo\u0142a\u0144:"
::msgcat::mcset pl "Show watch variables" "Wy\u015Bwietl zmienne obserwowane"
::msgcat::mcset pl "Hide watch variables" "Ukryj zmienne obserwowane"
::msgcat::mcset pl "Show call stack" "Wy\u015Bwietl stos wywo\u0142a\u0144"
::msgcat::mcset pl "Hide call stack" "Ukryj stos wywo\u0142a\u0144"
::msgcat::mcset pl "Undefined elements are not legal in variable"\
                   "Niezdefiniowane elementy nie s\u0105\
                   prawid\u0142owe w zmiennej"
::msgcat::mcset pl ".\nThis variable will not be updated in Scilab."\
                   ".\nTe zmienne nie zostan\u0105 zaktualizowane w Scilab-ie"
::msgcat::mcset pl "Illegal undefined element found"\
                   "Znaleziono nieprawid\u0142owe i niezdefiniowane elementy"


# filecommands.tcl
::msgcat::mcset pl "New Script" "Nowy skrypt"
::msgcat::mcset pl "The contents of" "Zawarto\u015B\u0107"
::msgcat::mcset pl "may have changed, do you wish to save your changes?"\
                   "mog\u0142a ulec zmianie. Czy chcesz zapisa\u0107 zmiany?"
::msgcat::mcset pl "Save Confirm?" "Potwierdzenie zapisu"
::msgcat::mcset pl "Open source of..." "Otw\u00F3rz \u017Ar\u00F3d\u0142o..."
::msgcat::mcset pl "Open source of:" "Otw\u00F3rz \u017Ar\u00F3d\u0142o :"
::msgcat::mcset pl "Weird drag&drop" "Dziwne przeci\u0105gnij i upu\u015B\u0107"
::msgcat::mcset pl "is neither a valid filename nor a\
                   valid directory. Either you're dragging an object of\
                   another type, or you hit a bug of the dnd mechanism."\
                   "nie jest ani parawid\u0142ow\u0105 nazw\u0105 pliku,\
                   ani prawid\u0142owym katalogiem. Albo objekt kt\u00F3ry przeci\u0105gasz\
                   jest innego typu, albo znalaz\u0142e\u015B b\u0142\u0105d w mechani\u017Amie\
                   przeci\u0105gnij i upu\u015B\u0107."
::msgcat::mcset pl "File not found" "Nie znaleziono pliku"
::msgcat::mcset pl "does not exist anymore. Do you want\
                   to create an empty file with the same name?"\
                   "ju\u017C nie istnieje. Czy\
                   chcesz utworzy\u0107 pusty plik o tej samej nazwie? ?"
::msgcat::mcset pl "Open file" "Otw\u00F3rz plik"
::msgcat::mcset pl "The file" "Plik"
::msgcat::mcset pl "is already opened! Save the current\
                   opened file to another name and reopen it from disk!"\
                   "jest ju\u017C otwarty! Zapisz\
                   bie\u017C\u0105cy plik pod inna nazw\u0105 i otw\u00F3rz\
                   ponownie z dysku!"
::msgcat::mcset pl "File" "Plik"
::msgcat::mcset pl "saved" "zapisano"
::msgcat::mcset pl "cannot be written!" "nie mo\u017Ce by\u0107 zapisany!"
::msgcat::mcset pl "The contents of " "Zawarto\u015B\u0107 pliku"
::msgcat::mcset pl "has changed on disk, save it anyway?"\
                   "uleg\u0142a zmianie na dysku. Czy mimo to zapisa\u0107?"
::msgcat::mcset pl "has been modified outside of Scipad.\
                   Do you want to reload it?"\
                   "uleg\u0142a zmianie poza programem Scipad.\
                   Czy chcesz j\u0105 wczyta\u0107 ponownie ?"
::msgcat::mcset pl "File has changed!" "Plik zosta\u0142 zmieniony"
::msgcat::mcset pl "Save as" "Zapisz jako"
::msgcat::mcset pl "Scilab files" "Pliki Scilab"
::msgcat::mcset pl "Scicos files" "Pliki Scicos"
::msgcat::mcset pl "XML files" "Pliki XML"
::msgcat::mcset pl "Wait seconds while loading and colorizing file"\
                   "Prosz\u0119 czeka\u0107. Trwa\
                   wczytywanie i dodawanie koloru do pliku"
::msgcat::mcset pl "The contents of this file may have\
                   changed, do you wish to save your changes?"\
                   "Zawarto\u015B\u0107 tego pliku\
                   mog\u0142a ulec zmianie. Czy chcesz zapisa\u0107 zmiany?"
::msgcat::mcset pl "Revert" "Przywr\u00F3ci\u0107 do wersji zapisanej w pliku"
::msgcat::mcset pl "to saved?" "?"
::msgcat::mcset pl "Revert Confirm?" "Potwierdzenie przywr\u00F3cenia"


# findreplace.tcl
::msgcat::mcset pl "The string" "Ci\u0105g znak\u00F3w"
::msgcat::mcset pl "could not be found in" "nie zosta\u0142 znaleziony w"
::msgcat::mcset pl "could not be found in any of the opened files !"\
                   "nie zosta\u0142 znaleziony, w \u017Cadnym otwartym pliku !"
::msgcat::mcset pl "replacements done" "Zamienianie zako\u0144czone"
::msgcat::mcset pl "Find" "Znajd\u017A"
::msgcat::mcset pl "No match found in the selection for"\
                   "Nie znaleziono w zaznaczeniu"
::msgcat::mcset pl "\nWould you like to look for it in the entire text?"\
                   "\nCzy chcesz przeszuka\u0107 ca\u0142y tekst ?"
::msgcat::mcset pl "You are searching for an empty string!"\
                   "Szukasz pustego \u0142a\u0144cucha!"
::msgcat::mcset pl "Re&place" "Za&mie\u0144"
::msgcat::mcset pl "Find what:" "Znajd\u017A :"
::msgcat::mcset pl "Replace with:" "Zamie\u0144 na :"
::msgcat::mcset pl "Find &Next" "Z&najd\u017A nast\u0119pne"
::msgcat::mcset pl "Replace &All" "&Zamie\u0144 wszystko"
::msgcat::mcset pl "&Upwards" "&Do g\u00F3ry"
::msgcat::mcset pl "&Downwards" "D&o do\u0142u"
::msgcat::mcset pl "Match &whole word only" "U&wzgl\u0119dnij tylko ca\u0142e wyrazy"
::msgcat::mcset pl "Match &case" "Uw&zgl\u0119dnij wielko\u015B\u0107 liter"
::msgcat::mcset pl "&Regular expression"\
                   "Wy&ra\u017Cenie regularne"
::msgcat::mcset pl "In all &opened files"\
                   "We wszystkich &otwartych plikach"
::msgcat::mcset pl "In &selection only" "Ty&lko w zaznaczeniu"
::msgcat::mcset pl "In a director&y" "W ka&talogu"
::msgcat::mcset pl "Directory r&ecurse search" "Prz&eszukiwanie dalszych katalog\u00F3w"
::msgcat::mcset pl "In files/file types:" "W pliku/typie pliku :"
::msgcat::mcset pl "In directory:" "W katalogu :"
::msgcat::mcset pl "Back at the first file searched!"\
                   "Powr\u00F3cono do pierwszego przeszukiwanego pliku!"
::msgcat::mcset pl "No more matches" "Nie znaleziono nic pasuj\u0105cego"


# findinfiles.tcl
::msgcat::mcset pl "Directory access issue"\
                   "B\u0142\u0105d dost\u0119pu do katalogu"
::msgcat::mcset pl "The directory you specified cannot be\
                   reached. The search won't bring any result."\
                   "Nie mo\u017Cna wej\u015B\u0107 do katalogu .\
                   Szukanie nie przyniesie rezultatu."
::msgcat::mcset pl "Some directories or files could not\
                   be open and will be ignored during search.\n You might\
                   miss read access to them.\n\n The following ones will be\
                   ignored"\
                   "Niekt\u00F3re katalogi lub pliki nie mog\u0105 by\u0107 otwarte\
                   i zostan\u0105 zignorowane w trakcie szukania.\n\
                   Prawdopobobnie masz nieustawiony tryb do odcztu dla nich.\n\n\
                   Poni\u017Csze pliki zostan\u0105 zignorowane"
::msgcat::mcset pl "Ignored files" "Ignorowane pliki"
::msgcat::mcset pl "Searching in file:" "Szukanie w pliku:"
::msgcat::mcset pl "End of file search:" "Koniec przeszukiwania pliku:"
::msgcat::mcset pl "matches found" "znaleziono pasuj\u0105cych"
::msgcat::mcset pl "Constructing file list:"\
                   "Tworzenie listy plik\u00F3w:"
::msgcat::mcset pl "&Previous" "&Poprzedni"
::msgcat::mcset pl "&Next" "&Nast\u0119pny"
::msgcat::mcset pl "Scipad search results" "Wyniki wyszukiwania Scipad-a"


# helps.tcl
::msgcat::mcset pl "About" "O autorach"
::msgcat::mcset pl "aboutme_message"\
                   "\n Program zapocz\u0105tkowany przez\
                   Joseph Acosta, joeja@mindspring.com.\n Zmodyfikowany\
                   przez Konsorcjum Scilab.\n Skorygowany przez Enrico Segre\
                   2003-2005.\n R\u00F3\u017Cnorodne ulepszenia,\
                   narz\u0119dzia debuguj\u0105ce, zosta\u0142y stworzone\
                   przez Francois Vogel 2004-2005.\n Autorem szwedzkiej\
                   lokalizacji jest Lindholm 2005.\n Norweskej lokalizacji\
                   jest Torbj\u0159rn Pettersen 2005.\n Niemieckiej lokalizacji\
                   jest Dirk Reusch 2005.\n Polskiej lokalizacji jest\
                   Bartosz Kosiorek 2005."


# infomessages.tcl
::msgcat::mcset pl "Line:" "Linia:"
::msgcat::mcset pl "Column:" "Kolumna:"
::msgcat::mcset pl "Line" "Linia"
::msgcat::mcset pl "in" "do"
::msgcat::mcset pl "Logical line:" "Linia logiczna:"
::msgcat::mcset pl " \[ReadOnly\]" " \[Tylko do odczytu\]"
::msgcat::mcset pl " (modified)" " (zmodyfikowany)"


# inputtext.tcl
::msgcat::mcset pl "No corresponding <" "Nie znaleziono odpowiednich dla <"
::msgcat::mcset pl "> found!" "> !"
::msgcat::mcset pl "Code editing is not allowed during debug!"\
                   "Edycja kodu podczas odpluskwiania jest niedozwolone!"


# menues.tcl
::msgcat::mcset pl "&File" "&Plik"
::msgcat::mcset pl "&Edit" "&Edycja"
::msgcat::mcset pl "&Search" "&Szukaj"
::msgcat::mcset pl "E&xecute" "&Wykonaj"
::msgcat::mcset pl "&Debug" "O&dpluskwianie"
::msgcat::mcset pl "S&cheme" "Sc&hemat"
::msgcat::mcset pl "&Options" "&Opcje"
::msgcat::mcset pl "&Windows" "O&kna"
::msgcat::mcset pl "&Help" "Pomo&c"

# menues.tcl - File menu
::msgcat::mcset pl "&New" "&Nowy"
::msgcat::mcset pl "&Open..." "&Otw\u00F3rz..."
::msgcat::mcset pl "Open &in new" "O&tw\u00F3rz w nowym"
::msgcat::mcset pl "&horizontal tile" "&pionowym oknie"
::msgcat::mcset pl "&vertical tile" "p&oziomym oknie"
::msgcat::mcset pl "Op&en source of..." "Otw\u00F3&rz \u017Ar\u00F3d\u0142o..."
::msgcat::mcset pl "&Save" "Zapi&sz"
::msgcat::mcset pl "Save &as..." "Z&apisz jako..."
::msgcat::mcset pl "&Revert..." "&Przywr\u00F3\u0107..."
::msgcat::mcset pl "Import &Matlab file..." "Importuj plik &Matlab-a..."
::msgcat::mcset pl "Create help s&keleton..." "Utw\u00F3rz sz&kielet pomocy..."
::msgcat::mcset pl "Compile as &help page" "Kompi&luj jako strona pomocy"
::msgcat::mcset pl "Open &function source"\
                   "Otw\u00F3rz \u017Ar\u00F3d\u0142o &funkcji"
::msgcat::mcset pl "Print Se&tup" "&Ustawienia drukowania"
::msgcat::mcset pl "&Print" "&Drukuj"
::msgcat::mcset pl "E&xit" "Zako\u0144&cz"

# menues.tcl - Edit menu
::msgcat::mcset pl "&Undo" "&Cofnij"
::msgcat::mcset pl "&Redo" "&Powt\u00F3rz"
::msgcat::mcset pl "Cu&t" "&Wytnij"
::msgcat::mcset pl "&Copy" "&Kopiuj"
::msgcat::mcset pl "&Paste" "Wkl&ej"
::msgcat::mcset pl "&Delete" "&Usu\u0144"
::msgcat::mcset pl "Select &All" "Z&aznacz wszystko"
::msgcat::mcset pl "Insert &Time/Date" "Wstaw &czas/dat\u0119"
::msgcat::mcset pl "Co&mment selection" "Dodaj ko&mentarz do zaznaczenia"
::msgcat::mcset pl "U&ncomment selection" "Usu\u0144 kome&ntarz z zaznaczenia"
::msgcat::mcset pl "&Indent selection"\
                   "Pow&i\u0119ksz wci\u0119cie zaznaczenia"
::msgcat::mcset pl "Unin&dent selection" "&Zmniejsz wci\u0119cie zaznaczenia"

# menues.tcl - Search menu
::msgcat::mcset pl "&Find..." "&Znajd\u017A..."
::msgcat::mcset pl "Find &Next" "Z&najd\u017A nast\u0119pne"
::msgcat::mcset pl "&Replace..." "Z&amie\u0144..."
::msgcat::mcset pl "&Goto Line..." "&Id\u017A do linii..."

# menues.tcl - Exec menu
::msgcat::mcset pl "&Load into Scilab" "&Za\u0142aduj do Scilab-a"
::msgcat::mcset pl "&Evaluate selection" "&Uruchom zaznaczenie"

# menues.tcl - Debug menu
::msgcat::mcset pl "&Insert/Remove breakpoint"\
                   "&Dodaj/usu\u0144 pu\u0142apk\u0119"
::msgcat::mcset pl "Remove &all breakpoints"\
                   "&Usu\u0144 wszystkie pu\u0142apki"
::msgcat::mcset pl "&Configure execution..." "&Konfiguruj wykonanie..."
::msgcat::mcset pl "Go to next b&reakpoint"\
                   "Id\u017A do nast\u0119pnej &pu\u0142apki"
::msgcat::mcset pl "&Step by step" "K&rok po kroku"
::msgcat::mcset pl "Step &into" "&Wskocz do"
::msgcat::mcset pl "Step o&ver" "&Przeskocz"
::msgcat::mcset pl "Step &out" "Wy&skocz"
::msgcat::mcset pl "Run to c&ursor" "Uruchom do kur&sora"
::msgcat::mcset pl "G&o on ignoring any breakpoint"\
                   "Kon&tynuuj ignoruj\u0105c wszystkie pu\u0142apki"
::msgcat::mcset pl "Show &watch" "Poka\u017C zmienne &obserwowane"
::msgcat::mcset pl "&Break" "Pr&zerwij"
::msgcat::mcset pl "Cance&l debug" "&Anuluj odpluskwianie"

#    ::msgcat::mcset fr "language scheme" "sch\u00E9ma langage"
# menues.tcl - Scheme menu
::msgcat::mcset pl "S&cilab" "S&cilab"
::msgcat::mcset pl "&XML" "&XML"
::msgcat::mcset pl "&none" "&brak"

# menues.tcl - Options menu
::msgcat::mcset pl "&Font size" "&Rozmiar czcionki"
::msgcat::mcset pl "&micro" "m&ikro"
::msgcat::mcset pl "&small" "&ma\u0142a"
::msgcat::mcset pl "m&edium" "\u015B&rednia"
::msgcat::mcset pl "&large" "&du\u017Ca"
::msgcat::mcset pl "&Colors" "&Kolory"
::msgcat::mcset pl "Colorize \'&strings\'" "K&oloruj \'\u0142a\u0144cuchy\'"
::msgcat::mcset pl "Word &Wrap" "&Zawijaj wiersze"
::msgcat::mcset pl "&Tabs and indentation" "&Tabulator i wci\u0119cia"
::msgcat::mcset pl "Tab inserts &spaces" "Klawisz Tab wstawia &spacje"
::msgcat::mcset pl "&Indentation spaces"\
                   "&Ilo\u015B\u0107 spacji w wci\u0119ciu"
::msgcat::mcset pl "Com&pletion" "&Uko\u0144czenie"
::msgcat::mcset pl "&Tab" "&Tab"
::msgcat::mcset pl "&Control-Tab" "&Control-Tab"
::msgcat::mcset pl "&Alt-Tab" "&Alt-Tab"
::msgcat::mcset pl "&Shift-Tab" "&Shift-Tab"
::msgcat::mcset pl "C&ontrol-Alt-Tab" "C&ontrol-Alt-Tab"
::msgcat::mcset pl "S&hift-Control-Tab" "S&hift-Control-Tab"
::msgcat::mcset pl "Sh&ift-Alt-Tab" "Sh&ift-Alt-Tab"
::msgcat::mcset pl "Shi&ft-Control-Alt-Tab" "Shi&ft-Control-Alt-Tab"
::msgcat::mcset pl "File&names" "&Nazwy plik\u00F3w"
::msgcat::mcset pl "&Full path" "&Pe\u0142na \u015Bcie\u017Cka"
::msgcat::mcset pl "Full path if &ambiguous"\
                   "Pe\u0142n&a \u015Bcie\u017Cka\
                   je\u017Celi jest niejednoznaczna"
::msgcat::mcset pl "&Unambiguous pruned path"\
                   "&Jednoznaczna obci\u0119ta \u015Bcie\u017Cka"
::msgcat::mcset pl "&Recent files" "O&statnie pliki"
::msgcat::mcset pl "&Locale" "&J\u0119zyk"
# locale names (common)
::msgcat::mcset pl "de locale (de)" "Deutsch (de)"
::msgcat::mcset pl "fr locale (fr)" "Fran\u00E7ais (fr)"
::msgcat::mcset pl "eng locale (eng)" "English (eng)"
::msgcat::mcset pl "es locale (es)" "Español (es)"
::msgcat::mcset pl "it locale (it)" "Italiano (it)"
::msgcat::mcset pl "no locale (no)" "Norsk (no)"
::msgcat::mcset pl "pl locale (pl)" "Polski (pl)"
::msgcat::mcset pl "se locale (se)" "Svenska (se)"

# menues.tcl - Windows menu
::msgcat::mcset pl "&Maximize" "&Maksymalizuj"
::msgcat::mcset pl "&Split" "&Podziel poziomo"
::msgcat::mcset pl "S&plit (side by side)" "P&odziel pionowo"
::msgcat::mcset pl "Tile all &vertically" "&Uporz\u0105dkuj poziomo"
::msgcat::mcset pl "Tile all &horizontally" "Upo&rz\u0105dkuj pionowo"
::msgcat::mcset pl "Space sashes &evenly" "&Wyr\u00F3wnaj"
::msgcat::mcset pl "Opened Files" "Otwarte pliki"

# menues.tcl - Help menu
::msgcat::mcset pl "&Help..." "&Pomoc..."
::msgcat::mcset pl "&What's?..." "&Co to jest?..."
::msgcat::mcset pl "&About" "&O programie"


# popupmenus.tcl
::msgcat::mcset pl "Open the source of" "Otw\u00F3rz \u017Ar\u00F3d\u0142o dla"
::msgcat::mcset pl "AddWatch" "Dodaj %s jako obserwowan\u0105 zmienn\u0105"


# print.tcl
::msgcat::mcset pl "Print Setup" "Ustawienia wydruku"
::msgcat::mcset pl "Print Command: " "Komenda wydruku : "


# scilabexec.tcl
::msgcat::mcset pl "No point in loading an empty file!"\
                   "Pr\u00F3bujesz wczyta\u0107 pusty plik!"
::msgcat::mcset pl "Scilab is working, wait for the prompt to load file"\
                   "Scilab pracuje, poczekaj na znak\
                   zach\u0119ty, aby wczyta\u0107 plik"
::msgcat::mcset pl "Exec done" "Wykonanie zako\u0144czone"
::msgcat::mcset pl "Scilab is working, wait for the\
                   prompt to execute the selection."\
                   "Scilab pracuje, poczekaj na znak\
                   zach\u0119ty, aby wykona\u0107 zaznaczenie."
::msgcat::mcset pl "Scilab is working, wait for the\
                   prompt to convert a Matlab file."\
                   "Scilab pracuje, poczekaj na znak\
                   zach\u0119ty, aby przekonwerterowa\u0107 plik Matlab."
::msgcat::mcset pl "Matlab files" "Pliki Matlab-a"
::msgcat::mcset pl "Matlab file to convert" "Plik Matlab-a do konwersji"
::msgcat::mcset pl "Scilab is converting, please hold on..."\
                   "Scilab teraz konwertuje, prosz\u0119 czeka\u0107..."
::msgcat::mcset pl "Matlab file import" "Importowanie pliku programu Matlab"
::msgcat::mcset pl "Conversion of the file failed, see\
                   the Scilab window for details -- Perhaps report the error\
                   text and the offending Matlab file to \
                   http://scilabsoft.inria.fr/bugzilla_bug_II/index.cgi"\
                   "Konwersja pliku nie powiod\u0142a\
                   si\u0119, zobacz okno Scilab-a aby uzyska\u0107\
                   szczeg\u00F3\u0142y -- Spr\u00F3buj wys\u0142a\u0107\
                   tekst z b\u0142\u0119dem oraz niedzia\u0142aj\u0105cy\
                   plik Matlab-a na stron\u0119\
                   http://scilabsoft.inria.fr/bugzilla_bug_II/index.cgi"
::msgcat::mcset pl "Please select destination path for\
                   the xml source of the help file"\
                   "Wska\u017C \u015Bcie\u017Ck\u0119\
                   docelow\u0105, \u017Ar\u00F3d\u0142a xml pliku pomocy"
::msgcat::mcset pl "Older version of" "Znaleziono starsz\u0105 wersj\u0119"
::msgcat::mcset pl "found!" "!"
::msgcat::mcset pl "An old version of" "Starsza wersja pliku"
::msgcat::mcset pl "already exists: open the old file instead?"\
                   "ju\u017C istnieje: otworzy\u0107 stary plik ?"
::msgcat::mcset pl "Existing file" "Istniej\u0105cy plik"
::msgcat::mcset pl "New skeleton" "Nowy szkielet"
::msgcat::mcset pl "Scilab is working, wait for the\
                   prompt to compile the help file."\
                   "Scilab pracuje, poczekaj na znak\
                   zach\u0119ty, aby skompilowa\u0107 plik pomocy."
::msgcat::mcset pl "ScilabEval command cannot be passed to Scilab!"\
                   "Polecenie ScilabEval nie mog\u0142o\
                   by\u0107 przetworzone przez program Scilab!"
::msgcat::mcset pl "impossibleScilabEval_message"\
                   "UWAGA : Polecenie nie mo\u017Ce\
                   zosta\u0107 wykonane przez program Scilab i zostanie\
                   zignorowane.\n\n  Prawdopodobnie nie masz uprawnie\u0144\
                   zapisu do katalogu TMPDIR Scilab-a.  Mog\u0105\
                   wyst\u0105pi\u0107 trudne do przewidzenia rezultaty.\n\n \
                   Wadliwe polecenie ma posta\u0107:\n\n-->"


# scipad.tcl
::msgcat::mcset pl "NOT CONNECTED TO SCILAB"\
                   "NIE PO\u0142\u0105CZONO ZE SCILAB-em"


# textarea.tcl
::msgcat::mcset pl "Font size" "Rozmiar czcionki"


# whichfun.tcl
::msgcat::mcset pl "Being at line" "Jeste\u015B w linii"
::msgcat::mcset pl ", function" ", funkcja"
::msgcat::mcset pl "begins at" "zaczyna si\u0119"
::msgcat::mcset pl ", and there are" ", i jest tu"
::msgcat::mcset pl "continued lines, i.e. we are at line"\
                   "ci\u0105g\u0142ych linii, tj. jeste\u015Bmy w linii"
::msgcat::mcset pl "of" "na"
::msgcat::mcset pl "Function" "Funkcja"
::msgcat::mcset pl "line" "linia"
::msgcat::mcset pl "The cursor is not currently inside a function body."\
                   "Kursor nie znajduje si\u0119 w ciele funkcji."
::msgcat::mcset pl "Which function?" "Kt\u00F3ra funkcja?"


#preference variables description
::msgcat::mcset pl "BGCOLOR" "T\u0142a"
::msgcat::mcset pl "FGCOLOR" "Tekstu"
::msgcat::mcset pl "CURCOLOR" "Kursora"
::msgcat::mcset pl "PARCOLOR" "Nawiasu okr\u0105g\u0142ego ()"
::msgcat::mcset pl "BRAKCOLOR" "Nawiasu kwadratowego \\\[\\\]"
::msgcat::mcset pl "BRACCOLOR" "Klamry {}"
::msgcat::mcset pl "PUNCOLOR" "Interpunkcji"
::msgcat::mcset pl "INTFCOLOR" "Funkcje wewn\u0119trzne"
::msgcat::mcset pl "COMMCOLOR" "Polecenia Scilab-a"
::msgcat::mcset pl "SCICCOLOR" "Funkcje Scicos"
::msgcat::mcset pl "OPCOLOR" "Operator\u00F3w"
::msgcat::mcset pl "QTXTCOLOR" "\u0142a\u0144cucha znak\u00F3w"
::msgcat::mcset pl "REMCOLOR" "Komentarzy"
::msgcat::mcset pl "XMLCOLOR" "Tag\u00F3w XML"
::msgcat::mcset pl "NUMCOLOR" "Liczb"
::msgcat::mcset pl "SELCOLOR" "Zaznaczenia"
::msgcat::mcset pl "BREAKPOINTCOLOR" "Pu\u0142apki"
::msgcat::mcset pl "LFUNCOLOR" "Funkcji bibliotecznych"
::msgcat::mcset pl "PDEFCOLOR" "Predefiniowanych zmiennych"
::msgcat::mcset pl "FOUNDTEXTCOLOR" "Znalezionego tekstu"


#overriding tcl-core dialogs (many are actually already found in
#  TKDIR/msgs/fr.msg)
::msgcat::mcset pl "&Directory:" "&Katalog :"
::msgcat::mcset pl "File &name:" "&Nazwa pliku :"
::msgcat::mcset pl "Files of &type:" "Pliki &typu :"
#::msgcat::mcset fr "&Save" "&Enregistrer" # already included
::msgcat::mcset pl "&Open" "&Otw\u00F3rz"
::msgcat::mcset pl "Open" "Otw\u00F3rz"
::msgcat::mcset pl "Save As" "Zapisz jako"
::msgcat::mcset pl "&Cancel" "&Anuluj"
::msgcat::mcset pl "&Selection:" "&Zaznaczenie :"
::msgcat::mcset pl "&Yes" "&Tak"
::msgcat::mcset pl "&No" "&Nie"
::msgcat::mcset pl "&Red" "&Czerwony"
::msgcat::mcset pl "&Green" "&Zielony"
::msgcat::mcset pl "&Blue" "&Niebieski"
::msgcat::mcset pl "Directory \"%1\$s\" does not exist."\
                   "Katalog \"%1\$s\" nie istnieje."
