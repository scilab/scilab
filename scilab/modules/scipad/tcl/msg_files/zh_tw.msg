# From Big5 中文版之　Scipad texts
# 此檔內容必須先轉成　utf8 格式後才能為　tcl 讀取
#
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
    ::msgcat::mcset zh_tw "&Revert..." "回復(&v)..."
    ::msgcat::mcset zh_tw "Import &Matlab file..."  "載入 &Matlab 檔..."
    ::msgcat::mcset zh_tw "Create help s&keleton..." "產生文件架構(&k)..."
    ::msgcat::mcset zh_tw "Compile as &help page" "編譯出文件(&h)"
    ::msgcat::mcset zh_tw "Open &function source" "編輯函數原始碼(&f)"
    ::msgcat::mcset zh_tw "Print Se&tup" "印表機設定(&t)"
    ::msgcat::mcset zh_tw "&Print" "列印(&P)"
    ::msgcat::mcset zh_tw "E&xit" "結束(&x)"

    # menues.tcl - Edit menu
    ::msgcat::mcset zh_tw "&Undo" "回復(&U)"
    ::msgcat::mcset zh_tw "&Redo" "再回復(&R)"
    ::msgcat::mcset zh_tw "Cu&t" "剪下(&t)"
    ::msgcat::mcset zh_tw "&Copy" "複製(&C)"
    ::msgcat::mcset zh_tw "&Paste" "貼上(&P)"
    ::msgcat::mcset zh_tw "&Delete" "刪除(&D)"
    ::msgcat::mcset zh_tw "Select &All" "全選(&A)"
    ::msgcat::mcset zh_tw "Insert &Time/Date" "插入時間/日期(&T)"
    ::msgcat::mcset zh_tw "Co&mment selection" "選擇區成為註解(&m)"
    ::msgcat::mcset zh_tw "U&ncomment selection" "清除選擇區之註解標誌(&n)"
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
    ::msgcat::mcset zh_tw "Run to c&ursor" "執行至游標處(&u)"
    ::msgcat::mcset zh_tw "G&o on ignoring any breakpoint" "執行並忽略所有停止點"
    ::msgcat::mcset zh_tw "Show &watch" "檢查變數(&w)"
    ::msgcat::mcset zh_tw "&Break" "終止(&B)"
    ::msgcat::mcset zh_tw "Cance&l debug" "取消偵錯(&l)"

#    ::msgcat::mcset zh_tw "language scheme" "程式語言"
    # menues.tcl - Scheme menu
    ::msgcat::mcset zh_tw "S&cilab" "S&cilab"
    ::msgcat::mcset zh_tw "&XML" "&XML"
    ::msgcat::mcset zh_tw "&none" "&aucun"

    # menues.tcl - Options menu
    ::msgcat::mcset zh_tw "&Font size" "字體大小(&F)"
        ::msgcat::mcset zh_tw "&micro" "微小(&m)"
        ::msgcat::mcset zh_tw "&small" "小(&s)"
        ::msgcat::mcset zh_tw "m&edium" "中(&m)"
        ::msgcat::mcset zh_tw "&large" "大(&l)"
    ::msgcat::mcset zh_tw "&Colors" "顏色(&C)"
    ::msgcat::mcset zh_tw "Colorize \'&strings\'" "彩色字串(&s)"
    ::msgcat::mcset zh_tw "Show c&ontinued lines" "顯示連續行(&o)"
    ::msgcat::mcset zh_tw "Word &Wrap" "Word &Wrap"
    ::msgcat::mcset zh_tw "&Tabs and indentation" "定位點及縮排(&T)"
        ::msgcat::mcset zh_tw "Tab inserts &spaces" "定位點之空白鍵數目(&s)"
        ::msgcat::mcset zh_tw "&Indentation spaces" "縮排之空白鍵數目(&I)"
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
        ::msgcat::mcset zh_tw "Full path if &ambiguous" "如果名稱衝突時使用全名(&a)"
        ::msgcat::mcset zh_tw "&Unambiguous pruned path" "短名(&U)"
    ::msgcat::mcset zh_tw "&Recent files" "最近檔案(&R)"
    ::msgcat::mcset zh_tw "&Backup files depth" "備存檔深度(&B)"
    ::msgcat::mcset zh_tw "&Locale" "地區化語言"
        # locale names (common)
        ::msgcat::mcset zh_tw "fr locale (fr)" "法文 (fr)"
        ::msgcat::mcset zh_tw "eng locale (eng)" "英文 (eng)"
        ::msgcat::mcset zh_tw "zh_tw locale (zh_tw)" "中文繁體 (zh_tw)"

    # menues.tcl - Windows menu
    ::msgcat::mcset zh_tw "&Maximize" "視窗最大化(&M)"
    ::msgcat::mcset zh_tw "&Split" "分割(&S)"
    ::msgcat::mcset zh_tw "S&plit (side by side)" "左右分割(&p)"
    ::msgcat::mcset zh_tw "Tile all &vertically" "垂直重疊(&v)"
    ::msgcat::mcset zh_tw "Tile all &horizontally" "水平重疊(&v)"
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
        ::msgcat::mcset zh_tw "Changes in the Scipad codebase" "Scipad 程式碼之變動"


# commons
::msgcat::mcset zh_tw "Cancel" "清除"
::msgcat::mcset zh_tw "Cance&l" "清除(&l)"
::msgcat::mcset zh_tw "OK" "確定"
::msgcat::mcset zh_tw "Scilab working" "Scilab 工作中"
::msgcat::mcset zh_tw "All files" "所有檔案"
::msgcat::mcset zh_tw "Add/Change" "添加/變更"
::msgcat::mcset zh_tw "Untitled" "無名"
::msgcat::mcset zh_tw "Close" "關閉"
::msgcat::mcset zh_tw "&Close" "關閉(&C)"


# buffernavigation.tcl
::msgcat::mcset zh_tw "Goto Line?" "跳至第 .. 行"
::msgcat::mcset zh_tw "Go to:" "跳至: "
::msgcat::mcset zh_tw "&logical line" "邏輯行(&l)"
::msgcat::mcset zh_tw "&physical line" "實體行(&p)"
::msgcat::mcset zh_tw "in &function" "函數內(&f)"
::msgcat::mcset zh_tw "in &current file" "目前檔案(&c)"
::msgcat::mcset zh_tw "Outside of function definition" "超出函數定義範圍"


# colorize.tcl
::msgcat::mcset zh_tw "Wait seconds while recolorizing file" "等待幾秒.."
::msgcat::mcset zh_tw "You can't do that while colorization is in progress.\
                    Please try again when finished." \
                   "顏色變換中，無法執行此動作，請靜待..."
::msgcat::mcset zh_tw "Command forbidden during colorization" "顏色變換中，無法執指令"


# completion.tcl
::msgcat::mcset zh_tw "No possible completion" "不可能結束"
::msgcat::mcset zh_tw "possible completions" "可能結束"
::msgcat::mcset zh_tw "Possible completions" "可能結束"


# db_argsmanagement.tcl
::msgcat::mcset zh_tw "Variable:" "變數 :"
::msgcat::mcset zh_tw "Value:" "變數 :"
::msgcat::mcset zh_tw "Get current value from Scilab" "由 Scilab 取得目前數值"



# helps.tcl
::msgcat::mcset zh_tw "About" "有關"
::msgcat::mcset zh_tw aboutme_message \
                    "\n Originated by Joseph Acosta, joeja@mindspring.com.\n\
                    Modified by Scilab Consortium.\n\
                    Revised by Enrico Segre 2003-2005.\n\
                    Miscellaneous improvements, particularly debug tools,\
                    by Fran蔞is Vogel 2004-2005.\n\
                    Swedish translation by Gunnar Lindholm 2005.\n\
                    Norwegian translation by Torbj鷨n Pettersen 2005.\n\
                    German translation by Dirk Reusch 2005.\n\
                    Polish translation by Bartosz Kosiorek 2005.\n\
                    Spanish translation by Juan Nicol嫳 Quesada 2006\n\
                    中文繁體版李運璋  2006."
 

# db_watch.tcl
::msgcat::mcset zh_tw "Watch" "檢視"
::msgcat::mcset zh_tw "Watch variables:" "檢視變數 :"
::msgcat::mcset zh_tw "Remove" "移除"
::msgcat::mcset zh_tw "Call stack:" "呼叫堆疊:"
::msgcat::mcset zh_tw "Show watch variables" "顯示檢視變數"
::msgcat::mcset zh_tw "Hide watch variables" "隱藏檢視變數"
::msgcat::mcset zh_tw "Show call stack" "顯示呼叫堆疊"
::msgcat::mcset zh_tw "Hide call stack" "隱藏呼叫堆疊"
::msgcat::mcset zh_tw "Undefined elements are not legal in variable" "變數中未定義元素"
::msgcat::mcset zh_tw ".\nThis variable will not be updated in Scilab." ".\n此變數無法在 Scilab 內更新"
::msgcat::mcset zh_tw "Illegal undefined element found" "找到不合法之元素"





# findreplace.tcl
::msgcat::mcset zh_tw "The string" "字串"
    ::msgcat::mcset zh_tw "could not be found in" "無法在 .. 找到"
    ::msgcat::mcset zh_tw "could not be found in any of the opened files !" "無法在開啟之檔案內找到 !"
::msgcat::mcset zh_tw "replacements done" "完成取代動作"
::msgcat::mcset zh_tw "Find" "尋找"
::msgcat::mcset zh_tw "No match found in the selection for" "選擇區內找不到相符者"
    ::msgcat::mcset zh_tw "\nWould you like to look for it in the entire text?" "\n希望在全文內搜尋 ?"
::msgcat::mcset zh_tw "You are searching for an empty string!" "你是否尋找一空字串!"
::msgcat::mcset zh_tw "Re&place" "取代(&R)"
::msgcat::mcset zh_tw "Find what:" "尋找:"
::msgcat::mcset zh_tw "Replace with:" "取代成 :"
::msgcat::mcset zh_tw "Find &Next" "尋找下一個(&N)"
::msgcat::mcset zh_tw "Replace &All" "全部取代(&a)"
::msgcat::mcset zh_tw "&Upwards" "向上尋找(&U)"
::msgcat::mcset zh_tw "&Downwards" "向下尋找(&D)" 
::msgcat::mcset zh_tw "Match &whole word only" "只檢驗ㄧ完整之文字(&w)"
::msgcat::mcset zh_tw "Match &case" "大小寫敏感(&c)"
::msgcat::mcset zh_tw "&Regular expression" "正則運算(&Regular expression)"
::msgcat::mcset zh_tw "In all &opened files" "在所有開啟檔案內(&o)"
::msgcat::mcset zh_tw "In &selection only" "在選擇區內(&s)"
::msgcat::mcset zh_tw "In a director&y" "在目錄內(&y)"
::msgcat::mcset zh_tw "Directory r&ecurse search" "目錄內遞迴搜尋(&e)"
::msgcat::mcset zh_tw "In files/file types:" "在以下檔案型態內 :"
::msgcat::mcset zh_tw "In directory:" "在目錄內 :"
::msgcat::mcset zh_tw "Wrapped around" "Wrapped around"
::msgcat::mcset zh_tw "Back at the first file searched!" "回到第一個找到的檔案!"
::msgcat::mcset zh_tw "No more matches" "無法再找到相符的"
::msgcat::mcset zh_tw "The string to regexp search for cannot be compiled by Tcl.\nCheck again, maybe certain characters should be escaped." \
                   "此正則運算無法以 TCL 編譯。\n 再檢查ㄧ次，也許有些字元需加上跳脫字元"

#################以下幾個法文，在經轉碼後會出錯，有法文之行必須剪貼回去####################################

# helps.tcl
::msgcat::mcset zh_tw "About" "有關"
::msgcat::mcset zh_tw aboutme_message \
                    "\n Originated by Joseph Acosta, joeja@mindspring.com.\n\
                    Modified by Scilab Consortium.\n\
                    Revised by Enrico Segre 2003-2005.\n\
                    by Fran�ois Vogel 2004-2005.\n\
                    Swedish translation by Gunnar Lindholm 2005.\n\
                    Norwegian translation by Torbj�rn Pettersen 2005.\n\
                    German translation by Dirk Reusch 2005.\n\
                    Polish translation by Bartosz Kosiorek 2005.\n\
                    Spanish translation by Juan Nicol�s Quesada 2006\n\
                    中文繁體版李運璋  2006."


#################以下尚未翻譯轉檔後必須全數刪除, 否則法文部份會出錯####################################

