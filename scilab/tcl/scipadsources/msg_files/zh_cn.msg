# Scipad message for chinese-simplified
# Scipad 中文讯息档，zh_cn.msg 及 zh_cn.msg
# zh_cn.msg 内容直接由 zh_cn.msg 进行繁体转简体
#
# 如果有人愿意接手新版讯息翻译，请与 李运璋 yjlee123@gmail.com 连络
#

# commons
::msgcat::mcset zh_cn "Cancel" "清除"
::msgcat::mcset zh_cn "Cance&l" "清除(&l)"
::msgcat::mcset zh_cn "OK" "确定"
::msgcat::mcset zh_cn "All files" "所有档案"
::msgcat::mcset zh_cn "Add/Change" "添加/变更"
::msgcat::mcset zh_cn "Add/Chan&ge" "添加/变更(&g)"
::msgcat::mcset zh_cn "&Remove" "移除(&R)"
::msgcat::mcset zh_cn "Rem&ove" "移除(&o)"
::msgcat::mcset zh_cn "Untitled" "无名"
::msgcat::mcset zh_cn "Close" "关闭"
::msgcat::mcset zh_cn "&Close" "关闭(&C)"
::msgcat::mcset zh_cn "Error " "错误 "
::msgcat::mcset zh_cn "The file" "档案"


# buffernavigation.tcl
::msgcat::mcset zh_cn "Hide" "隐藏"
::msgcat::mcset zh_cn "Close this tile, keep content"\
                      "关闭此区块，保留内容"
::msgcat::mcset zh_cn "Close this tile" "关闭此区块"
::msgcat::mcset zh_cn "Goto Line?" "跳至第 .. 行"
::msgcat::mcset zh_cn "Go to:" "跳至: "
::msgcat::mcset zh_cn "&logical line" "逻辑行(&l)"
::msgcat::mcset zh_cn "&physical line" "实体行(&p)"
::msgcat::mcset zh_cn "in &function" "函数内(&f)"
::msgcat::mcset zh_cn "in &current file" "目前档案(&c)"
::msgcat::mcset zh_cn "Outside of function definition"\
                      "超出函数定义范围"


# colorize.tcl
::msgcat::mcset zh_cn "Wait seconds while recolorizing file" "等待几秒.."
::msgcat::mcset zh_cn "You can't do that while colorization is in progress.\
                       Please try again when finished."\
                      "颜色变换中，无法执行此动作，请静待..."
::msgcat::mcset zh_cn "Command forbidden during colorization"\
                      "颜色变换中，无法执指令"


# completion.tcl
::msgcat::mcset zh_cn "No possible completion" "不可能结束"
::msgcat::mcset zh_cn "possible completions" "可能结束"
::msgcat::mcset zh_cn "Possible completions" "可能结束"


# db_argsmanagement.tcl
::msgcat::mcset zh_cn "Add" "新增"
::msgcat::mcset zh_cn "This variable can be watched but cannot be edited!"\
                      "此变数可检视，但不能被编辑!"
::msgcat::mcset zh_cn "A new watch variable will be created."\
                      "新增一检视变数."
::msgcat::mcset zh_cn "Non editable variable" "不可编辑之变数"
::msgcat::mcset zh_cn "Variable:" "变数 :"
::msgcat::mcset zh_cn "Expression:" "算式 :"
::msgcat::mcset zh_cn "Value:" "变数 :"
::msgcat::mcset zh_cn "&Get current value from Scilab"\
                      "由 Scilab 取得目前数值(&G)"
::msgcat::mcset zh_cn "Non watchable variable" "不可检视之变数"
::msgcat::mcset zh_cn "Sorry, variable" "对不起， 变数"
::msgcat::mcset zh_cn "is special and cannot be watched."\
                      "为特殊变数，无法检视"


# db_bptsmanagement.tcl
::msgcat::mcset zh_cn "No breakpoint here!" "此处并无停止点 !"


# db_configureexec.tcl
::msgcat::mcset zh_cn "Configure execution" "设定执行组态..."
::msgcat::mcset zh_cn "Function name:" "函数名称 :"
::msgcat::mcset zh_cn "&Obtain" "得到(&O)"
::msgcat::mcset zh_cn "Input arguments:" "输入参数 :"
::msgcat::mcset zh_cn "The selected function has not been found in the\
                       opened files, maybe it was deleted since last\
                       configuration of the debugger.\nUse button \""\
                      "所选的函数不在开启的档案内，也许在上次侦错器组态之后删除此函数，\
                       \n使用按键 \""
::msgcat::mcset zh_cn "\" to refresh the list of available functions."\
                      "\" 以得到最新函数清单."
::msgcat::mcset zh_cn "Function name or input arguments do not match\
                       definition of the function"\
                      "函数名称或输入参数与定义在"
::msgcat::mcset zh_cn "in the file!\n\nCheck function name and arguments\
                       (names, order) in the configuration dialog.\
                       \nArguments order can be changed using drag and drop\
                       with right mouse button in the arguments listbox."\
                      "档案内之内容不符!\n\n\
                      请检查组态对话盒内之函数名称及输入参数\n\
                      参数之顺序，可以在参数清单按滑鼠右键中拖拉以改变."
::msgcat::mcset zh_cn "Error in selected function name or arguments"\
                      "选择函数名称或参数时发生错误"
::msgcat::mcset zh_cn "At least one input argument does not match definition\
                       of the function"\
                      "最少一个参数与定义在"
::msgcat::mcset zh_cn "in the file!\n\nCheck function arguments (their\
                       names) in the configuration dialog."\
                      "档案内之内容不符!\n\n\
                       请检查组态对话盒内之输入参数."
::msgcat::mcset zh_cn "Error in selected function arguments"\
                      "选择函数参数时发生错误"
::msgcat::mcset zh_cn "Main level code found" "找到最上层程式码"
::msgcat::mcset zh_cn "Scipad ancillary file" "Scipad 辅助档"
::msgcat::mcset zh_cn "The current file contains only Scipad ancillaries\
                       that cannot be debugged in Scipad. Please select\
                       another buffer."\
                      "此档仅为 Scipad 辅助档，无法在 Scipad\
                       内进行侦错，请选其它暂存区 (buffer)."
::msgcat::mcset zh_cn "This file contains main level code.\nDebug as a\
                       single .sce file?"\
                      "这个档案内含主程式码(mail level code).\n以\
                       单一 .sce 档进行侦错?"
::msgcat::mcset zh_cn "No function found" "找不到函数"
::msgcat::mcset zh_cn "This file contains no function and will be treated"\
                      " 此档未含函数，因此将被视为"
::msgcat::mcset zh_cn "as a single .sce file.\nStart debugging?"\
                      "一个单一 .sce 档.\n开始侦错 ?"
::msgcat::mcset zh_cn "You must enable colorization to be able to launch the\
                       debugger!"\
                      "你必须将选色(colorization)功能开启以启动侦错器\
                       !"
::msgcat::mcset zh_cn "The debugger needs colorization"\
                      "侦错器启动需要选色(colorization)功能"
::msgcat::mcset zh_cn "Warning!\n\n  A function missing the closing\
                       \"endfunction\" has been found in the currently\
                       opened files.\n  Such syntax is now obsolete.\n\n \
                       Please terminate function "\
                      " 警告 !\n\n \
                       目前开启的档案中，发现一个不以\
                       \"endfunction\" 指令结束的函数.\n \
                       这类语法目前版本已不允许.\n\n \
                       请以正确指令结束函数 "
::msgcat::mcset zh_cn " and try again." " 并重新测试."
::msgcat::mcset zh_cn "Note: This message might also be triggered by a\
                       string\n  quoted with single quotes and containing\
                       the word \"function\",\n  when \"Colorize\
                       \'strings\'\" is unchecked in the Options menu."\
                      "注意: 如果选单中 \"多色显示(&s) (Colorize\
                       \'strings\')\" 未被勾选，\n \
                       此讯息也可能被单引号 包覆之内含\
                       \"function\" 的字串\n  所触发."
::msgcat::mcset zh_cn "This known bug can be worked around simply by\
                       checking \"Colorize \'&strings\'\"."\
                      "这个 bug 可以很简单的透过勾选\
                       \"多色显示(&s)\" 以避免."
::msgcat::mcset zh_cn "Unterminated function definition found"\
                      "函数定义未结束"
::msgcat::mcset zh_cn "Warning!\n\n  One or more function is defined more\
                       than once in the currently opened files.\n  The\
                       debugger cannot cope with multiple copies of\
                       functions.\n  The following duplicates were\
                       detected:\n"\
                      "警告 !\n\n \
                      一个以上的函数在目前开启的档案中，重复定义.\n \
                       侦错器无法应付多重定义函数.\n \
                       侦测到之重复定义为 :\n"
::msgcat::mcset zh_cn "\n--> Definition of function " "\n--> 函数定义 "
::msgcat::mcset zh_cn " found in:\n" " 位置 :\n"
::msgcat::mcset zh_cn "Duplicate function definitions found"\
                      "重复之函数定义"



# db_debugsession.tcl
::msgcat::mcset zh_cn "Too many breakpointed functions"\
                      "设定停止点(breakpoint)的函数数目太多"
::msgcat::mcset zh_cn "You have currently" "目前共有"
::msgcat::mcset zh_cn "functions in your opened files."\
                      "个设定停止点(breakpoint)函数."
::msgcat::mcset zh_cn "Scilab supports a maximum of" "Scilab 最多允许"
::msgcat::mcset zh_cn "possible breakpointed functions (see help setbpt)."\
                      "个函数设定停止点(breakpoint)."
::msgcat::mcset zh_cn "Step-by-step hence cannot be performed."\
                      "因此无法进行逐步侦错."
::msgcat::mcset zh_cn "This command will actually run to the next breakpoint."\
                      "此指令实际将会进行到下个停止点(breakpoint)."
::msgcat::mcset zh_cn "Break hence cannot be performed." "所以无法暂停."
::msgcat::mcset zh_cn "Too many breakpoints" "太多停止点"
::msgcat::mcset zh_cn "Executing this command would require to set"\
                      "执行此指令必须在开启之档案内设定"
::msgcat::mcset zh_cn "breakpoints in your opened files." "停止点."
::msgcat::mcset zh_cn "possible breakpoints in Scilab (see help setbpt)."\
                      "Scilab 中可能之停止点 (参考 setbpt 指令)."
::msgcat::mcset zh_cn "Cursor must be in a function" "游标必须在函数内"
::msgcat::mcset zh_cn "This command cannot succeed if the function where the\
                       debug just stopped is not opened in Scipad!"\
                      "如果定义函数的档案未在 Scipad\
                       中开启，此指令无法成功执行 !"
::msgcat::mcset zh_cn "Cannot run to return point"\
                      "无法执行到函数回返点 (return point)"
::msgcat::mcset zh_cn "Warning!\nThe line where the debugger just stopped\
                       contains a return statement (either endfunction,\
                       return or resume) but this statement is not the first\
                       one on the line."\
                      "\
                      警告!\n侦错器停止的位置是一函数的回返点\
                       (指令 endfunction, return 或\
                       resume),但却不是此行第一个指令 ."
::msgcat::mcset zh_cn "As a consequence the debugger cannot predict if the\
                       next debug step will really jump outside of the\
                       current function, which is what you just asked the\
                       debugger to do."\
                      "因此，侦错器无法预测是否一个侦错步该跳出目前含数范围，而这是你所要求的."
::msgcat::mcset zh_cn "To avoid this unresolvable case, it is recommended to\
                       place the return statement alone on its line."\
                      "为避免这种无法判断状况，建议将回返点\
                       (指令 endfunction, return 或\
                       resume)独立成一行."
::msgcat::mcset zh_cn "Do you want to see this warning again during this\
                       Scipad session?"\
                      "你希望在 Scipad 中继续看到此警告讯息吗?"
::msgcat::mcset zh_cn "Unresolvable case found while running to return point"\
                      "执行到回返点(return point)时无法判断"
::msgcat::mcset zh_cn "No effect - The debugged file is not stuck"\
                      "无影响 - 档案继续进行侦错"


# db_init.tcl
::msgcat::mcset zh_cn "Please wait..." "稍待片刻..."


# db_states.tcl
::msgcat::mcset zh_cn "Currently no debug session" "目前无侦错段"
::msgcat::mcset zh_cn "Ready to start debug" "准备进行侦错"
::msgcat::mcset zh_cn "Debug in progress" "侦错中"
::msgcat::mcset zh_cn "Scilab is back at the main level now."\
                      "Scilab 返回主执行层 (main level)."



# db_watch.tcl
::msgcat::mcset zh_cn "Watch" "检视"
::msgcat::mcset zh_cn "Watch variables:" "检视变数 :"
::msgcat::mcset zh_cn "A&uto (local variables)" "A&uto 变数(局部变数)"
::msgcat::mcset zh_cn "Au&to watch globals too"\
                      "Au&to 同时检视 globlas 变数"
::msgcat::mcset zh_cn "Generic expressions:" "通用算式 :"
::msgcat::mcset zh_cn "Call stack:" "呼叫堆叠 (call stack) :"
::msgcat::mcset zh_cn "Always on to&p" "永远在最上层(&p)"
::msgcat::mcset zh_cn "Hide watch &variables" "隐藏检视变数"
::msgcat::mcset zh_cn "Hide e&xpressions" "隐藏算式(&x)"
::msgcat::mcset zh_cn "Hide call &stack" "隐藏呼叫堆叠 (call &stack) "
::msgcat::mcset zh_cn "&Dock" "减少(&D)"
::msgcat::mcset zh_cn "&Add" "新增(&A)"
::msgcat::mcset zh_cn "&Filter errors" "&Filter 错误"
::msgcat::mcset zh_cn "Breakpoints" "停止点"
::msgcat::mcset zh_cn "Breakpointed functions" "设定停止点的函数"
::msgcat::mcset zh_cn "Scilab busy indicator" "Scilab 忙碌装况指示"
::msgcat::mcset zh_cn "Debugger state indicator" "侦错器装态指示"
::msgcat::mcset zh_cn "Undefined elements are not legal in variable"\
                      " 变数中出现未定义元素"
::msgcat::mcset zh_cn ".\nThis variable will not be updated in Scilab."\
                      ".\n此变数无法在 Scilab 内更新"
::msgcat::mcset zh_cn "Illegal undefined element found"\
                      "不合法之未定义元素"
::msgcat::mcset zh_cn "evaluates to:" "计算值:"
::msgcat::mcset zh_cn "produces error" "产生错误"
::msgcat::mcset zh_cn "Scipad cannot find source code for the selected\
                       function.\nPlease open the adequate file manually and\
                       try again."\
                      "Scipad\
                      无法找到所选函数的原始档.\n请手动开启洽当的档案."
::msgcat::mcset zh_cn "Missing source code" "找不到原始档"


# filecommands.tcl
::msgcat::mcset zh_cn "New script" "新程序档 (script)"
::msgcat::mcset zh_cn "New view on the same file" "同一档案的另一视窗"
::msgcat::mcset zh_cn "The contents of" "档案"
::msgcat::mcset zh_cn "may have changed, do you wish to save your changes?"\
                      "内容可能已变更，是否需要储存你的变更?"
::msgcat::mcset zh_cn "Save Confirm?" "确定储存档案内容 ?"
::msgcat::mcset zh_cn "Open source of..." "开启函数之原始档..."
::msgcat::mcset zh_cn "Open source of:" "开启函数之原始档 :"
::msgcat::mcset zh_cn "Weird drag&drop" "古怪的 drag&drop"
::msgcat::mcset zh_cn "is neither a valid filename nor a valid directory.\
                       Either you're dragging an object of another type, or\
                       you hit a bug of the dnd mechanism."\
                      "档名或目录名称不合法.\
                       可能是档案型态不对."
::msgcat::mcset zh_cn "File not found" "无此档案"
::msgcat::mcset zh_cn "does not exist anymore. Do you want to create an\
                       empty file with the same name?"\
                      "不再存在.\
                       你希望产生一个同名之新空白档案吗 ?"
::msgcat::mcset zh_cn "Open file" "开启档案"
#::msgcat::mcset zh_cn "The file" "档按" ;# defined in the "commons" section
::msgcat::mcset zh_cn "is already opened! Save the current opened file to\
                       another name and reopen it from disk!"\
                      "已经开启，请将目前档案以另一档名存起，并从硬碟中再开启\
                       !"
::msgcat::mcset zh_cn "File" "档案"
::msgcat::mcset zh_cn "saved" "存起"
::msgcat::mcset zh_cn "cannot be written!" "无法写入 !"
::msgcat::mcset zh_cn "The contents of " "磁碟内之档案内容"
::msgcat::mcset zh_cn "has changed on disk, save it anyway?"\
                      "已经变更, 继续进行储存?"
::msgcat::mcset zh_cn "has been modified outside of Scipad. Do you want to\
                       reload it?"\
                      "已再 Scipad\
                       外变更内容，你希望重新载入到 Scipad 中\
                       ?"
::msgcat::mcset zh_cn "File has changed!" "档案以变更!"
::msgcat::mcset zh_cn "Save as" "存成"
::msgcat::mcset zh_cn "Scilab files" "Scilab 档"
::msgcat::mcset zh_cn "Scicos files" "Scicos 档"
::msgcat::mcset zh_cn "XML files" "XML 档"
::msgcat::mcset zh_cn "Wait seconds while loading and colorizing file"\
                      "载入档案时，稍待片刻"
::msgcat::mcset zh_cn "The contents of this file may have changed, do you\
                       wish to save your changes?"\
                      "档案内容已变更，你希储存变更吗 ?"
::msgcat::mcset zh_cn "Revert" "恢复"
::msgcat::mcset zh_cn "to saved?" "原状后存起?"
::msgcat::mcset zh_cn "Revert Confirm?" "确定恢复原状 ?"
::msgcat::mcset zh_cn "Unreadable file" "档案无法读取!"
::msgcat::mcset zh_cn "exists but is not readable!" "档案存在但不可读!"
::msgcat::mcset zh_cn "This file is rather large and colorization might take\
                       some time.\n\nColorize anyway?"\
                      "此档相当大，若完成全部文字色彩可能费时.\n\n，你确定继续进行文字色彩?"
::msgcat::mcset zh_cn "File size warning" "档案过大警告"
::msgcat::mcset zh_cn "Windows shortcut file" "Windows 捷径"
#::msgcat::mcset zh_cn "The file" "档案" ;# defined in the "commons" section
::msgcat::mcset zh_cn "is a Windows shortcut and will not be opened!"\
                      "为 Windows 捷径，无法开启 !"


# findreplace.tcl
::msgcat::mcset zh_cn "Dialog box already open!"\
                      "对话盒 (Dialog box)已开启!"
::msgcat::mcset zh_cn "The string" "字串"
::msgcat::mcset zh_cn "could not be found in" "无法在 .. 找到"
::msgcat::mcset zh_cn "could not be found in any of the opened files !"\
                      "无法在开启之档案内找到 !"
::msgcat::mcset zh_cn "replacements done" "完成取代动作"
::msgcat::mcset zh_cn "Find" "寻找"
::msgcat::mcset zh_cn "No match found in the selection for"\
                      "选择区内找不到相符者"
::msgcat::mcset zh_cn "\nWould you like to look for it in the entire text?"\
                      "\n希望在全文内搜寻 ?"
::msgcat::mcset zh_cn "You are searching for an empty string!"\
                      "你是否寻找一空字串!"
::msgcat::mcset zh_cn "Re&place" "取代(&R)"
::msgcat::mcset zh_cn "Find what:" "寻找:"
::msgcat::mcset zh_cn "Replace with:" "取代成 :"
::msgcat::mcset zh_cn "Find &Next" "寻找下一个(&N)"
::msgcat::mcset zh_cn "Replace &All" "全部取代(&a)"
::msgcat::mcset zh_cn "Direction" "方向"
::msgcat::mcset zh_cn "&Upwards" "向上寻找(&U)"
::msgcat::mcset zh_cn "&Downwards" "向下寻找(&D)"
::msgcat::mcset zh_cn "Text types" "文字型态"
::msgcat::mcset zh_cn "Find options" "档案选项"
::msgcat::mcset zh_cn "Match &whole word only"\
                      "只检验ㄧ完整之文字(&w)"
::msgcat::mcset zh_cn "Match &case" "大小写敏感(&c)"
::msgcat::mcset zh_cn "&Regular expression" "正则运算(&Regular expression)"
::msgcat::mcset zh_cn "In all &opened files" "在所有开启档案内(&o)"
::msgcat::mcset zh_cn "In &selection only" "在选择区内(&s)"
::msgcat::mcset zh_cn "In typed te&xt" "在特定型态文字内(&x)"
::msgcat::mcset zh_cn "In a director&y" "在目录内(&y)"
::msgcat::mcset zh_cn "Find in files search options" "档案搜索选项"
::msgcat::mcset zh_cn "Directory r&ecurse search" "目录内递回搜寻(&e)"
::msgcat::mcset zh_cn "Include &hidden files" "包含隐藏档(&h)"
::msgcat::mcset zh_cn "In files/file types:" "在以下档案型态内 :"
::msgcat::mcset zh_cn "In directory:" "在目录内 :"
::msgcat::mcset zh_cn "Search for file names" "以档名搜索"
::msgcat::mcset zh_cn "You have given an empty search string. Are you sure\
                       you want to search among file names?"\
                      "你可能输入一个空档名. 确定继续 ?"
::msgcat::mcset zh_cn "Wrapped around" "绕过去"
::msgcat::mcset zh_cn "Back at the first file searched!"\
                      "回到第一个找到的档案!"
::msgcat::mcset zh_cn "No more matches" "无法再找到相符的"
::msgcat::mcset zh_cn "The string to regexp search for cannot be compiled by\
                       Tcl.\nCheck again, maybe certain characters should be\
                       escaped."\
                      "此正则运算无法以 TCL 编译。\n\
                      再检查ㄧ次，也许有些字元需加上跳脱字元"
::msgcat::mcset zh_cn "Any single character" "任一单一字元"
::msgcat::mcset zh_cn "Zero or more" "零次以上"
::msgcat::mcset zh_cn "One or more" "一次以上"
::msgcat::mcset zh_cn "Zero or one" "零或一次"
::msgcat::mcset zh_cn "Any one character in the set" "集合内任一字元"
::msgcat::mcset zh_cn "Any one character not in the set"\
                      "任一不在集合内之字元"
::msgcat::mcset zh_cn "Or" "或"
::msgcat::mcset zh_cn "Beginning of line" "一行最前"
::msgcat::mcset zh_cn "End of line" "一行最后"
::msgcat::mcset zh_cn "Beginning of word" "一字最前"
::msgcat::mcset zh_cn "End of word" "一字最后"
::msgcat::mcset zh_cn "A letter" "一文字字元 (letter)"
::msgcat::mcset zh_cn "A decimal digit" "一数字字元 (decimal digit)"
::msgcat::mcset zh_cn "An alphanumeric (letter or digit)" "英文字母"
::msgcat::mcset zh_cn "A space or tab character"\
                      "空白(space) 或 对齐(tab) 字元"
::msgcat::mcset zh_cn "A rational number" "一有理数"
::msgcat::mcset zh_cn "A floating point number" "一实数"
::msgcat::mcset zh_cn "A Scilab comment" "Scilab 指令"
::msgcat::mcset zh_cn "A Scilab name" "Scilab 变数名"
::msgcat::mcset zh_cn "A Scilab continued line (with dots)"\
                      "Scilab 连续行 (行后为 .. 字串)"


# findinfiles.tcl
::msgcat::mcset zh_cn "Directory access issue" "目录存取问题"
::msgcat::mcset zh_cn "The directory you specified cannot be reached. The\
                       search won't bring any result."\
                      "所指定的目录无法存取. 搜寻无结果."
::msgcat::mcset zh_cn "Some directories or files could not be open and were\
                       ignored during search.\n You might miss read access\
                       to them.\n\n The following ones were ignored"\
                      "部分目录或档案无法开启，因此搜索过程中忽略它们.\n\
                       或许是读取允许设定不对.\n\n\
                       以下为忽略之部分"
::msgcat::mcset zh_cn "Ignored files" "忽略之档案"
::msgcat::mcset zh_cn "Searching in file:" "档案内搜索 :"
::msgcat::mcset zh_cn "End of file search:" "档案搜索结束:"
::msgcat::mcset zh_cn "matches found" "符合项"
::msgcat::mcset zh_cn "searched files" "搜索过之档案"
::msgcat::mcset zh_cn "Constructing file list:" "建立档案清单:"
::msgcat::mcset zh_cn "&Previous" "向前(&P)"
::msgcat::mcset zh_cn "&Next" "向后(&N)"
::msgcat::mcset zh_cn "Scipad search results" "Scipad 搜索结果"


# fonts.tcl
::msgcat::mcset zh_cn "Choose font" "选择字型"
::msgcat::mcset zh_cn "Family" "字族"
::msgcat::mcset zh_cn "Style" "风格(Style)"
::msgcat::mcset zh_cn "&Bold" "粗体(&B)"
::msgcat::mcset zh_cn "&Italic" "斜体(&I)"
::msgcat::mcset zh_cn "&Underline" "底线(&U)"
::msgcat::mcset zh_cn "Size" "大小"
::msgcat::mcset zh_cn "&Defaults" "内定(&D)"
::msgcat::mcset zh_cn "Use as..." "做为..."
::msgcat::mcset zh_cn "&Text font" "文字字型(&T)"
::msgcat::mcset zh_cn "&Menu font" "选单字型(&M)"
::msgcat::mcset zh_cn "Sample" "例"
::msgcat::mcset zh_cn "Font size" "字体大小"
::msgcat::mcset zh_cn "Revert fonts" "回复成内定字体"
::msgcat::mcset zh_cn "Are you sure you want to revert all fonts to defaults?"\
                      "你确定要回复成内定字体 ?"


# FormatStringsForWatch.sci
::msgcat::mcset zh_cn " what you try to watch is not supported by the\
                       debugger"\
                      " 你所要检视的内容还未为 Scipad\
                       侦错器支持"
::msgcat::mcset zh_cn " what you try to watch is of type "\
                      " 你所检视的型态为 "
::msgcat::mcset zh_cn " - this type is not supported by the debugger"\
                      " - 此型态还未为 Scipad 侦错器支持"
::msgcat::mcset zh_cn "Type:" "型态 :"
::msgcat::mcset zh_cn "(unsupported)" "(未支持)"
::msgcat::mcset zh_cn "size:" "大小 :"
::msgcat::mcset zh_cn "real" "实数"
::msgcat::mcset zh_cn "complex" "复数"
::msgcat::mcset zh_cn "elements" "元素"
::msgcat::mcset zh_cn "graphic handle" "图形  handle"
::msgcat::mcset zh_cn "uncompiled function" "未编译之函数"
::msgcat::mcset zh_cn "compiled function" "已编译函数"
::msgcat::mcset zh_cn "library" "程式库"
::msgcat::mcset zh_cn "pointer" "指标"
::msgcat::mcset zh_cn "size implicit index" "大小之隐含指标"
::msgcat::mcset zh_cn "primitive" "基础"


# FormatWhereForWatch.sci
::msgcat::mcset zh_cn "Breakpoint" "停止点"
::msgcat::mcset zh_cn "called at line" "在第"
::msgcat::mcset zh_cn "of exec file" "行被执行程序呼叫"
::msgcat::mcset zh_cn "of execstr instruction" "行被 execstr 的指令呼叫"
::msgcat::mcset zh_cn "called under pause" "在 pause 状态下被呼叫"
::msgcat::mcset zh_cn "of macro" "被巨集呼叫"


# infomessages.tcl
::msgcat::mcset zh_cn "Line:" "行:"
::msgcat::mcset zh_cn "Column:" "栏:"
::msgcat::mcset zh_cn "Line" "第"
::msgcat::mcset zh_cn "in" " 行在 "
::msgcat::mcset zh_cn "Logical line:" "逻辑行:"
::msgcat::mcset zh_cn " \[ReadOnly\]" " \[仅可读\]"
::msgcat::mcset zh_cn " (modified)" " (修改过)"


# inputtext.tcl
::msgcat::mcset zh_cn "No corresponding <" "没找到 <"
::msgcat::mcset zh_cn "> found!" ">  !"
::msgcat::mcset zh_cn "Code editing is not allowed during debug!"\
                      "侦错过程中，无法编辑程式 !"


# menues.tcl
::msgcat::mcset zh_cn "&File" "档案(&F)"
::msgcat::mcset zh_cn "&Edit" "编辑(&E)"
::msgcat::mcset zh_cn "&Search" "搜寻(&R)"
::msgcat::mcset zh_cn "E&xecute" "执行(&x)"
::msgcat::mcset zh_cn "&Debug" "侦错(&D)"
::msgcat::mcset zh_cn "S&cheme" "程式语言(&L)"
::msgcat::mcset zh_cn "&Options" "选项(&O)"
::msgcat::mcset zh_cn "&Windows" "视窗(&W)"
::msgcat::mcset zh_cn "&Help" "辅助(&H)"

# menues.tcl - File menu
::msgcat::mcset zh_cn "&New" "新档(&N)"
::msgcat::mcset zh_cn "&Open..." "开启旧档(&O)..."
::msgcat::mcset zh_cn "Open &in new" "在新视窗开启(&u)"
::msgcat::mcset zh_cn "&horizontal tile" "水平重叠(&h)"
::msgcat::mcset zh_cn "&vertical tile" "垂直重叠(&v)"
::msgcat::mcset zh_cn "Op&en source of..." "开启...的原始码(&e)"
::msgcat::mcset zh_cn "&Save" "存档(&S)"
::msgcat::mcset zh_cn "Save &as..." "另存新档(&a)"
::msgcat::mcset zh_cn "Save a&ll" "存起所有档案(&l)"
::msgcat::mcset zh_cn "&Revert..." "回复(&v)..."
::msgcat::mcset zh_cn "Import &Matlab file..." "载入 &Matlab 档..."
::msgcat::mcset zh_cn "Create help s&keleton..." "产生文件架构(&k)..."
::msgcat::mcset zh_cn "Compile as &help page" "编译出文件(&h)"
::msgcat::mcset zh_cn "Open &function source" "编辑函数原始码(&f)"
::msgcat::mcset zh_cn "Print Se&tup" "印表机设定(&t)"
::msgcat::mcset zh_cn "&Print" "列印(&P)"
::msgcat::mcset zh_cn "&Close file" "关密档案(&C)"
::msgcat::mcset zh_cn "E&xit" "结束(&x)"

# menues.tcl - Edit menu
::msgcat::mcset zh_cn "&Undo" "回复(&U)"
::msgcat::mcset zh_cn "&Redo" "再回复(&R)"
::msgcat::mcset zh_cn "Cu&t" "剪下(&t)"
::msgcat::mcset zh_cn "B&lock cut" "删除文字块(&l)"
::msgcat::mcset zh_cn "&Copy" "复制(&C)"
::msgcat::mcset zh_cn "&Paste" "贴上(&P)"
::msgcat::mcset zh_cn "&Block paste" "复制文字块(&B)"
::msgcat::mcset zh_cn "&Delete" "删除(&D)"
::msgcat::mcset zh_cn "Select &All" "全选(&A)"
::msgcat::mcset zh_cn "Insert &Time/Date" "插入时间/日期(&T)"
::msgcat::mcset zh_cn "Co&mment selection" "选择区成为注解(&m)"
::msgcat::mcset zh_cn "U&ncomment selection"\
                      "清除选择区之注解标志(&n)"
::msgcat::mcset zh_cn "&Indent selection" "缩排(&I)"
::msgcat::mcset zh_cn "Unin&dent selection" "清除缩排(&d)"

# menues.tcl - Search menu
::msgcat::mcset zh_cn "&Find..." "寻找(&F)..."
::msgcat::mcset zh_cn "Find &Next" "寻找下一个(&N)"
::msgcat::mcset zh_cn "&Replace..." "取代(&R)"
::msgcat::mcset zh_cn "&Goto Line..." "跳至第...行(&G)"

# menues.tcl - Exec menu
::msgcat::mcset zh_cn "&Load into Scilab" "载入 Scilab 中(&L)"
::msgcat::mcset zh_cn "&Evaluate selection" "执行选择之部分(&E)"

# menues.tcl - Debug menu
::msgcat::mcset zh_cn "&Insert/Remove breakpoint" "插入/移除停止点(&I)"
::msgcat::mcset zh_cn "Remove &all breakpoints" "清除所有停止点(&a)"
::msgcat::mcset zh_cn "&Configure execution..." "设定执行组态(&C)..."
::msgcat::mcset zh_cn "Go to next b&reakpoint" "跳至下一停止点(&r)"
::msgcat::mcset zh_cn "&Step by step" "一步一步(&S)"
::msgcat::mcset zh_cn "Step &into" "进入(&i)"
::msgcat::mcset zh_cn "Step o&ver" "跳过(&v)"
::msgcat::mcset zh_cn "Step &out" "跳出(&o)"
::msgcat::mcset zh_cn "Run to re&turn point"\
                      "执行至返回点 (re&turn point)"
::msgcat::mcset zh_cn "Run to c&ursor" "执行至游标处(&u)"
::msgcat::mcset zh_cn "G&o on ignoring any breakpoint"\
                      "执行并忽略所有停止点"
::msgcat::mcset zh_cn "Show &watch" "检查变数(&w)"
::msgcat::mcset zh_cn "&Break" "终止(&B)"
::msgcat::mcset zh_cn "Cance&l debug" "取消侦错(&l)"

#    ::msgcat::mcset zh_cn "language scheme" "程式语言"
# menues.tcl - Scheme menu
::msgcat::mcset zh_cn "S&cilab" "S&cilab"
::msgcat::mcset zh_cn "&XML" "&XML"
::msgcat::mcset zh_cn "&none" "无(&n)"
::msgcat::mcset zh_cn "&Colorize" "多色显示(&C)"

# menues.tcl - Options menu
::msgcat::mcset zh_cn "&Fonts..." "字体(&F)..."
::msgcat::mcset zh_cn "&Colors" "颜色(&C)"
::msgcat::mcset zh_cn "Colori&ze" "彩色文字(&z)"
::msgcat::mcset zh_cn "&Always" "永远要"
::msgcat::mcset zh_cn "Ask for &big files" "档案过大时询问"
::msgcat::mcset zh_cn "&Never" "不须要"
::msgcat::mcset zh_cn "Colorize \'&strings\'" "多色显示"
::msgcat::mcset zh_cn "Line n&umbers" "行号(&u)"
::msgcat::mcset zh_cn "&Hide" "隐藏(H)"
::msgcat::mcset zh_cn "&Left aligned" "左边对齐(L)"
::msgcat::mcset zh_cn "&Right aligned" "右边对齐(R)"
::msgcat::mcset zh_cn "Show c&ontinued lines" "显示连续行(&o)"
::msgcat::mcset zh_cn "Word &wrap" "文字环绕(&W)"
::msgcat::mcset zh_cn "&Double-click behavior" "滑鼠双击行为(&D)"
::msgcat::mcset zh_cn "&Linux" "&Linux"
::msgcat::mcset zh_cn "Windo&ws" "Windo&ws"
::msgcat::mcset zh_cn "&Scilab" "&Scilab"
::msgcat::mcset zh_cn "&Tabs and indentation" "定位点及缩排(&T)"
::msgcat::mcset zh_cn "Tab inserts &spaces" "定位点之空白键数目(&s)"
::msgcat::mcset zh_cn "&Indentation spaces" "缩排之空白键数目(&I)"
::msgcat::mcset zh_cn "&Tab size (characters)" "定位点大小(&T)"
::msgcat::mcset zh_cn "Use &keyword indentation" "使用关键字定位 (&k)"
::msgcat::mcset zh_cn "Com&pletion" "结束(&p)"
::msgcat::mcset zh_cn "&Tab" "&Tab"
::msgcat::mcset zh_cn "&Control-Tab" "&Control-Tab"
::msgcat::mcset zh_cn "&Alt-Tab" "&Alt-Tab"
::msgcat::mcset zh_cn "&Shift-Tab" "&Shift-Tab"
::msgcat::mcset zh_cn "C&ontrol-Alt-Tab" "C&ontrol-Alt-Tab"
::msgcat::mcset zh_cn "S&hift-Control-Tab" "S&hift-Control-Tab"
::msgcat::mcset zh_cn "Sh&ift-Alt-Tab" "Sh&ift-Alt-Tab"
::msgcat::mcset zh_cn "Shi&ft-Control-Alt-Tab" "Shi&ft-Control-Alt-Tab"
::msgcat::mcset zh_cn "File&names" "档案名称(&n)"
::msgcat::mcset zh_cn "&Full path" "全名(&F)"
::msgcat::mcset zh_cn "Full path if &ambiguous"\
                      "如果名称冲突时使用全名(&a)"
::msgcat::mcset zh_cn "&Unambiguous pruned path" "短名(&U)"
::msgcat::mcset zh_cn "Windows menu &sorting" "视窗排序方法(&s)"
::msgcat::mcset zh_cn "&File opening order" "依档案开启顺序(&F)"
::msgcat::mcset zh_cn "&Alphabetically" "依字母顺序(&A)"
::msgcat::mcset zh_cn "&Most recently used" "依使用顺序(&M)"
::msgcat::mcset zh_cn "&Recent files" "最近档案(&R)"
::msgcat::mcset zh_cn "&Backup files depth" "备存档深度(&B)"
::msgcat::mcset zh_cn "&Locale" "地区化语言"
::msgcat::mcset zh_cn "Execution &errors" "执行错误(&e)"
::msgcat::mcset zh_cn "In Scilab &shell only" "只出现在Scilab &shell "
::msgcat::mcset zh_cn "Copied in a &message box" "复制到讯息区(&m)"

# menues.tcl - Windows menu
::msgcat::mcset zh_cn "&Maximize" "视窗最大化(&M)"
::msgcat::mcset zh_cn "&Split" "分割(&S)"
::msgcat::mcset zh_cn "S&plit (side by side)" "左右分割(&p)"
::msgcat::mcset zh_cn "Sp&lit file" "分割档案(&l)"
::msgcat::mcset zh_cn "Spl&it file (side by side)" "左右分割档案(&i)"
::msgcat::mcset zh_cn "Tile all &vertically" "垂直重叠(&v)"
::msgcat::mcset zh_cn "Tile all &horizontally" "水平重叠(&h)"
::msgcat::mcset zh_cn "Space sashes &evenly" "视窗均分(&e)"
::msgcat::mcset zh_cn "Opened Files" "开启之档案"

# menues.tcl - Help menu
::msgcat::mcset zh_cn "&Help..." "Scipad 参考..."
::msgcat::mcset zh_cn "&What's?..." "这是..?(&W)(查询选择之函数)"
::msgcat::mcset zh_cn "&About" "有关(&A)"
::msgcat::mcset zh_cn "Adding &translations..." "协助翻译 Scipad...(&T)"
::msgcat::mcset zh_cn "Adding Scipad Translations..." "协助翻译 Scipad..."
::msgcat::mcset zh_cn "&Bugs \& wishlist" "已知臭虫及计画工作(&B)"
::msgcat::mcset zh_cn "Scipad Known bugs" "Scipad 已知臭虫 "
::msgcat::mcset zh_cn "Change&log" "变动(&l)"
::msgcat::mcset zh_cn "Changes in the Scipad codebase"\
                      "Scipad 程式码之变动"


# popupmenus.tcl
::msgcat::mcset zh_cn "Open the source of" "开启 .. 的原始档"
::msgcat::mcset zh_cn "AddWatch" "新增观测值"
::msgcat::mcset zh_cn "AddGenExp" "新增通用算式"
::msgcat::mcset zh_cn "Jump to" "跳到"


# print.tcl
::msgcat::mcset zh_cn "Print Setup" "印表机设定"
::msgcat::mcset zh_cn "Print Command: " "列印指令 : "


# scilabexec.tcl
::msgcat::mcset zh_cn "No point in loading an empty file!"\
                      "无法载入一个空档案!"
::msgcat::mcset zh_cn "Silent file save failed" "无名档案无法存起"
::msgcat::mcset zh_cn "File could not be saved in a temporary\
                       location.\nOverwrite original file?"\
                      "暂存位置内无法存档. \n覆盖原始档案 ?"
::msgcat::mcset zh_cn "Exec done" "执行结束"
::msgcat::mcset zh_cn "Matlab files" "Matlab 档"
::msgcat::mcset zh_cn "Matlab file to convert" "转换 Matlab 档"
::msgcat::mcset zh_cn "Scilab is converting, please hold on..."\
                      "Scilab 转换中，稍待..."
::msgcat::mcset zh_cn "Matlab file import" "载入 Matlab 档"
::msgcat::mcset zh_cn "Conversion of the file failed, see the Scilab window\
                       for details -- Perhaps report the error text and the\
                       offending Matlab file to \
                      http://scilabsoft.inria.fr/bugzilla_bug_II/index.cgi"\
                      "档案转换失败，详情请参考 Scilab\
                       视窗内容 -- 也许可将错误讯息及对应之\
                       Matlab 档到\
                       http://scilabsoft.inria.fr/bugzilla_bug_II/index.cgi\
                       位置报告"
::msgcat::mcset zh_cn "Please select destination path for the xml source of\
                       the help file"\
                      "请选择 xml help 档的路径位置 "
::msgcat::mcset zh_cn "Older version of" "找到档案"
::msgcat::mcset zh_cn "found!" "的旧版本!"
::msgcat::mcset zh_cn "An old version of" "档案"
::msgcat::mcset zh_cn "already exists: open the old file instead?"\
                      "的旧版本已经存在: 开启旧档?"
::msgcat::mcset zh_cn "Existing file" "已存在之档案"
::msgcat::mcset zh_cn "New skeleton" "新的档案骨架"
::msgcat::mcset zh_cn "ScilabEval command cannot be passed to Scilab!"\
                      "ScilabEval 指令无法传送至 Scilab!"
::msgcat::mcset zh_cn "impossibleScilabEval_message"\
                      "警告: 指令无法传送至 Scilab,\
                       因此忽略.\n\n  最有可能是你没有 Scilab\
                       的 TMPDIR 目录的写入权限. \
                       可能产生不可预期的结果.\n\n \
                       出问题的指令为:\n\n-->"
::msgcat::mcset zh_cn "impossibleScilabEval_message2"\
                      "警告: 指令无法传送至 Scilab, 因此忽略， \
                       原因是指令过长.\n\n \
                       可能产生不可预期的结果.\n\n \
                      如果正在侦错，请试著移除观测之变数.\n\n \
                       出问题的指令为:\n\n-->"
::msgcat::mcset zh_cn "(end of command skipped)" "(后面指令跳过)"
::msgcat::mcset zh_cn "Scilab working" "Scilab 工作中"
::msgcat::mcset zh_cn "Scilab is working, please wait for the prompt to\
                       execute this command!"\
                      "Scilab 工作中，请等候此指令之输出结果 !"
::msgcat::mcset zh_cn "Scilab is working, wait for the prompt to load file"\
                      "Scilab 工作中，请等候载入档案之结果"
::msgcat::mcset zh_cn "Scilab is working, wait for the prompt to execute the\
                       selection."\
                      "Scilab\
                       工作中，请等候执行选择之指令部份."
::msgcat::mcset zh_cn "Scilab is working, wait for the prompt to convert a\
                       Matlab file."\
                      "Scilab 工作中，请等候转换 Matlab 档案结果."
::msgcat::mcset zh_cn "Scilab is working, wait for the prompt to compile the\
                       help file."\
                      "Scilab 工作中，请等候编译说明档之结果."
::msgcat::mcset zh_cn "Scilab is working, wait for the prompt to issue a\
                       debugger command."\
                      "Scilab 工作中，请等候侦错指令之结果."
::msgcat::mcset zh_cn "Scilab execution error" "Scilab  执行错误"
::msgcat::mcset zh_cn "The shell reported an error while trying to execute "\
                      "执行  "
::msgcat::mcset zh_cn ": error " "时，Scilab Shell 发现错误: 错误 "
::msgcat::mcset zh_cn "at line " "第 "
::msgcat::mcset zh_cn " of " " 行，位于 "
::msgcat::mcset zh_cn "Execution aborted!" "执行中断!"


# scipad.tcl
::msgcat::mcset zh_cn "NOT CONNECTED TO SCILAB" "未与 SCILAB 连结"

# whichfun.tcl
::msgcat::mcset zh_cn "Being at line" "第"
::msgcat::mcset zh_cn ", function" "行, 函数"
::msgcat::mcset zh_cn "begins at" "起始位置为"
::msgcat::mcset zh_cn ", and there are" ", 共有"
::msgcat::mcset zh_cn "continued lines, i.e. we are at line"\
                      "行连续行，也就是目前为第"
::msgcat::mcset zh_cn "of" "行，此行位于"
::msgcat::mcset zh_cn "Function" "函数"
::msgcat::mcset zh_cn "line" "行"
::msgcat::mcset zh_cn "The cursor is not currently inside a function body."\
                      "游标目前不在函数内部."
::msgcat::mcset zh_cn "Which function?" "哪一个函数 ?"



#preference variables description
::msgcat::mcset zh_cn "BGCOLOR" "编辑区背景色"
::msgcat::mcset zh_cn "BGLNMARGCOLOR" "行号背景色"
::msgcat::mcset zh_cn "FGCOLOR" "文字"
::msgcat::mcset zh_cn "FGLNMARGCOLOR" "行号"
::msgcat::mcset zh_cn "CURCOLOR" "游标"
::msgcat::mcset zh_cn "PARCOLOR" "括符 ()"
::msgcat::mcset zh_cn "BRAKCOLOR" "中括符 \\\[\\\]"
::msgcat::mcset zh_cn "BRACCOLOR" "大括符  {}"
::msgcat::mcset zh_cn "PUNCOLOR" "标点符号"
::msgcat::mcset zh_cn "INTFCOLOR" "内部函数"
::msgcat::mcset zh_cn "COMMCOLOR" "指令"
::msgcat::mcset zh_cn "SCICCOLOR" "Scicos 函数"
::msgcat::mcset zh_cn "OPCOLOR" "运算子"
::msgcat::mcset zh_cn "QTXTCOLOR" "字串文字"
::msgcat::mcset zh_cn "REMCOLOR" "注解"
::msgcat::mcset zh_cn "XMLCOLOR" "XML 标签"
::msgcat::mcset zh_cn "NUMCOLOR" "数字"
::msgcat::mcset zh_cn "SELCOLOR" "选择区"
::msgcat::mcset zh_cn "BREAKPOINTCOLOR" "停止点背景"
::msgcat::mcset zh_cn "LFUNCOLOR" "程式库函数"
::msgcat::mcset zh_cn "PDEFCOLOR" "系统变数"
::msgcat::mcset zh_cn "FOUNDTEXTCOLOR" "符合条件之文字"
::msgcat::mcset zh_cn "CONTLINECOLOR" "连续行"
::msgcat::mcset zh_cn "USERFUNCOLOR" "使用者定义之函数"



#overriding tcl-core dialogs (many are actually already found in
#  TKDIR/msgs/fr.msg)
::msgcat::mcset zh_cn "&Directory:" "目录(&D) :"
::msgcat::mcset zh_cn "File &name:" "档案名称 (&n):"
::msgcat::mcset zh_cn "Files of &type:" "档案型态(&t) :"
#::msgcat::mcset zh_cn "&Save" "存档(&S)" # already included
::msgcat::mcset zh_cn "&Open" "开启(&O)"
::msgcat::mcset zh_cn "Open" "开启"
::msgcat::mcset zh_cn "Save As" "存成"
::msgcat::mcset zh_cn "&Cancel" "取消(&C)"
::msgcat::mcset zh_cn "&Selection:" "选择(&S) :"
::msgcat::mcset zh_cn "&Yes" "&Yes"
::msgcat::mcset zh_cn "&No" "&No"
::msgcat::mcset zh_cn "&Red" "红(&R)"
::msgcat::mcset zh_cn "&Green" "绿(&G)"
::msgcat::mcset zh_cn "&Blue" "蓝(&B)"
::msgcat::mcset zh_cn "Directory \"%1\$s\" does not exist."\
                      "附录 \"%1\$s\"不存在."

# helps.tcl
::msgcat::mcset zh_cn "About" "有关"
::msgcat::mcset zh_cn "aboutme_message"\
                      "\n 原版为 Joseph Acosta , joeja@mindspring.com.\
                       所开发\n 由 Scilab Consortium 修改.\n Enrico\
                       Segre 2003-2006 进一步改写.\n\
                       其他贡献者，,\n 侦错工具:\n  François\
                       Vogel 2004-2007 \n 各国语文翻译:\n 瑞典文\
                       Gunnar Lindholm 2005.\n 挪威文 Torbjørn Pettersen\
                       2005.\n 德文   Dirk Reusch 2005.\n 波兰文\
                       Bartosz Kosiorek 2005.\n 西班牙文 Juan Nicolás\
                       Quesada 2006.\n 中文 李运璋 2007.\n 丹麦文\
                       Kim B. Hansen 2007."
