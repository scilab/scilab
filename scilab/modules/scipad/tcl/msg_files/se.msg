#  Scipad - programmer's editor and debugger for Scilab
#
#  Copyright (C) 2002 -      INRIA, Matthieu Philippe
#  Copyright (C) 2003-2006 - Weizmann Institute of Science, Enrico Segre
#  Copyright (C) 2004-2008 - Francois Vogel
#
#  Localization files ( in tcl/msg_files/) are copyright of the 
#  individual authors, listed in the header of each file
#
# This program is free software; you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation; either version 2 of the License, or
# (at your option) any later version.
#
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with this program; if not, write to the Free Software
# Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
#
# See the file scipad/license.txt
#
# Scipad texts in Swedish
# in sync at least with v5.22
# provided by Gunnar Lindholm -- A new translator is sought!

# commons
::msgcat::mcset se "Cancel" "Avbryt"
::msgcat::mcset se "Cance&l" "Avbry&t"
::msgcat::mcset se "OK" "OK"
::msgcat::mcset se "Scilab working" "Scilab arbetar"
::msgcat::mcset se "All files" "Alla filer"
::msgcat::mcset se "Add/Change" "Lägg till/Ändra"
::msgcat::mcset se "Add/Chan&ge" "&Lägg till/Ändra"
::msgcat::mcset se "&Remove" "&Tabort"
::msgcat::mcset se "Untitled" "Namnlös"
::msgcat::mcset se "Close" "Stäng"
::msgcat::mcset se "&Close" "&Stäng"


# buffernavigation.tcl
::msgcat::mcset se "Goto Line?" "Gå till rad ?"
::msgcat::mcset se "Go to:" "Gå till  : "
::msgcat::mcset se "&logical line" "logisk rad"
::msgcat::mcset se "&physical line" "fysisk rad"
::msgcat::mcset se "in &function" "i funktionen"
::msgcat::mcset se "in &current file" "i aktuella filen"
::msgcat::mcset se "Outside of function definition" "Utanför funktionsdefinitionen"

# colorize.tcl
::msgcat::mcset se "Wait seconds while recolorizing file" \
                   "Vänta ett ögonblick medans filen färgläggs"


# db_argsmanagement.tcl
::msgcat::mcset se "Variable:" "Variabel :"
::msgcat::mcset se "Value:" "Värde :"
::msgcat::mcset se "&Get current value from Scilab" "Hämta &aktuellt värde från Scilab"


# db_configureexec.tcl
::msgcat::mcset se "Configure execution" "Konfigurera exekveringen"
::msgcat::mcset se "Function name:" "Funktionsnamn :"
::msgcat::mcset se "&Obtain" "&Få"
::msgcat::mcset se "Input arguments:" "Indata argument :"
::msgcat::mcset se "Function name or input arguments do not match definition of the function" \
                    "Funktionsnamnet eller indata argumenten matchar inte definitionen av funktionen"
    ::msgcat::mcset se "in the file!\n\nCheck function\
                        name and arguments (names, order) in the configuration dialog.\
                        \nArguments order can be changed using drag and drop with\
                        right mouse button in the arguments listbox." \
                       "i filen!\n\n\
                        Kontrollera funktionsnamnet och argumenten (namn, ordning)\
                        i konfigurationsdialogen.\n\
                        Ordningen på argumenten kan ändras genom dra och släppa med\
                        höger musknapp i rutan med argument."
::msgcat::mcset se "Error in selected function name or arguments" \
                   "Fel i valt funktionsnamn eller argument"
::msgcat::mcset se "Debugger is not yet developed for sce scripts!" \
                    "Det är ingen debugger utvecklad för sce script!"


# db_debugsession.tcl
::msgcat::mcset se "Scilab is working, wait for the prompt to issue a debugger command." \
         "Scilab arbetar, vänta på att prompten ger ett debugg kommando."
::msgcat::mcset se "Cursor position is out of reach!" \
                   "Markören är utom räckhåll!"
::msgcat::mcset se "Cursor must be in a function" \
                   "Markören måste vara i en funktion"
::msgcat::mcset se "No effect - The debugged file is not stuck" \
                   "Verkningslöst - den debuggade filen har inte fastnat"
::msgcat::mcset se "Scilab execution error" "Exekveringsfel i Scilab"
::msgcat::mcset se "The shell reported an error while trying to execute " \
                   "Skalet rapporterade ett fel när det försökte exekvera "
    ::msgcat::mcset se "at line " "på rad "
    ::msgcat::mcset se " of " " i "
::msgcat::mcset se "Execution aborted!" "Exekveringen avbrute!"


# db_init.tcl
::msgcat::mcset se "Please wait..." "Vänta är du snäll..."


# db_states.tcl
::msgcat::mcset se "Currently no debug session" \
                   "Finns ingen debuggsession för tillfället"
::msgcat::mcset se "Ready to start debug" "Redo att starta debugging"
::msgcat::mcset se "Debug in progress" "Debuggning pågår"


# db_watch.tcl
::msgcat::mcset se "Watch" "Bevakning"
::msgcat::mcset se "Watch variables:" "Iakta variabler :"
::msgcat::mcset se "Call stack:" "Anropsstack:"
::msgcat::mcset se "Hide watch &variables" "Dölj bevakade &variabler"
::msgcat::mcset se "Hide call &stack" "Dölj &anropsstacken"
::msgcat::mcset se "Undefined elements are not legal in variable" "Odefinierade element är inte giltiga i variabel"
::msgcat::mcset se ".\nThis variable will not be updated in Scilab."\
                   ".\nDenna variabel kommer ej uppdateras i Scilab"
::msgcat::mcset se "Illegal undefined element found" "Ogiltigt odefinierat element funnet"

# filecommands.tcl
::msgcat::mcset se "New script" "Nytt script"
::msgcat::mcset se "The contents of" \
                 "Innehållet kan ha ändrats, vill du spara dina ändringar"
    ::msgcat::mcset se "may have changed, do you wish to save your changes?" \
                       "?"
::msgcat::mcset se "Save Confirm?" "Bekräfta spara ?"
::msgcat::mcset se "Weird drag&drop" "Konstig drag och släpp"
::msgcat::mcset se "is neither a valid filename nor a valid directory.\
                    Either you're dragging an object of another type, or\
                    you hit a bug of the dnd mechanism." \
                    "är varken ett giltigt filnamn eller bibliotek.\
                     Antingen drar du ett objekt av en annan sort, eller \
                     är det fel i drag och släpp funktionaliteten."
::msgcat::mcset se "File not found" "Filen finns inte"
::msgcat::mcset se "does not exist anymore. Do you want to create an\
                    empty file with the same name?" \
                   "finns inte längre. Vill du skapa en tom fil med samma namn?"
::msgcat::mcset se "Open file" "Öppna fil"
::msgcat::mcset se "The file" "Filen"
    ::msgcat::mcset se "is already opened! Save the current opened file to another name and reopen it from disk!" \
                       "är redan öppen! Spara den öppna filen till ett annat filnamn och öppna den igen!"
::msgcat::mcset se "File" "Fil"
    ::msgcat::mcset se "saved" "sparad"
::msgcat::mcset se "cannot be written!" "kan inte skrivas!"
::msgcat::mcset se "The contents of " "Innehållet i"
    ::msgcat::mcset se "has changed on disk, save it anyway?" \
                       "har ändrats på hårddisken, spara det i allafall?"
::msgcat::mcset se "File has changed!" "Filen har ändrats!"
::msgcat::mcset se "Save as" "Spara som"
::msgcat::mcset se "Scilab files" "Scilab filer"
::msgcat::mcset se "XML files" "XML filer"
::msgcat::mcset se "Wait seconds while loading and colorizing file" \
                   "Vänta ett ögonblick medan filen laddas och färgläggs"
::msgcat::mcset se "The contents of this file may have changed, do you wish to save your changes?" \
                   "Innehållet i denna fil kan ha ändrats, will du spara dina ändringar?"
::msgcat::mcset se "Revert" "Återgå till tidigare version"
    ::msgcat::mcset se "to saved?" "?"
::msgcat::mcset se "Revert Confirm?" "Bekräfta återgång ?"


# findreplace.tcl
::msgcat::mcset se "The string" "Strängen"
    ::msgcat::mcset se "could not be found in" "kan inte hittas"
::msgcat::mcset se "replacements done" "ersättningar utförda"
::msgcat::mcset se "Find" "Finn"
::msgcat::mcset se "No match found in the selection for" \
                   "Ingen träff i den markerade delen för"
    ::msgcat::mcset se "\nWould you like to look for it in the entire text?" \
                       "\nVill du leta efter det i hela texten?"
::msgcat::mcset se "You are searching for an empty string!" \
                       "Du letar efter en tom sträng, toker!"
::msgcat::mcset se "Re&place" "&Ersätt"
::msgcat::mcset se "Find what:" "Leta efter :"
::msgcat::mcset se "Replace with:" "Ersätt med :"
::msgcat::mcset se "Find &Next" "&Hitta nästa"
::msgcat::mcset se "Replace &All" "Ersätt &alla"
::msgcat::mcset se "&Upwards" "&Uppåt"
::msgcat::mcset se "&Downwards" "&Neråt" 
::msgcat::mcset se "Match &case" "&Skilj på gemener och versaler"
::msgcat::mcset se "&Regular expression" "&Reguljärt uttryck"


# helps.tcl
::msgcat::mcset se "About" "Om"
::msgcat::mcset se aboutme_message \
                   "\n Skapat av Joseph Acosta, joeja@mindspring.com.\n\
                    Modifierat av Scilab Constortium.\n\
                    Granskat av Enrico Segre 2003-2006.\n\
                    Diverse förbättringar, i synnerhet debuggingverktygen av François Vogel 2004-2008.\n\
                    Översättning till svenska av Gunnar Lindholm 2005.\n\
                    Översättning till tyska av Dirk Reusch 2005."


# infomessages.tcl
::msgcat::mcset se "Line:" "Rd:"
    ::msgcat::mcset se "Column:" "Kolumn:"
::msgcat::mcset se "Line" "Rad"
    ::msgcat::mcset se "in" "i"
::msgcat::mcset se " \[ReadOnly\]" " \[Skrivskyddad\]"
::msgcat::mcset se " (modified)" " (ändrad)"
::msgcat::mcset se "Logical line:" "Logisk rad:"

# inputtext.tcl
::msgcat::mcset se "No corresponding <" "Ingen motsvarande <"
    ::msgcat::mcset se "> found!" "> hittad !"
::msgcat::mcset se "Code editing is not allowed during debug!" \
                    "Du kan inte ändra koden under debuggningen!"


# menues.tcl
::msgcat::mcset se "&File" "&Fil"
::msgcat::mcset se "&Edit" "R&edigera"
::msgcat::mcset se "&Search" "&Sök och ersätt"
::msgcat::mcset se "E&xecute" "E&xekvera"
::msgcat::mcset se "&Debug" "&Debugga"
::msgcat::mcset se "S&cheme" "Spr&åk"
::msgcat::mcset se "&Options" "Inställningar"
::msgcat::mcset se "&Windows" "Fönster"
::msgcat::mcset se "&Help" "&Hjälp"

    # menues.tcl - File menu
    ::msgcat::mcset se "&New" "&Ny"
    ::msgcat::mcset se "&Open..." "Öppna..."
    ::msgcat::mcset se "&Save" "&Spara"
    ::msgcat::mcset se "Save &as..." "Spar&a som..."
    ::msgcat::mcset se "&Revert..." "Åte&rgå..."
    ::msgcat::mcset se "Import &Matlab file..."  "Importera &Matlab fil..."
    ::msgcat::mcset se "Create help s&keleton..." "S&kapa hjälpmall..."
    ::msgcat::mcset se "Compile as &help page" "Kompilera som &hjälpfil"
    ::msgcat::mcset se "Open &function source" "Öppna &funktions källkod"
    ::msgcat::mcset se "Print Se&tup" "Skrivar ins&tällningar"
    ::msgcat::mcset se "&Print" "Skriv ut"
    ::msgcat::mcset se "&Close" "Stäng"
    ::msgcat::mcset se "E&xit" "Avsluta"

    # menues.tcl - Edit menu
    ::msgcat::mcset se "&Undo" "Ångra"
    ::msgcat::mcset se "&Redo" "Upp&repa"
    ::msgcat::mcset se "Cu&t" "Klipp u&t"
    ::msgcat::mcset se "&Copy" "Kopiera"
    ::msgcat::mcset se "&Paste" "Klistra in"
    ::msgcat::mcset se "&Delete" "Ta bort"
    ::msgcat::mcset se "Select &All" "Markera &Allt"
    ::msgcat::mcset se "Insert &Time/Date" "Infoga datum och &Tid"
    ::msgcat::mcset se "Co&mment selection" "Ko&mmentera markering"
    ::msgcat::mcset se "U&ncomment selection" "A&vkommentera markeringen"
    ::msgcat::mcset se "&Indent selection" "&Indentera markeringen"
    ::msgcat::mcset se "Unin&dent selection" "Oin&dentera markeringen"

    # menues.tcl - Search menu
    ::msgcat::mcset se "&Find..." "&Finn..."
    ::msgcat::mcset se "Find &Next" "&Hitta Nästa"
    ::msgcat::mcset se "&Replace..." "E&rsätt..."
    ::msgcat::mcset se "&Goto Line..." "&Gå till rad..."

    # menues.tcl - Exec menu
    ::msgcat::mcset se "&Load into Scilab" "&Ladda in i Scilab"
    ::msgcat::mcset se "&Evaluate selection" "&Evaluera markering"

    # menues.tcl - Debug menu
    ::msgcat::mcset se "&Insert/Remove breakpoint" \
                       "Lägg t&ill tabort brytpunkt"
    ::msgcat::mcset se "Remove &all breakpoints" "&Tabort alla brytpunkter"
    ::msgcat::mcset se "&Configure execution..." "Konfigurera exekveringen..."
    ::msgcat::mcset se "Go to next b&reakpoint" "Gå till nästa b&rytpunkt"
    ::msgcat::mcset se "&Step by step" "S&teg för steg"
    ::msgcat::mcset se "Step &into" "Kliv &in i"
    ::msgcat::mcset se "Step o&ver" "Kliv &över"
    ::msgcat::mcset se "Step &out" "Kliv ut &ur"
    ::msgcat::mcset se "Run to c&ursor" "Exekvera till markering"
    ::msgcat::mcset se "G&o on ignoring any breakpoint" \
                       "&Fortsätt och strunta i brytpunkter"
    ::msgcat::mcset se "Show &watch" "&Visa bevakning"
    ::msgcat::mcset se "&Break" "&Stanna"
    ::msgcat::mcset se "Cance&l debug" "&Avbryt debugging"

#    ::msgcat::mcset se "language scheme" "Språk"
    # menues.tcl - Scheme menu
    ::msgcat::mcset se "S&cilab" "S&cilab"
    ::msgcat::mcset se "&XML" "&XML"
    ::msgcat::mcset se "&none" "&inget"

    # menues.tcl - Options menu
    ::msgcat::mcset se "&Font size" "&Teckenstorlek"
        ::msgcat::mcset se "&micro" "&mikro"
        ::msgcat::mcset se "&small" "&liten"
        ::msgcat::mcset se "m&edium" "lago&m"
        ::msgcat::mcset se "&large" "&stor"
    ::msgcat::mcset se "&Colors" "&Färger"
    ::msgcat::mcset se "&Tabs and indentation" "Tabbar och &indentering"
        ::msgcat::mcset se "Tab inserts &spaces" "Tab blir mellanslag"
        ::msgcat::mcset se "&Indentation spaces" "&Indragsutrymme"
    ::msgcat::mcset se "Colorize \'&strings\'" "Färga \'&strängar\'"
    ::msgcat::mcset se "Word &wrap" "&Bryt ej långa rader"
    ::msgcat::mcset se "File&names" "Fil&namn"
        ::msgcat::mcset se "&Full path" "&Hela sökvägen"
        ::msgcat::mcset se "Full path if &ambiguous"\
                           "Hela sökvägen är tvetydig"
        ::msgcat::mcset se "&Unambiguous pruned path" "&Entydig sökväg"
    ::msgcat::mcset se "&Recent files" "S&enaste filerna"
    ::msgcat::mcset se "&Locale" "&Språk"

    # menues.tcl - Windows menu
    ::msgcat::mcset se "Opened Files" "Öppna filer"
	::msgcat::mcset se "Tile &horizontally" "Pannorera &horisontelt"
    ::msgcat::mcset se "Tile &vertically" "Pannorera &vertikalt"
    ::msgcat::mcset se "&Maximize" "&Maximera"

    # menues.tcl - Help menu
    ::msgcat::mcset se "&Help..." "&Hjälp..."
    ::msgcat::mcset se "&What's?..." "&Vad är ?..."
    ::msgcat::mcset se "&About" "&Om"


# popupmenus.tcl
::msgcat::mcset se "Open the source of" "Öppna källkoden till"
::msgcat::mcset se "AddWatch" "Bevaka %s"


# print.tcl
::msgcat::mcset se "Print Setup" "Skrivar inställningar"
::msgcat::mcset se "Print Command: " "Utskriftskommando : "


# scilabexec.tcl
::msgcat::mcset se "No point in loading an empty file!" \
                   "Ingen ide att ladda en tom fil!"
::msgcat::mcset se "Scilab is working, wait for the prompt to load file"\
                   "Scilab arbetar, vänta på att filen laddas"
::msgcat::mcset se "Exec done" "Exekveringen klar"
::msgcat::mcset se "Scilab is working, wait for the prompt to execute the selection."\
                   "Scilab arbetar, vänta på att markeringen exekveraas."
::msgcat::mcset se "Scilab is working, wait for the prompt to convert a Matlab file."\
                   "Scilab arbetar, vänta på att Matlab filen konverteras."
::msgcat::mcset se "Matlab files" "Matlab filer"
::msgcat::mcset se "Matlab file to convert" "Konvertera denna Matlab fil"
::msgcat::mcset se "Scilab is converting, please hold on..." \
                   "Scilab konverterar, vänligen vänta..."
::msgcat::mcset se "Matlab file import" "Importera Matlab fil"
::msgcat::mcset se "Conversion of the file failed, see the Scilab window\
                    for details -- Perhaps report the error text and the\
                    offending Matlab file to" \
                   "Konverteringen av filen misslyckades, titta i Scilab fönstret\
                    för detaljer. Kanske skall du rapportera felmeddelandet och\
					den krånglande Matlab filen till"
::msgcat::mcset se "Please select destination path for the xml source of the help file"\
                   "Vänligen välj xml koden för hjälp filen"
::msgcat::mcset se "Older version of" "Äldre version av"
    ::msgcat::mcset se "found!" "funnen!"
::msgcat::mcset se "An old version of" "En gammal version av"
    ::msgcat::mcset se "already exists: open the old file instead?"\
                        "finns redan: Öppna den gammla filen istället?"
::msgcat::mcset se "Existing file" "Filen finns"
::msgcat::mcset se "New skeleton" "Ny mall"
::msgcat::mcset se "Scilab is working, wait for the prompt to compile the help file."\
                   "Scilab arbetar, vänta på att hjälpfilen ska kompileras."
::msgcat::mcset se "ScilabEval command cannot be passed to Scilab!" "ScilabEval kommando kan inte ges till Scilab!"


# scipad.tcl
::msgcat::mcset se "NOT CONNECTED TO SCILAB" "INTE ANSLUTEN TILL SCILAB"


# textarea.tcl
::msgcat::mcset se "Font size" "Teckenstorlek"


# whichfun.tcl
::msgcat::mcset se "Being at line" "Är på rad"
    ::msgcat::mcset se ", function" ", funktionen"
    ::msgcat::mcset se "begins at" "börjar på"
    ::msgcat::mcset se ", and there are" ", och det är "
    ::msgcat::mcset se "continued lines, i.e. we are at line"\
            "flera rader, d.v.s. vi är på rad"
    ::msgcat::mcset se "of" "i"
::msgcat::mcset se "Function" "Funktion"
    ::msgcat::mcset se "line" "rad"
::msgcat::mcset se "The cursor is not currently inside a function body."\
               "Markören är förnärvarande inte inuti en funktionskropp."
::msgcat::mcset se "Which function?" "Vilken funktion?"


#preference variables description
::msgcat::mcset se "BGCOLOR" "Bakgrundsfärg"
::msgcat::mcset se "FGCOLOR" "Textfärg"
::msgcat::mcset se "CURCOLOR" "Markör"
::msgcat::mcset se "PARCOLOR" "Parenteser ()"
::msgcat::mcset se "BRAKCOLOR" "Hakparenteser \\\[\\\]"
::msgcat::mcset se "BRACCOLOR" "Krullparenteser {}"
::msgcat::mcset se "PUNCOLOR"  "Punkter"
::msgcat::mcset se "KEYWCOLOR" "Reserverade ord"
::msgcat::mcset se "OPCOLOR"   "Operatorer"
::msgcat::mcset se "QTXTCOLOR"  "Texter"
::msgcat::mcset se "REMCOLOR" "Kommentarer"
::msgcat::mcset se "XMLCOLOR" "Balises XML"
::msgcat::mcset se "NUMCOLOR" "Tal"
::msgcat::mcset se "SELCOLOR" "Markering"
::msgcat::mcset se "BREAKPOINTCOLOR" "Brytpunktsfärg"
::msgcat::mcset se "LFUNCOLOR" "Biblioteksrutiner"
::msgcat::mcset se "PDEFCOLOR" "Fördefinierade variabler"
::msgcat::mcset se "FOUNDTEXTCOLOR" "Funnen text"


#overriding tcl-core dialogs (many are actually already found in 
#  TKDIR/msgs/se.msg)
::msgcat::mcset se "&Directory:" "&Katalog :"
::msgcat::mcset se "File &name:" "Fil&namn :"
::msgcat::mcset se "Files of &type:" "Filer av &typen :"
#::msgcat::mcset se "&Save" "&Enregistrer" # already included
::msgcat::mcset se "&Open" "&Öppna"
::msgcat::mcset se "Open" "Öppna"
::msgcat::mcset se "Save As" "Spara som"
::msgcat::mcset se "&Cancel" "&Avbryt"
::msgcat::mcset se "&Selection:" "&Slektion :"
::msgcat::mcset se "&Yes" "&Ja"
::msgcat::mcset se "&No" "&Nej"
::msgcat::mcset se "&Red" "&Röd"
::msgcat::mcset se "&Green" "&Grön"
::msgcat::mcset se "&Blue" "&Blå"
::msgcat::mcset se "Directory \"%1\$s\" does not exist."\
        "Katalogen \"%1\$s\" finns ej."
