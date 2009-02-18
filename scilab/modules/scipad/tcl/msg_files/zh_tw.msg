#  Scipad - programmer's editor and debugger for Scilab
#
#  Copyright (C) 2002 -      INRIA, Matthieu Philippe
#  Copyright (C) 2003-2006 - Weizmann Institute of Science, Enrico Segre
#  Copyright (C) 2004-2009 - Francois Vogel
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

# Scipad message for chinese-traditional
# Scipad 中文訊息檔，zh_tw.msg 及 zh_cn.msg
# zh_cn.msg 內容直接由 zh_tw.msg 進行繁體轉簡體
#
# 如果有人願意接手新版訊息翻譯，請與 李運璋 yjlee123@gmail.com 連絡
#

# commons
::msgcat::mcset zh_tw "Cancel" "清除"
::msgcat::mcset zh_tw "Cance&l" "清除(&l)"
::msgcat::mcset zh_tw "OK" "確定"
::msgcat::mcset zh_tw "All files" "所有檔案"
::msgcat::mcset zh_tw "Add/Change" "添加/變更"
::msgcat::mcset zh_tw "Add/Chan&ge" "添加/變更(&g)"
::msgcat::mcset zh_tw "&Remove" "移除(&R)"
::msgcat::mcset zh_tw "Rem&ove" "移除(&o)"
::msgcat::mcset zh_tw "Untitled" "無名"
::msgcat::mcset zh_tw "Close" "關閉"
::msgcat::mcset zh_tw "&Close" "關閉(&C)"
::msgcat::mcset zh_tw "Error " "錯誤 "
::msgcat::mcset zh_tw "The file" "檔案"


# buffernavigation.tcl
::msgcat::mcset zh_tw "Hide" "隱藏"
::msgcat::mcset zh_tw "Close this tile, keep content"\
                      "關閉此區塊，保留內容"
::msgcat::mcset zh_tw "Close this tile" "關閉此區塊"
::msgcat::mcset zh_tw "Goto Line?" "跳至第 .. 行"
::msgcat::mcset zh_tw "Go to:" "跳至: "
::msgcat::mcset zh_tw "&logical line" "邏輯行(&l)"
::msgcat::mcset zh_tw "&physical line" "實體行(&p)"
::msgcat::mcset zh_tw "in &function" "函數內(&f)"
::msgcat::mcset zh_tw "in &current file" "目前檔案(&c)"
::msgcat::mcset zh_tw "Outside of function definition"\
                      "超出函數定義範圍"


# colorize.tcl
::msgcat::mcset zh_tw "Wait seconds while recolorizing file" "等待幾秒.."
::msgcat::mcset zh_tw "You can't do that while colorization is in progress.\
                       Please try again when finished."\
                      "顏色變換中，無法執行此動作，請靜待..."
::msgcat::mcset zh_tw "Command forbidden during colorization"\
                      "顏色變換中，無法執指令"


# completion.tcl
::msgcat::mcset zh_tw "No possible completion" "不可能結束"
::msgcat::mcset zh_tw "possible completions" "可能結束"
::msgcat::mcset zh_tw "Possible completions" "可能結束"


# db_argsmanagement.tcl
::msgcat::mcset zh_tw "Add" "新增"
::msgcat::mcset zh_tw "This variable can be watched but cannot be edited!"\
                      "此變數可檢視，但不能被編輯!"
::msgcat::mcset zh_tw "A new watch variable will be created."\
                      "新增一檢視變數."
::msgcat::mcset zh_tw "Non editable variable" "不可編輯之變數"
::msgcat::mcset zh_tw "Variable:" "變數 :"
::msgcat::mcset zh_tw "Expression:" "算式 :"
::msgcat::mcset zh_tw "Value:" "變數 :"
::msgcat::mcset zh_tw "&Get current value from Scilab"\
                      "由 Scilab 取得目前數值(&G)"
::msgcat::mcset zh_tw "Non watchable variable" "不可檢視之變數"
::msgcat::mcset zh_tw "Sorry, variable" "對不起， 變數"
::msgcat::mcset zh_tw "is special and cannot be watched."\
                      "為特殊變數，無法檢視"


# db_bptsmanagement.tcl
::msgcat::mcset zh_tw "No breakpoint here!" "此處並無停止點 !"


# db_configureexec.tcl
::msgcat::mcset zh_tw "Configure execution" "設定執行組態..."
::msgcat::mcset zh_tw "Function name:" "函數名稱 :"
::msgcat::mcset zh_tw "&Obtain" "得到(&O)"
::msgcat::mcset zh_tw "Input arguments:" "輸入參數 :"
::msgcat::mcset zh_tw "The selected function has not been found in the\
                       opened files, maybe it was deleted since last\
                       configuration of the debugger.\nUse button \""\
                      "所選的函數不在開啟的檔案內，也許在上次偵錯器組態之後刪除此函數，\
                       \n使用按鍵 \""
::msgcat::mcset zh_tw "\" to refresh the list of available functions."\
                      "\" 以得到最新函數清單."
::msgcat::mcset zh_tw "Function name or input arguments do not match\
                       definition of the function"\
                      "函數名稱或輸入參數與定義在"
::msgcat::mcset zh_tw "in the file!\n\nCheck function name and arguments\
                       (names, order) in the configuration dialog.\
                       \nArguments order can be changed using drag and drop\
                       with right mouse button in the arguments listbox."\
                      "檔案內之內容不符!\n\n\
                      請檢查組態對話盒內之函數名稱及輸入參數\n\
                      參數之順序，可以在參數清單按滑鼠右鍵中拖拉以改變."
::msgcat::mcset zh_tw "Error in selected function name or arguments"\
                      "選擇函數名稱或參數時發生錯誤"
::msgcat::mcset zh_tw "At least one input argument does not match definition\
                       of the function"\
                      "最少一個參數與定義在"
::msgcat::mcset zh_tw "in the file!\n\nCheck function arguments (their\
                       names) in the configuration dialog."\
                      "檔案內之內容不符!\n\n\
                       請檢查組態對話盒內之輸入參數."
::msgcat::mcset zh_tw "Error in selected function arguments"\
                      "選擇函數參數時發生錯誤"
::msgcat::mcset zh_tw "Main level code found" "找到最上層程式碼"
::msgcat::mcset zh_tw "Scipad ancillary file" "Scipad 輔助檔"
::msgcat::mcset zh_tw "The current file contains only Scipad ancillaries\
                       that cannot be debugged in Scipad."\
                      "此檔僅為 Scipad 輔助檔，無法在 Scipad\
                       內進行偵錯，請選其它暫存區 (buffer)."
::msgcat::mcset zh_tw "This file contains main level code.\nDebug as a\
                       single .sce file?"\
                      "這個檔案內含主程式碼(mail level code).\n以\
                       單一 .sce 檔進行偵錯?"
::msgcat::mcset zh_tw "No function found" "找不到函數"
::msgcat::mcset zh_tw "This file contains no function and will be treated"\
                      " 此檔未含函數，因此將被視為"
::msgcat::mcset zh_tw "as a single .sce file.\nStart debugging?"\
                      "一個單一 .sce 檔.\n開始偵錯 ?"
::msgcat::mcset zh_tw "You must enable colorization to be able to launch the\
                       debugger!"\
                      "你必須將選色(colorization)功能開啟以啟動偵錯器\
                       !"
::msgcat::mcset zh_tw "The debugger needs colorization"\
                      "偵錯器啟動需要選色(colorization)功能"
::msgcat::mcset zh_tw "Warning!\n\n  A function missing the closing\
                       \"endfunction\" has been found in the currently\
                       opened files.\n  Such syntax is now obsolete.\n\n \
                       Please terminate function "\
                      " 警告 !\n\n \
                       目前開啟的檔案中，發現一個不以\
                       \"endfunction\" 指令結束的函數.\n \
                       這類語法目前版本已不允許.\n\n \
                       請以正確指令結束函數 "
::msgcat::mcset zh_tw " and try again." " 並重新測試."
::msgcat::mcset zh_tw "Note: This message might also be triggered by a\
                       string\n  quoted with single quotes and containing\
                       the word \"function\",\n  when \"Colorize\
                       \'strings\'\" is unchecked in the Options menu."\
                      "注意: 如果選單中 \"多色顯示(&s) (Colorize\
                       \'strings\')\" 未被勾選，\n \
                       此訊息也可能被單引號 包覆之內含\
                       \"function\" 的字串\n  所觸發."
::msgcat::mcset zh_tw "This known bug can be worked around simply by\
                       checking \"Colorize \'&strings\'\"."\
                      "這個 bug 可以很簡單的透過勾選\
                       \"多色顯示(&s)\" 以避免."
::msgcat::mcset zh_tw "Unterminated function definition found"\
                      "函數定義未結束"
::msgcat::mcset zh_tw "Warning!\n\n  One or more function is defined more\
                       than once in the currently opened files.\n  The\
                       debugger cannot cope with multiple copies of\
                       functions.\n  The following duplicates were\
                       detected:\n"\
                      "警告 !\n\n \
                      一個以上的函數在目前開啟的檔案中，重複定義.\n \
                       偵錯器無法應付多重定義函數.\n \
                       偵測到之重複定義為 :\n"
::msgcat::mcset zh_tw "\n--> Definition of function " "\n--> 函數定義 "
::msgcat::mcset zh_tw " found in:\n" " 位置 :\n"
::msgcat::mcset zh_tw "Duplicate function definitions found"\
                      "重複之函數定義"



# db_debugsession.tcl
::msgcat::mcset zh_tw "Too many breakpointed functions"\
                      "設定停止點(breakpoint)的函數數目太多"
::msgcat::mcset zh_tw "You have currently" "目前共有"
::msgcat::mcset zh_tw "functions in your opened files."\
                      "個設定停止點(breakpoint)函數."
::msgcat::mcset zh_tw "Scilab supports a maximum of" "Scilab 最多允許"
::msgcat::mcset zh_tw "possible breakpointed functions (see help setbpt)."\
                      "個函數設定停止點(breakpoint)."
::msgcat::mcset zh_tw "Step-by-step hence cannot be performed."\
                      "因此無法進行逐步偵錯."
::msgcat::mcset zh_tw "This command will actually run to the next breakpoint."\
                      "此指令實際將會進行到下個停止點(breakpoint)."
::msgcat::mcset zh_tw "Break hence cannot be performed." "所以無法暫停."
::msgcat::mcset zh_tw "Too many breakpoints" "太多停止點"
::msgcat::mcset zh_tw "Executing this command would require to set"\
                      "執行此指令必須在開啟之檔案內設定"
::msgcat::mcset zh_tw "breakpoints in your opened files." "停止點."
::msgcat::mcset zh_tw "possible breakpoints in Scilab (see help setbpt)."\
                      "Scilab 中可能之停止點 (參考 setbpt 指令)."
::msgcat::mcset zh_tw "Cursor must be in a function" "游標必須在函數內"
::msgcat::mcset zh_tw "This command cannot succeed if the function where the\
                       debug just stopped is not opened in Scipad!"\
                      "如果定義函數的檔案未在 Scipad\
                       中開啟，此指令無法成功執行 !"
::msgcat::mcset zh_tw "Cannot run to return point"\
                      "無法執行到函數回返點 (return point)"
::msgcat::mcset zh_tw "Warning!\nThe line where the debugger just stopped\
                       contains a return statement (either endfunction,\
                       return or resume) but this statement is not the first\
                       one on the line."\
                      "\
                      警告!\n偵錯器停止的位置是一函數的回返點\
                       (指令 endfunction, return 或\
                       resume),但卻不是此行第一個指令 ."
::msgcat::mcset zh_tw "As a consequence the debugger cannot predict if the\
                       next debug step will really jump outside of the\
                       current function, which is what you just asked the\
                       debugger to do."\
                      "因此，偵錯器無法預測是否一個偵錯步該跳出目前含數範圍，而這是你所要求的."
::msgcat::mcset zh_tw "To avoid this unresolvable case, it is recommended to\
                       place the return statement alone on its line."\
                      "為避免這種無法判斷狀況，建議將回返點\
                       (指令 endfunction, return 或\
                       resume)獨立成一行."
::msgcat::mcset zh_tw "Do you want to see this warning again during this\
                       Scipad session?"\
                      "你希望在 Scipad 中繼續看到此警告訊息嗎?"
::msgcat::mcset zh_tw "Unresolvable case found while running to return point"\
                      "執行到回返點(return point)時無法判斷"
::msgcat::mcset zh_tw "No effect - The debugged file is not stuck"\
                      "無影響 - 檔案繼續進行偵錯"


# db_init.tcl
::msgcat::mcset zh_tw "Please wait..." "稍待片刻..."


# db_states.tcl
::msgcat::mcset zh_tw "Currently no debug session" "目前無偵錯段"
::msgcat::mcset zh_tw "Ready to start debug" "準備進行偵錯"
::msgcat::mcset zh_tw "Debug in progress" "偵錯中"
::msgcat::mcset zh_tw "Scilab is back at the main level now."\
                      "Scilab 返回主執行層 (main level)."



# db_watch.tcl
::msgcat::mcset zh_tw "Watch" "檢視"
::msgcat::mcset zh_tw "Watch variables:" "檢視變數 :"
::msgcat::mcset zh_tw "A&uto (local variables)" "A&uto 變數(局部變數)"
::msgcat::mcset zh_tw "Au&to watch globals too"\
                      "Au&to 同時檢視 globlas 變數"
::msgcat::mcset zh_tw "Generic expressions:" "通用算式 :"
::msgcat::mcset zh_tw "Call stack:" "呼叫堆疊 (call stack) :"
::msgcat::mcset zh_tw "Always on to&p" "永遠在最上層(&p)"
::msgcat::mcset zh_tw "Hide watch &variables" "隱藏檢視變數"
::msgcat::mcset zh_tw "Hide e&xpressions" "隱藏算式(&x)"
::msgcat::mcset zh_tw "Hide call &stack" "隱藏呼叫堆疊 (call &stack) "
::msgcat::mcset zh_tw "&Dock" "減少(&D)"
::msgcat::mcset zh_tw "&Add" "新增(&A)"
::msgcat::mcset zh_tw "&Filter errors" "&Filter 錯誤"
::msgcat::mcset zh_tw "Breakpoints" "停止點"
::msgcat::mcset zh_tw "Breakpointed functions" "設定停止點的函數"
::msgcat::mcset zh_tw "Scilab busy indicator" "Scilab 忙碌裝況指示"
::msgcat::mcset zh_tw "Debugger state indicator" "偵錯器裝態指示"
::msgcat::mcset zh_tw "Undefined elements are not legal in variable"\
                      " 變數中出現未定義元素"
::msgcat::mcset zh_tw ".\nThis variable will not be updated in Scilab."\
                      ".\n此變數無法在 Scilab 內更新"
::msgcat::mcset zh_tw "Illegal undefined element found"\
                      "不合法之未定義元素"
::msgcat::mcset zh_tw "evaluates to:" "計算值:"
::msgcat::mcset zh_tw "produces error" "產生錯誤"
::msgcat::mcset zh_tw "Scipad cannot find source code for the selected\
                       function.\nPlease open the adequate file manually and\
                       try again."\
                      "Scipad\
                      無法找到所選函數的原始檔.\n請手動開啟洽當的檔案."
::msgcat::mcset zh_tw "Missing source code" "找不到原始檔"


# filecommands.tcl
::msgcat::mcset zh_tw "New script" "新程序檔 (script)"
::msgcat::mcset zh_tw "New view on the same file" "同一檔案的另一視窗"
::msgcat::mcset zh_tw "The contents of" "檔案"
::msgcat::mcset zh_tw "may have changed, do you wish to save your changes?"\
                      "內容可能已變更，是否需要儲存你的變更?"
::msgcat::mcset zh_tw "Save Confirm?" "確定儲存檔案內容 ?"
::msgcat::mcset zh_tw "Open source of..." "開啟函數之原始檔..."
::msgcat::mcset zh_tw "Open source of:" "開啟函數之原始檔 :"
::msgcat::mcset zh_tw "Weird drag&drop" "古怪的 drag&drop"
::msgcat::mcset zh_tw "is neither a valid filename nor a valid directory.\
                       Either you're dragging an object of another type, or\
                       you hit a bug of the dnd mechanism."\
                      "檔名或目錄名稱不合法.\
                       可能是檔案型態不對."
::msgcat::mcset zh_tw "File not found" "無此檔案"
::msgcat::mcset zh_tw "does not exist anymore. Do you want to create an\
                       empty file with the same name?"\
                      "不再存在.\
                       你希望產生一個同名之新空白檔案嗎 ?"
::msgcat::mcset zh_tw "Open file" "開啟檔案"
#::msgcat::mcset zh_tw "The file" "檔按" ;# defined in the "commons" section
::msgcat::mcset zh_tw "is already opened! Save the current opened file to\
                       another name and reopen it from disk!"\
                      "已經開啟，請將目前檔案以另一檔名存起，並從硬碟中再開啟\
                       !"
::msgcat::mcset zh_tw "File" "檔案"
::msgcat::mcset zh_tw "saved" "存起"
::msgcat::mcset zh_tw "cannot be written!" "無法寫入 !"
::msgcat::mcset zh_tw "The contents of " "磁碟內之檔案內容"
::msgcat::mcset zh_tw "has changed on disk, save it anyway?"\
                      "已經變更, 繼續進行儲存?"
::msgcat::mcset zh_tw "The contents and/or properties of " "磁碟內之檔案內容"
::msgcat::mcset zh_tw "has been modified outside of Scipad. Do you want to\
                       reload it?"\
                      "已再 Scipad\
                       外變更內容，你希望重新載入到 Scipad 中\
                       ?"
::msgcat::mcset zh_tw "File has changed!" "檔案以變更!"
::msgcat::mcset zh_tw "Save as" "存成"
::msgcat::mcset zh_tw "Scilab files" "Scilab 檔"
::msgcat::mcset zh_tw "Scicos files" "Scicos 檔"
::msgcat::mcset zh_tw "XML files" "XML 檔"
::msgcat::mcset zh_tw "Wait seconds while loading and colorizing file"\
                      "載入檔案時，稍待片刻"
::msgcat::mcset zh_tw "The contents of this file may have changed, do you\
                       wish to save your changes?"\
                      "檔案內容已變更，你希儲存變更嗎 ?"
::msgcat::mcset zh_tw "Revert" "恢復"
::msgcat::mcset zh_tw "to saved?" "原狀後存起?"
::msgcat::mcset zh_tw "Revert Confirm?" "確定恢復原狀 ?"
::msgcat::mcset zh_tw "Unreadable file" "檔案無法讀取!"
::msgcat::mcset zh_tw "exists but is not readable!" "檔案存在但不可讀!"
::msgcat::mcset zh_tw "This file is rather large and colorization might take\
                       some time.\n\nColorize anyway?"\
                      "此檔相當大，若完成全部文字色彩可能費時.\n\n，你確定繼續進行文字色彩?"
::msgcat::mcset zh_tw "File size warning" "檔案過大警告"
::msgcat::mcset zh_tw "Windows shortcut file" "Windows 捷徑"
#::msgcat::mcset zh_tw "The file" "檔案" ;# defined in the "commons" section
::msgcat::mcset zh_tw "is a Windows shortcut and will not be opened!"\
                      "為 Windows 捷徑，無法開啟 !"


# findreplace.tcl
::msgcat::mcset zh_tw "Dialog box already open!"\
                      "對話盒 (Dialog box)已開啟!"
::msgcat::mcset zh_tw "The string" "字串"
::msgcat::mcset zh_tw "could not be found in" "無法在 .. 找到"
::msgcat::mcset zh_tw "could not be found in any of the opened files !"\
                      "無法在開啟之檔案內找到 !"
::msgcat::mcset zh_tw "replacements done" "完成取代動作"
::msgcat::mcset zh_tw "Find" "尋找"
::msgcat::mcset zh_tw "No match found in the selection for"\
                      "選擇區內找不到相符者"
::msgcat::mcset zh_tw "\nWould you like to look for it in the entire text?"\
                      "\n希望在全文內搜尋 ?"
::msgcat::mcset zh_tw "You are searching for an empty string!"\
                      "你是否尋找一空字串!"
::msgcat::mcset zh_tw "Re&place" "取代(&R)"
::msgcat::mcset zh_tw "Find what:" "尋找:"
::msgcat::mcset zh_tw "Replace with:" "取代成 :"
::msgcat::mcset zh_tw "Find &Next" "尋找下一個(&N)"
::msgcat::mcset zh_tw "Replace &All" "全部取代(&a)"
::msgcat::mcset zh_tw "Direction" "方向"
::msgcat::mcset zh_tw "&Upwards" "向上尋找(&U)"
::msgcat::mcset zh_tw "&Downwards" "向下尋找(&D)"
::msgcat::mcset zh_tw "Text types" "文字型態"
::msgcat::mcset zh_tw "Find options" "檔案選項"
::msgcat::mcset zh_tw "Match &whole word only"\
                      "只檢驗ㄧ完整之文字(&w)"
::msgcat::mcset zh_tw "Match &case" "大小寫敏感(&c)"
::msgcat::mcset zh_tw "&Regular expression" "正則運算(&Regular expression)"
::msgcat::mcset zh_tw "In all &opened files" "在所有開啟檔案內(&o)"
::msgcat::mcset zh_tw "In &selection only" "在選擇區內(&s)"
::msgcat::mcset zh_tw "In typed te&xt" "在特定型態文字內(&x)"
::msgcat::mcset zh_tw "In a director&y" "在目錄內(&y)"
::msgcat::mcset zh_tw "Find in files search options" "檔案搜索選項"
::msgcat::mcset zh_tw "Directory r&ecurse search" "目錄內遞迴搜尋(&e)"
::msgcat::mcset zh_tw "Include &hidden files" "包含隱藏檔(&h)"
::msgcat::mcset zh_tw "In files/file types:" "在以下檔案型態內 :"
::msgcat::mcset zh_tw "In directory:" "在目錄內 :"
::msgcat::mcset zh_tw "Search for file names" "以檔名搜索"
::msgcat::mcset zh_tw "You have given an empty search string. Are you sure\
                       you want to search among file names?"\
                      "你可能輸入一個空檔名. 確定繼續 ?"
::msgcat::mcset zh_tw "Wrapped around" "繞過去"
::msgcat::mcset zh_tw "Back at the first file searched!"\
                      "回到第一個找到的檔案!"
::msgcat::mcset zh_tw "No more matches" "無法再找到相符的"
::msgcat::mcset zh_tw "The string to regexp search for cannot be compiled by\
                       Tcl.\nCheck again, maybe certain characters should be\
                       escaped."\
                      "此正則運算無法以 TCL 編譯。\n\
                      再檢查ㄧ次，也許有些字元需加上跳脫字元"
::msgcat::mcset zh_tw "Any single character" "任一單一字元"
::msgcat::mcset zh_tw "Zero or more" "零次以上"
::msgcat::mcset zh_tw "One or more" "一次以上"
::msgcat::mcset zh_tw "Zero or one" "零或一次"
::msgcat::mcset zh_tw "Any one character in the set" "集合內任一字元"
::msgcat::mcset zh_tw "Any one character not in the set"\
                      "任一不在集合內之字元"
::msgcat::mcset zh_tw "Or" "或"
::msgcat::mcset zh_tw "Beginning of line" "一行最前"
::msgcat::mcset zh_tw "End of line" "一行最後"
::msgcat::mcset zh_tw "Beginning of word" "一字最前"
::msgcat::mcset zh_tw "End of word" "一字最後"
::msgcat::mcset zh_tw "A letter" "一文字字元 (letter)"
::msgcat::mcset zh_tw "A decimal digit" "一數字字元 (decimal digit)"
::msgcat::mcset zh_tw "An alphanumeric (letter or digit)" "英文字母"
::msgcat::mcset zh_tw "A space or tab character"\
                      "空白(space) 或 對齊(tab) 字元"
::msgcat::mcset zh_tw "A rational number" "一有理數"
::msgcat::mcset zh_tw "A floating point number" "一實數"
::msgcat::mcset zh_tw "A Scilab comment" "Scilab 指令"
::msgcat::mcset zh_tw "A Scilab name" "Scilab 變數名"
::msgcat::mcset zh_tw "A Scilab continued line (with dots)"\
                      "Scilab 連續行 (行後為 .. 字串)"


# findinfiles.tcl
::msgcat::mcset zh_tw "Directory access issue" "目錄存取問題"
::msgcat::mcset zh_tw "The directory you specified cannot be reached. The\
                       search won't bring any result."\
                      "所指定的目錄無法存取. 搜尋無結果."
::msgcat::mcset zh_tw "Some directories or files could not be open and were\
                       ignored during search.\n You might miss read access\
                       to them.\n\n The following ones were ignored"\
                      "部分目錄或檔案無法開啟，因此搜索過程中忽略它們.\n\
                       或許是讀取允許設定不對.\n\n\
                       以下為忽略之部分"
::msgcat::mcset zh_tw "Ignored files" "忽略之檔案"
::msgcat::mcset zh_tw "Searching in file:" "檔案內搜索 :"
::msgcat::mcset zh_tw "End of file search:" "檔案搜索結束:"
::msgcat::mcset zh_tw "matches found" "符合項"
::msgcat::mcset zh_tw "searched files" "搜索過之檔案"
::msgcat::mcset zh_tw "Constructing file list:" "建立檔案清單:"
::msgcat::mcset zh_tw "&Previous" "向前(&P)"
::msgcat::mcset zh_tw "&Next" "向後(&N)"
::msgcat::mcset zh_tw "Scipad search results" "Scipad 搜索結果"


# fonts.tcl
::msgcat::mcset zh_tw "Choose font" "選擇字型"
::msgcat::mcset zh_tw "Family" "字族"
::msgcat::mcset zh_tw "Style" "風格(Style)"
::msgcat::mcset zh_tw "&Bold" "粗體(&B)"
::msgcat::mcset zh_tw "&Italic" "斜體(&I)"
::msgcat::mcset zh_tw "&Underline" "底線(&U)"
::msgcat::mcset zh_tw "Size" "大小"
::msgcat::mcset zh_tw "&Defaults" "內定(&D)"
::msgcat::mcset zh_tw "Use as..." "做為..."
::msgcat::mcset zh_tw "&Text font" "文字字型(&T)"
::msgcat::mcset zh_tw "&Menu font" "選單字型(&M)"
::msgcat::mcset zh_tw "Sample" "例"
::msgcat::mcset zh_tw "Font size" "字體大小"
::msgcat::mcset zh_tw "Revert fonts" "回復成內定字體"
::msgcat::mcset zh_tw "Are you sure you want to revert all fonts to defaults?"\
                      "你確定要回復成內定字體 ?"


# FormatStringsForWatch.sci
::msgcat::mcset zh_tw " what you try to watch is not supported by the\
                       debugger"\
                      " 你所要檢視的內容還未為 Scipad\
                       偵錯器支持"
::msgcat::mcset zh_tw " what you try to watch is of type "\
                      " 你所檢視的型態為 "
::msgcat::mcset zh_tw " - this type is not supported by the debugger"\
                      " - 此型態還未為 Scipad 偵錯器支持"
::msgcat::mcset zh_tw "Type:" "型態 :"
::msgcat::mcset zh_tw "(unsupported)" "(未支持)"
::msgcat::mcset zh_tw "size:" "大小 :"
::msgcat::mcset zh_tw "real" "實數"
::msgcat::mcset zh_tw "complex" "複數"
::msgcat::mcset zh_tw "elements" "元素"
::msgcat::mcset zh_tw "graphic handle" "圖形  handle"
::msgcat::mcset zh_tw "uncompiled function" "未編譯之函數"
::msgcat::mcset zh_tw "compiled function" "已編譯函數"
::msgcat::mcset zh_tw "library" "程式庫"
::msgcat::mcset zh_tw "pointer" "指標"
::msgcat::mcset zh_tw "size implicit index" "大小之隱含指標"
::msgcat::mcset zh_tw "primitive" "基礎"


# FormatWhereForWatch.sci
::msgcat::mcset zh_tw "Breakpoint" "停止點"
::msgcat::mcset zh_tw "called at line" "在第"
::msgcat::mcset zh_tw "of exec file" "行被執行程序呼叫"
::msgcat::mcset zh_tw "of execstr instruction" "行被 execstr 的指令呼叫"
::msgcat::mcset zh_tw "called under pause" "在 pause 狀態下被呼叫"
::msgcat::mcset zh_tw "of macro" "被巨集呼叫"


# infomessages.tcl
::msgcat::mcset zh_tw "Line:" "行:"
::msgcat::mcset zh_tw "Column:" "欄:"
::msgcat::mcset zh_tw "Line" "第"
::msgcat::mcset zh_tw "in" " 行在 "
::msgcat::mcset zh_tw "Logical line:" "邏輯行:"
::msgcat::mcset zh_tw " \[ReadOnly\]" " \[僅可讀\]"
::msgcat::mcset zh_tw " (modified)" " (修改過)"


# inputtext.tcl
::msgcat::mcset zh_tw "No corresponding <" "沒找到 <"
::msgcat::mcset zh_tw "> found!" ">  !"
::msgcat::mcset zh_tw "Code editing is not allowed during debug!"\
                      "偵錯過程中，無法編輯程式 !"


# menues.tcl
::msgcat::mcset zh_tw "&File" "檔案(&F)"
::msgcat::mcset zh_tw "&Edit" "編輯(&E)"
::msgcat::mcset zh_tw "&Search" "搜尋(&R)"
::msgcat::mcset zh_tw "E&xecute" "執行(&x)"
::msgcat::mcset zh_tw "&Debug" "偵錯(&D)"
::msgcat::mcset zh_tw "S&cheme" "程式語言(&L)"
::msgcat::mcset zh_tw "&Options" "選項(&O)"
::msgcat::mcset zh_tw "&Windows" "視窗(&W)"
::msgcat::mcset zh_tw "&Help" "輔助(&H)"

# menues.tcl - File menu
::msgcat::mcset zh_tw "&New" "新檔(&N)"
::msgcat::mcset zh_tw "&Open..." "開啟舊檔(&O)..."
::msgcat::mcset zh_tw "Open &in new" "在新視窗開啟(&u)"
::msgcat::mcset zh_tw "&horizontal tile" "水平重疊(&h)"
::msgcat::mcset zh_tw "&vertical tile" "垂直重疊(&v)"
::msgcat::mcset zh_tw "Op&en source of..." "開啟...的原始碼(&e)"
::msgcat::mcset zh_tw "&Save" "存檔(&S)"
::msgcat::mcset zh_tw "Save &as..." "另存新檔(&a)"
::msgcat::mcset zh_tw "Save a&ll" "存起所有檔案(&l)"
::msgcat::mcset zh_tw "&Revert..." "回復(&v)..."
::msgcat::mcset zh_tw "Import &Matlab file..." "載入 &Matlab 檔..."
::msgcat::mcset zh_tw "Create help s&keleton..." "產生文件架構(&k)..."
::msgcat::mcset zh_tw "Compile as &help page" "編譯出文件(&h)"
::msgcat::mcset zh_tw "Open &function source" "編輯函數原始碼(&f)"
::msgcat::mcset zh_tw "Print Se&tup" "印表機設定(&t)"
::msgcat::mcset zh_tw "&Print" "列印(&P)"
::msgcat::mcset zh_tw "&Close file" "關密檔案(&C)"
::msgcat::mcset zh_tw "E&xit" "結束(&x)"

# menues.tcl - Edit menu
::msgcat::mcset zh_tw "&Undo" "回復(&U)"
::msgcat::mcset zh_tw "&Redo" "再回復(&R)"
::msgcat::mcset zh_tw "Cu&t" "剪下(&t)"
::msgcat::mcset zh_tw "B&lock cut" "刪除文字塊(&l)"
::msgcat::mcset zh_tw "&Copy" "複製(&C)"
::msgcat::mcset zh_tw "&Paste" "貼上(&P)"
::msgcat::mcset zh_tw "&Block paste" "複製文字塊(&B)"
::msgcat::mcset zh_tw "&Delete" "刪除(&D)"
::msgcat::mcset zh_tw "Select &All" "全選(&A)"
::msgcat::mcset zh_tw "Insert &Time/Date" "插入時間/日期(&T)"
::msgcat::mcset zh_tw "Co&mment selection" "選擇區成為註解(&m)"
::msgcat::mcset zh_tw "U&ncomment selection"\
                      "清除選擇區之註解標誌(&n)"
::msgcat::mcset zh_tw "&Indent selection" "縮排(&I)"
::msgcat::mcset zh_tw "Unin&dent selection" "清除縮排(&d)"

# menues.tcl - Search menu
::msgcat::mcset zh_tw "&Find..." "尋找(&F)..."
::msgcat::mcset zh_tw "Find &Next" "尋找下一個(&N)"
::msgcat::mcset zh_tw "&Replace..." "取代(&R)"
::msgcat::mcset zh_tw "&Goto Line..." "跳至第...行(&G)"

# menues.tcl - Exec menu
::msgcat::mcset zh_tw "&Load into Scilab" "載入 Scilab 中(&L)"
::msgcat::mcset zh_tw "&Evaluate selection" "執行選擇之部分(&E)"

# menues.tcl - Debug menu
::msgcat::mcset zh_tw "&Insert/Remove breakpoint" "插入/移除停止點(&I)"
::msgcat::mcset zh_tw "Remove &all breakpoints" "清除所有停止點(&a)"
::msgcat::mcset zh_tw "&Configure execution..." "設定執行組態(&C)..."
::msgcat::mcset zh_tw "Go to next b&reakpoint" "跳至下一停止點(&r)"
::msgcat::mcset zh_tw "&Step by step" "一步一步(&S)"
::msgcat::mcset zh_tw "Step &into" "進入(&i)"
::msgcat::mcset zh_tw "Step o&ver" "跳過(&v)"
::msgcat::mcset zh_tw "Step &out" "跳出(&o)"
::msgcat::mcset zh_tw "Run to re&turn point"\
                      "執行至返回點 (re&turn point)"
::msgcat::mcset zh_tw "Run to c&ursor" "執行至游標處(&u)"
::msgcat::mcset zh_tw "G&o on ignoring any breakpoint"\
                      "執行並忽略所有停止點"
::msgcat::mcset zh_tw "Show &watch" "檢查變數(&w)"
::msgcat::mcset zh_tw "&Break" "終止(&B)"
::msgcat::mcset zh_tw "Cance&l debug" "取消偵錯(&l)"

#    ::msgcat::mcset zh_tw "language scheme" "程式語言"
# menues.tcl - Scheme menu
::msgcat::mcset zh_tw "S&cilab" "S&cilab"
::msgcat::mcset zh_tw "&XML" "&XML"
::msgcat::mcset zh_tw "&none" "無(&n)"
::msgcat::mcset zh_tw "&Colorize" "多色顯示(&C)"

# menues.tcl - Options menu
::msgcat::mcset zh_tw "&Fonts..." "字體(&F)..."
::msgcat::mcset zh_tw "&Colors" "顏色(&C)"
::msgcat::mcset zh_tw "Colori&ze" "彩色文字(&z)"
::msgcat::mcset zh_tw "&Always" "永遠要"
::msgcat::mcset zh_tw "Ask for &big files" "檔案過大時詢問"
::msgcat::mcset zh_tw "&Never" "不須要"
::msgcat::mcset zh_tw "Colorize \'&strings\'" "多色顯示"
::msgcat::mcset zh_tw "Line n&umbers" "行號(&u)"
::msgcat::mcset zh_tw "&Hide" "隱藏(H)"
::msgcat::mcset zh_tw "&Left aligned" "左邊對齊(L)"
::msgcat::mcset zh_tw "&Right aligned" "右邊對齊(R)"
::msgcat::mcset zh_tw "Show c&ontinued lines" "顯示連續行(&o)"
::msgcat::mcset zh_tw "Word &wrap" "文字環繞(&W)"
::msgcat::mcset zh_tw "&Double-click behavior" "滑鼠雙擊行為(&D)"
::msgcat::mcset zh_tw "&Linux" "&Linux"
::msgcat::mcset zh_tw "Windo&ws" "Windo&ws"
::msgcat::mcset zh_tw "&Scilab" "&Scilab"
::msgcat::mcset zh_tw "&Tabs and indentation" "定位點及縮排(&T)"
::msgcat::mcset zh_tw "Tab inserts &spaces" "定位點之空白鍵數目(&s)"
::msgcat::mcset zh_tw "&Indentation spaces" "縮排之空白鍵數目(&I)"
::msgcat::mcset zh_tw "&Tab size (characters)" "定位點大小(&T)"
::msgcat::mcset zh_tw "Use &keyword indentation" "使用關鍵字定位 (&k)"
::msgcat::mcset zh_tw "Com&pletion" "結束(&p)"
::msgcat::mcset zh_tw "&Tab" "&Tab"
::msgcat::mcset zh_tw "&Control-Tab" "&Control-Tab"
::msgcat::mcset zh_tw "&Alt-Tab" "&Alt-Tab"
::msgcat::mcset zh_tw "&Shift-Tab" "&Shift-Tab"
::msgcat::mcset zh_tw "C&ontrol-Alt-Tab" "C&ontrol-Alt-Tab"
::msgcat::mcset zh_tw "S&hift-Control-Tab" "S&hift-Control-Tab"
::msgcat::mcset zh_tw "Sh&ift-Alt-Tab" "Sh&ift-Alt-Tab"
::msgcat::mcset zh_tw "Shi&ft-Control-Alt-Tab" "Shi&ft-Control-Alt-Tab"
::msgcat::mcset zh_tw "File&names" "檔案名稱(&n)"
::msgcat::mcset zh_tw "&Full path" "全名(&F)"
::msgcat::mcset zh_tw "Full path if &ambiguous"\
                      "如果名稱衝突時使用全名(&a)"
::msgcat::mcset zh_tw "&Unambiguous pruned path" "短名(&U)"
::msgcat::mcset zh_tw "Windows menu &sorting" "視窗排序方法(&s)"
::msgcat::mcset zh_tw "&File opening order" "依檔案開啟順序(&F)"
::msgcat::mcset zh_tw "&Alphabetically" "依字母順序(&A)"
::msgcat::mcset zh_tw "&Most recently used" "依使用順序(&M)"
::msgcat::mcset zh_tw "&Recent files" "最近檔案(&R)"
::msgcat::mcset zh_tw "&Backup files depth" "備存檔深度(&B)"
::msgcat::mcset zh_tw "&Locale" "地區化語言"
::msgcat::mcset zh_tw "Execution &errors" "執行錯誤(&e)"
::msgcat::mcset zh_tw "In Scilab &shell only" "只出現在Scilab &shell "
::msgcat::mcset zh_tw "Copied in a &message box" "複製到訊息區(&m)"

# menues.tcl - Windows menu
::msgcat::mcset zh_tw "&Maximize" "視窗最大化(&M)"
::msgcat::mcset zh_tw "&Split" "分割(&S)"
::msgcat::mcset zh_tw "S&plit (side by side)" "左右分割(&p)"
::msgcat::mcset zh_tw "Sp&lit file" "分割檔案(&l)"
::msgcat::mcset zh_tw "Spl&it file (side by side)" "左右分割檔案(&i)"
::msgcat::mcset zh_tw "Tile all &vertically" "垂直重疊(&v)"
::msgcat::mcset zh_tw "Tile all &horizontally" "水平重疊(&h)"
::msgcat::mcset zh_tw "Space sashes &evenly" "視窗均分(&e)"
::msgcat::mcset zh_tw "Opened Files" "開啟之檔案"

# menues.tcl - Help menu
::msgcat::mcset zh_tw "&Help..." "Scipad 參考..."
::msgcat::mcset zh_tw "&What's?..." "這是..?(&W)(查詢選擇之函數)"
::msgcat::mcset zh_tw "&About" "有關(&A)"
::msgcat::mcset zh_tw "Adding &translations..." "協助翻譯 Scipad...(&T)"
::msgcat::mcset zh_tw "Adding Scipad Translations..." "協助翻譯 Scipad..."
::msgcat::mcset zh_tw "&Bugs \& wishlist" "已知臭蟲及計畫工作(&B)"
::msgcat::mcset zh_tw "Scipad Known bugs" "Scipad 已知臭蟲 "
::msgcat::mcset zh_tw "Change&log" "變動(&l)"
::msgcat::mcset zh_tw "Changes in the Scipad codebase"\
                      "Scipad 程式碼之變動"


# popupmenus.tcl
::msgcat::mcset zh_tw "Open the source of" "開啟 .. 的原始檔"
::msgcat::mcset zh_tw "AddWatch" "新增觀測值"
::msgcat::mcset zh_tw "AddGenExp" "新增通用算式"
::msgcat::mcset zh_tw "Jump to" "跳到"


# print.tcl
::msgcat::mcset zh_tw "Print Setup" "印表機設定"
::msgcat::mcset zh_tw "Print Command: " "列印指令 : "


# scilabexec.tcl
::msgcat::mcset zh_tw "No point in loading an empty file!"\
                      "無法載入一個空檔案!"
::msgcat::mcset zh_tw "Silent file save failed" "無名檔案無法存起"
::msgcat::mcset zh_tw "File could not be saved in a temporary\
                       location.\nOverwrite original file?"\
                      "暫存位置內無法存檔. \n覆蓋原始檔案 ?"
::msgcat::mcset zh_tw "Exec done" "執行結束"
::msgcat::mcset zh_tw "Matlab files" "Matlab 檔"
::msgcat::mcset zh_tw "Matlab file to convert" "轉換 Matlab 檔"
::msgcat::mcset zh_tw "Scilab is converting, please hold on..."\
                      "Scilab 轉換中，稍待..."
::msgcat::mcset zh_tw "Matlab file import" "載入 Matlab 檔"
::msgcat::mcset zh_tw "Conversion of the file failed, see the Scilab window\
                       for details -- Perhaps report the error text and the\
                       offending Matlab file to"\
                      "檔案轉換失敗，詳情請參考 Scilab\
                       視窗內容 -- 也許可將錯誤訊息及對應之\
                       Matlab 檔到\
                       http://bugzilla.scilab.org\
                       位置報告"
::msgcat::mcset zh_tw "Please select destination path for the xml source of\
                       the help file"\
                      "請選擇 xml help 檔的路徑位置 "
::msgcat::mcset zh_tw "Older version of" "找到檔案"
::msgcat::mcset zh_tw "found!" "的舊版本!"
::msgcat::mcset zh_tw "An old version of" "檔案"
::msgcat::mcset zh_tw "already exists: open the old file instead?"\
                      "的舊版本已經存在: 開啟舊檔?"
::msgcat::mcset zh_tw "Existing file" "已存在之檔案"
::msgcat::mcset zh_tw "New skeleton" "新的檔案骨架"
::msgcat::mcset zh_tw "ScilabEval command cannot be passed to Scilab!"\
                      "ScilabEval 指令無法傳送至 Scilab!"
::msgcat::mcset zh_tw "impossibleScilabEval_message"\
                      "警告: 指令無法傳送至 Scilab,\
                       因此忽略.\n\n  最有可能是你沒有 Scilab\
                       的 TMPDIR 目錄的寫入權限. \
                       可能產生不可預期的結果.\n\n \
                       出問題的指令為:\n\n-->"
::msgcat::mcset zh_tw "impossibleScilabEval_message2"\
                      "警告: 指令無法傳送至 Scilab, 因此忽略， \
                       原因是指令過長.\n\n \
                       可能產生不可預期的結果.\n\n \
                      如果正在偵錯，請試著移除觀測之變數.\n\n \
                       出問題的指令為:\n\n-->"
::msgcat::mcset zh_tw "(end of command skipped)" "(後面指令跳過)"
::msgcat::mcset zh_tw "Scilab working" "Scilab 工作中"
::msgcat::mcset zh_tw "Scilab is working, please wait for the prompt to\
                       execute this command!"\
                      "Scilab 工作中，請等候此指令之輸出結果 !"
::msgcat::mcset zh_tw "Scilab is working, wait for the prompt to load file"\
                      "Scilab 工作中，請等候載入檔案之結果"
::msgcat::mcset zh_tw "Scilab is working, wait for the prompt to execute the\
                       selection."\
                      "Scilab\
                       工作中，請等候執行選擇之指令部份."
::msgcat::mcset zh_tw "Scilab is working, wait for the prompt to convert a\
                       Matlab file."\
                      "Scilab 工作中，請等候轉換 Matlab 檔案結果."
::msgcat::mcset zh_tw "Scilab is working, wait for the prompt to compile the\
                       help file."\
                      "Scilab 工作中，請等候編譯說明檔之結果."
::msgcat::mcset zh_tw "Scilab is working, wait for the prompt to issue a\
                       debugger command."\
                      "Scilab 工作中，請等候偵錯指令之結果."
::msgcat::mcset zh_tw "Scilab execution error" "Scilab  執行錯誤"
::msgcat::mcset zh_tw "The shell reported an error while trying to execute "\
                      "執行  "
::msgcat::mcset zh_tw ": error " "時，Scilab Shell 發現錯誤: 錯誤 "
::msgcat::mcset zh_tw "at line " "第 "
::msgcat::mcset zh_tw " of " " 行，位於 "
::msgcat::mcset zh_tw "Execution aborted!" "執行中斷!"


# scipad.tcl
::msgcat::mcset zh_tw "NOT CONNECTED TO SCILAB" "未與 SCILAB 連結"

# whichfun.tcl
::msgcat::mcset zh_tw "Being at line" "第"
::msgcat::mcset zh_tw ", function" "行, 函數"
::msgcat::mcset zh_tw "begins at" "起始位置為"
::msgcat::mcset zh_tw ", and there are" ", 共有"
::msgcat::mcset zh_tw "continued lines, i.e. we are at line"\
                      "行連續行，也就是目前為第"
::msgcat::mcset zh_tw "of" "行，此行位於"
::msgcat::mcset zh_tw "Function" "函數"
::msgcat::mcset zh_tw "line" "行"
::msgcat::mcset zh_tw "The cursor is not currently inside a function body."\
                      "游標目前不在函數內部."
::msgcat::mcset zh_tw "Which function?" "哪一個函數 ?"



#preference variables description
::msgcat::mcset zh_tw "BGCOLOR" "編輯區背景色"
::msgcat::mcset zh_tw "BGLNMARGCOLOR" "行號背景色"
::msgcat::mcset zh_tw "FGCOLOR" "文字"
::msgcat::mcset zh_tw "FGLNMARGCOLOR" "行號"
::msgcat::mcset zh_tw "CURCOLOR" "游標"
::msgcat::mcset zh_tw "PARCOLOR" "括符 ()"
::msgcat::mcset zh_tw "BRAKCOLOR" "中括符 \\\[\\\]"
::msgcat::mcset zh_tw "BRACCOLOR" "大括符  {}"
::msgcat::mcset zh_tw "PUNCOLOR" "標點符號"
::msgcat::mcset zh_tw "INTFCOLOR" "內部函數"
::msgcat::mcset zh_tw "COMMCOLOR" "指令"
::msgcat::mcset zh_tw "SCICCOLOR" "Scicos 函數"
::msgcat::mcset zh_tw "OPCOLOR" "運算子"
::msgcat::mcset zh_tw "QTXTCOLOR" "字串文字"
::msgcat::mcset zh_tw "REMCOLOR" "註解"
::msgcat::mcset zh_tw "XMLCOLOR" "XML 標籤"
::msgcat::mcset zh_tw "NUMCOLOR" "數字"
::msgcat::mcset zh_tw "SELCOLOR" "選擇區"
::msgcat::mcset zh_tw "BREAKPOINTCOLOR" "停止點背景"
::msgcat::mcset zh_tw "LFUNCOLOR" "程式庫函數"
::msgcat::mcset zh_tw "PDEFCOLOR" "系統變數"
::msgcat::mcset zh_tw "FOUNDTEXTCOLOR" "符合條件之文字"
::msgcat::mcset zh_tw "CONTLINECOLOR" "連續行"
::msgcat::mcset zh_tw "USERFUNCOLOR" "使用者定義之函數"



#overriding tcl-core dialogs (many are actually already found in
#  TKDIR/msgs/fr.msg)
::msgcat::mcset zh_tw "&Directory:" "目錄(&D) :"
::msgcat::mcset zh_tw "File &name:" "檔案名稱 (&n):"
::msgcat::mcset zh_tw "Files of &type:" "檔案型態(&t) :"
#::msgcat::mcset zh_tw "&Save" "存檔(&S)" # already included
::msgcat::mcset zh_tw "&Open" "開啟(&O)"
::msgcat::mcset zh_tw "Open" "開啟"
::msgcat::mcset zh_tw "Save As" "存成"
::msgcat::mcset zh_tw "&Cancel" "取消(&C)"
::msgcat::mcset zh_tw "&Selection:" "選擇(&S) :"
::msgcat::mcset zh_tw "&Yes" "&Yes"
::msgcat::mcset zh_tw "&No" "&No"
::msgcat::mcset zh_tw "&Red" "紅(&R)"
::msgcat::mcset zh_tw "&Green" "綠(&G)"
::msgcat::mcset zh_tw "&Blue" "藍(&B)"
::msgcat::mcset zh_tw "Directory \"%1\$s\" does not exist."\
                      "附錄 \"%1\$s\"不存在."

# helps.tcl
::msgcat::mcset zh_tw "About" "有關"
::msgcat::mcset zh_tw "aboutme_message"\
                      "\n 原版為 Joseph Acosta , joeja@mindspring.com.\
                       所開發\n 由 Scilab Consortium 修改.\n Enrico\
                       Segre 2003-2006 進一步改寫.\n\
                       其他貢獻者，,\n 偵錯工具:\n  François\
                       Vogel 2004-2009 \n 各國語文翻譯:\n 瑞典文\
                       Gunnar Lindholm 2005.\n 挪威文 Torbjørn Pettersen\
                       2005.\n 德文   Dirk Reusch 2005.\n 波蘭文\
                       Bartosz Kosiorek 2005.\n 西班牙文 Juan Nicolás\
                       Quesada 2006.\n 中文 李運璋 2007.\n 丹麥文\
                       Kim B. Hansen 2007."
