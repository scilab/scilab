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

# Scipad message for chinese-simplified
# Scipad 简体中文界面，zh_cn.msg
# 翻译者：于仕琪 <shiqi.yu@gmail.com> 2008
#        李运璋 <yjlee123@gmail.com> 2007

# commons
::msgcat::mcset zh_CN "Cancel" "取消"
::msgcat::mcset zh_CN "Cance&l" "取消(&l)"
::msgcat::mcset zh_CN "OK" "确定"
::msgcat::mcset zh_CN "All files" "所有文件"
::msgcat::mcset zh_CN "Add/Change" "添加/改变"
::msgcat::mcset zh_CN "Add/Chan&ge" "添加/改变(&g)"
::msgcat::mcset zh_CN "&Remove" "移除(&R)"
::msgcat::mcset zh_CN "Rem&ove" "移除(&o)"
::msgcat::mcset zh_CN "Untitled" "无名"
::msgcat::mcset zh_CN "Close" "关闭"
::msgcat::mcset zh_CN "&Close" "关闭(&C)"
::msgcat::mcset zh_CN "Error " "错误 "
::msgcat::mcset zh_CN "The file" "文件"


# buffernavigation.tcl
::msgcat::mcset zh_CN "Hide" "隐藏"
::msgcat::mcset zh_CN "Close this tile, keep content"\
                      "关闭此区块，保留内容"
::msgcat::mcset zh_CN "Close this tile" "关闭此区块"
::msgcat::mcset zh_CN "Goto Line?" "跳至第 .. 行"
::msgcat::mcset zh_CN "Go to:" "跳至: "
::msgcat::mcset zh_CN "&logical line" "逻辑行(&l)"
::msgcat::mcset zh_CN "&physical line" "实体行(&p)"
::msgcat::mcset zh_CN "in &function" "函数内(&f)"
::msgcat::mcset zh_CN "in &current file" "目前文件(&c)"
::msgcat::mcset zh_CN "Outside of function definition"\
                      "超出函数定义范围"


# colorize.tcl
::msgcat::mcset zh_CN "Wait seconds while recolorizing file" "执行代码高亮中，请稍等..."
::msgcat::mcset zh_CN "You can't do that while colorization is in progress.\
                       Please try again when finished."\
                      "代码高亮切换中，无法执行此动作，请在颜色变换结束后重试。"
::msgcat::mcset zh_CN "Command forbidden during colorization"\
                      "代码高亮切换中，无法执命令"


# completion.tcl
::msgcat::mcset zh_CN "No possible completion" "不可能结束"
::msgcat::mcset zh_CN "possible completions" "可能结束"
::msgcat::mcset zh_CN "Possible completions" "可能结束"


# db_argsmanagement.tcl
::msgcat::mcset zh_CN "Add" "添加"
::msgcat::mcset zh_CN "This variable can be watched but cannot be edited!"\
                      "此变量可以被查看，但不能被编辑!"
::msgcat::mcset zh_CN "A new watch variable will be created."\
                      "一新查看变量将被创建。"
::msgcat::mcset zh_CN "Non editable variable" "不可编辑的变量"
::msgcat::mcset zh_CN "Variable:" "变量："
::msgcat::mcset zh_CN "Expression:" "表达式："
::msgcat::mcset zh_CN "Value:" "值："
::msgcat::mcset zh_CN "&Get current value from Scilab"\
                      "由 Scilab 获取目前数值(&G)"
::msgcat::mcset zh_CN "Non watchable variable" "不可查看的变量"
::msgcat::mcset zh_CN "Sorry, variable" "对不起，变量"
::msgcat::mcset zh_CN "is special and cannot be watched."\
                      "为特殊变量，无法查看"


# db_bptsmanagement.tcl
::msgcat::mcset zh_CN "No breakpoint here!" "此处无断点 !"


# db_configureexec.tcl
::msgcat::mcset zh_CN "Configure execution" "配置执行状态"
::msgcat::mcset zh_CN "Function name:" "函数名称："
::msgcat::mcset zh_CN "&Obtain" "获取(&O)"
::msgcat::mcset zh_CN "Input arguments:" "输入参数 :"
::msgcat::mcset zh_CN "The selected function has not been found in the\
                       opened files, maybe it was deleted since last\
                       configuration of the debugger.\nUse button \""\
                      "所选的函数不在打开的文件内，也许在上次配置调试器之后删除了此函数，\
                       \n使用按键 \""
::msgcat::mcset zh_CN "\" to refresh the list of available functions."\
                      "\" 刷新可用的函数清单。"
::msgcat::mcset zh_CN "Function name or input arguments do not match\
                       definition of the function"\
                      "函数名称或输入参数与函数定义不符"
::msgcat::mcset zh_CN "in the file!\n\nCheck function name and arguments\
                       (names, order) in the configuration dialog.\
                       \nArguments order can be changed using drag and drop\
                       with right mouse button in the arguments listbox."\
                      "文件内的内容不符!\n\n\
                      请检查配置对话框内的函数名称和参数顺序。\n\
                      可以通过在参数列表中按下鼠标右键进行拖拽来改变参数顺序。"
::msgcat::mcset zh_CN "Error in selected function name or arguments"\
                      "选中的函数名称或参数中有错"
::msgcat::mcset zh_CN "At least one input argument does not match definition\
                       of the function"\
                      "至少一个输入参数与函数定义不匹配"
::msgcat::mcset zh_CN "in the file!\n\nCheck function arguments (their\
                       names) in the configuration dialog."\
                      "文件内的内容不符！\n\n\
                       请检查配置对话框中的函数参数名称。"
::msgcat::mcset zh_CN "Error in selected function arguments"\
                      "选中的函数参数中有错误"
::msgcat::mcset zh_CN "Main level code found" "已找到顶层代码"
::msgcat::mcset zh_CN "Scipad ancillary file" "Scipad 辅助文件"
::msgcat::mcset zh_CN "The current file contains only Scipad ancillaries\
                       that cannot be debugged in Scipad."\
                      "此文件仅包含 Scipad 辅助，无法在 Scipad\
                       内进行调试，请选其它缓冲区。"
::msgcat::mcset zh_CN "This file contains main level code.\nDebug as a\
                       single .sce file?"\
                      "这个文件内包含顶层代码。\n\
                       作为一个单一 .sce 文件进行调试？"
::msgcat::mcset zh_CN "No function found" "找不到函数"
::msgcat::mcset zh_CN "This file contains no function and will be treated"\
                      " 此文件中没有函数，因此将被视为"
::msgcat::mcset zh_CN "as a single .sce file.\nStart debugging?"\
                      "作为一个单独的 .sce 文件。\n开始调试？"
::msgcat::mcset zh_CN "You must enable colorization to be able to launch the\
                       debugger!"\
                      "你必须将代码高亮功能开启以启动调试器！"
::msgcat::mcset zh_CN "The debugger needs colorization"\
                      "调试器需要开启代码高亮功能"
::msgcat::mcset zh_CN "Warning!\n\n  A function missing the closing\
                       \"endfunction\" has been found in the currently\
                       opened files.\n  Such syntax is now obsolete.\n\n \
                       Please terminate function "\
                      " 警告！\n\n \
                       目前开启的文件中，发现一个不以\
                       \"endfunction\" 命令结束的函数.\n \
                       这类语法目前版本已不允许。\n\n \
                       请以正确命令结束函数。"
::msgcat::mcset zh_CN " and try again." " 并重新尝试."
::msgcat::mcset zh_CN "Note: This message might also be triggered by a\
                       string\n  quoted with single quotes and containing\
                       the word \"function\",\n  when \"Colorize\
                       \'strings\'\" is unchecked in the Options menu."\
                      "注意：如果菜单中 \"高亮字符串(&s)\" 未被选中，\n \
                       使用单引号包含的单词\"function\"也可触发此消息。"
::msgcat::mcset zh_CN "This known bug can be worked around simply by\
                       checking \"Colorize \'&strings\'\"."\
                      "这个 bug 可以通过选中 \"高亮字符串(&s)\" 来避免。"
::msgcat::mcset zh_CN "Unterminated function definition found"\
                      "未结束的函数定义"
::msgcat::mcset zh_CN "Warning!\n\n  One or more function is defined more\
                       than once in the currently opened files.\n  The\
                       debugger cannot cope with multiple copies of\
                       functions.\n  The following duplicates were\
                       detected:\n"\
                      "警告！\n\n \
                      目前开启的文件中，有函数被重复定义.\n \
                      调试器无法处理函数重复定义。检测到\n \
                      如下重复定义：\n"
::msgcat::mcset zh_CN "\n--> Definition of function " "\n--> 函数定义 "
::msgcat::mcset zh_CN " found in:\n" "位置：\n"
::msgcat::mcset zh_CN "Duplicate function definitions found"\
                      "发现重复的函数定义"



# db_debugsession.tcl
::msgcat::mcset zh_CN "Too many breakpointed functions"\
                      "设定断点的函数数目太多"
::msgcat::mcset zh_CN "You have currently" "目前共有"
::msgcat::mcset zh_CN "functions in your opened files."\
                      "个函数在打开的文件中。"
::msgcat::mcset zh_CN "Scilab supports a maximum of" "Scilab 最多支持"
::msgcat::mcset zh_CN "possible breakpointed functions (see help setbpt)."\
                      "个被设置了断点的函数（请查看 setbpt 的帮助）。"
::msgcat::mcset zh_CN "Step-by-step hence cannot be performed."\
                      "因此无法进行逐步查错。"
::msgcat::mcset zh_CN "This command will actually run to the next breakpoint."\
                      "此命令实际将会进行到下一个断点。"
::msgcat::mcset zh_CN "Break hence cannot be performed." "所以无法暂停。"
::msgcat::mcset zh_CN "Too many breakpoints" "太多断点"
::msgcat::mcset zh_CN "Executing this command would require to set"\
                      "执行此命令必须在开启的文件内设定"
::msgcat::mcset zh_CN "breakpoints in your opened files." "个断点。"
::msgcat::mcset zh_CN "possible breakpoints in Scilab (see help setbpt)."\
                      "Scilab 中可能的断点 (参考 setbpt 命令)."
::msgcat::mcset zh_CN "Cursor must be in a function" "光标必须在某个函数内"
::msgcat::mcset zh_CN "This command cannot succeed if the function where the\
                       debug just stopped is not opened in Scipad!"\
                      "如果定义函数的文件未在 Scipad\
                       中打开，此命令便无法成功执行 !"
::msgcat::mcset zh_CN "Cannot run to return point"\
                      "无法执行到函数返回点"
::msgcat::mcset zh_CN "Warning!\nThe line where the debugger just stopped\
                       contains a return statement (either endfunction,\
                       return or resume) but this statement is not the first\
                       one on the line."\
                      "警告!\n调试器停止的位置是一函数的返回点\
                       （如命令 endfunction, return 或\
                       resume),但却不是此行第一个命令。"
::msgcat::mcset zh_CN "As a consequence the debugger cannot predict if the\
                       next debug step will really jump outside of the\
                       current function, which is what you just asked the\
                       debugger to do."\
                      "因此，调试器无法预测是否一个调试步该跳出目前函数，而这是你所要求的."
::msgcat::mcset zh_CN "To avoid this unresolvable case, it is recommended to\
                       place the return statement alone on its line."\
                      "为避免这种无法判断状况，建议让返回命令独占一行"
::msgcat::mcset zh_CN "Do you want to see this warning again during this\
                       Scipad session?"\
                      "你希望在 Scipad 中继续看到此警告信息吗?"
::msgcat::mcset zh_CN "Unresolvable case found while running to return point"\
                      "执行到返回点时无法判断"
::msgcat::mcset zh_CN "No effect - The debugged file is not stuck"\
                      "无影响 - 文件将继续进行调试"


# db_init.tcl
::msgcat::mcset zh_CN "Please wait..." "稍待片刻..."


# db_states.tcl
::msgcat::mcset zh_CN "Currently no debug session" "目前无调试任务"
::msgcat::mcset zh_CN "Ready to start debug" "准备进行调试"
::msgcat::mcset zh_CN "Debug in progress" "调试中"
::msgcat::mcset zh_CN "Scilab is back at the main level now."\
                      "Scilab 现已返回顶层。"



# db_watch.tcl
::msgcat::mcset zh_CN "Watch" "查看"
::msgcat::mcset zh_CN "Watch variables:" "查看变量："
::msgcat::mcset zh_CN "A&uto (local variables)" "自动(&u 局部变量)"
::msgcat::mcset zh_CN "Au&to watch globals too"\
                      "自动&u 同时查看全局变量"
::msgcat::mcset zh_CN "Generic expressions:" "通用表达式："
::msgcat::mcset zh_CN "Call stack:" "调用栈："
::msgcat::mcset zh_CN "Always on to&p" "永远在最上层(&p)"
::msgcat::mcset zh_CN "Hide watch &variables" "隐藏查看变量(&v)"
::msgcat::mcset zh_CN "Hide e&xpressions" "隐藏表达式(&x)"
::msgcat::mcset zh_CN "Hide call &stack" "隐藏调用栈(&s)"
::msgcat::mcset zh_CN "&Dock" "减少(&D)"
::msgcat::mcset zh_CN "&Add" "新增(&A)"
::msgcat::mcset zh_CN "&Filter errors" "过滤错误(&F)"
::msgcat::mcset zh_CN "Breakpoints" "断点"
::msgcat::mcset zh_CN "Breakpointed functions" "设定断点的函数"
::msgcat::mcset zh_CN "Scilab busy indicator" "Scilab 忙碌装态指示"
::msgcat::mcset zh_CN "Debugger state indicator" "调试器装态指示"
::msgcat::mcset zh_CN "Undefined elements are not legal in variable"\
                      " 变量中出现未定义元素"
::msgcat::mcset zh_CN ".\nThis variable will not be updated in Scilab."\
                      ".\n此变量无法在 Scilab 内更新"
::msgcat::mcset zh_CN "Illegal undefined element found"\
                      "不合法的未定义元素"
::msgcat::mcset zh_CN "evaluates to:" "计算值:"
::msgcat::mcset zh_CN "produces error" "产生错误"
::msgcat::mcset zh_CN "Scipad cannot find source code for the selected\
                       function.\nPlease open the adequate file manually and\
                       try again."\
                      "Scipad\
                      无法找到所选函数的源代码.\n请手动开启相应的文件并重试。"
::msgcat::mcset zh_CN "Missing source code" "找不到源代码"


# filecommands.tcl
::msgcat::mcset zh_CN "New script" "新脚本"
::msgcat::mcset zh_CN "New view on the same file" "同一文件的另一视图"
::msgcat::mcset zh_CN "The contents of" "文件"
::msgcat::mcset zh_CN "may have changed, do you wish to save your changes?"\
                      "内容可能已修改，是否需要保存你的修改？"
::msgcat::mcset zh_CN "Save Confirm?" "确定保存？"
::msgcat::mcset zh_CN "Open source of..." "打开函数的源代码..."
::msgcat::mcset zh_CN "Open source of:" "打开函数的源代码 :"
::msgcat::mcset zh_CN "Weird drag&drop" "灵活的拖拽(&d)"
::msgcat::mcset zh_CN "is neither a valid filename nor a valid directory.\
                       Either you're dragging an object of another type, or\
                       you hit a bug of the dnd mechanism."\
                      "文件名或目录名称不合法。\
                       可能是你拖入了另一种类型的对象，或者触及了 dnd 机制的一个 bug。"
::msgcat::mcset zh_CN "File not found" "无此文件"
::msgcat::mcset zh_CN "does not exist anymore. Do you want to create an\
                       empty file with the same name?"\
                      "不再存在。\
                       你希望创建一个同名的空白文件吗 ?"
::msgcat::mcset zh_CN "Open file" "开启文件"
#::msgcat::mcset zh_CN "The file" "文件" ;# defined in the "commons" section
::msgcat::mcset zh_CN "is already opened! Save the current opened file to\
                       another name and reopen it from disk!"\
                      "已经打开！请将目前文件以另一文件名保存，并重新打开它！"
::msgcat::mcset zh_CN "File" "文件"
::msgcat::mcset zh_CN "saved" "保存"
::msgcat::mcset zh_CN "cannot be written!" "无法写入！"
::msgcat::mcset zh_CN "The contents of " "硬盘上文件"
::msgcat::mcset zh_CN "has changed on disk, save it anyway?"\
                      "的内容已被更改, 依然要保存么？"
::msgcat::mcset zh_CN "has been modified outside of Scipad. Do you want to\
                       reload it?"\
                      "已在 Scipad\
                       外被修改，你希望重新装载到 Scipad 中么？"
::msgcat::mcset zh_CN "File has changed!" "文件已被修改！"
::msgcat::mcset zh_CN "Save as" "另存为"
::msgcat::mcset zh_CN "Scilab files" "Scilab 文件"
::msgcat::mcset zh_CN "Scicos files" "Scicos 文件"
::msgcat::mcset zh_CN "XML files" "XML 文件"
::msgcat::mcset zh_CN "Wait seconds while loading and colorizing file"\
                      "载入文件并高亮显示代码中，稍待片刻"
::msgcat::mcset zh_CN "The contents of this file may have changed, do you\
                       wish to save your changes?"\
                      "文件内容已被修改，你希望保存修改吗？"
::msgcat::mcset zh_CN "Revert" "恢复"
::msgcat::mcset zh_CN "to saved?" "原状后存起？"
::msgcat::mcset zh_CN "Revert Confirm?" "确定恢复原状？"
::msgcat::mcset zh_CN "Unreadable file" "文件无法读取！"
::msgcat::mcset zh_CN "exists but is not readable!" "文件存在但不可读！"
::msgcat::mcset zh_CN "This file is rather large and colorization might take\
                       some time.\n\nColorize anyway?"\
                      "此文件比较大，若完成全部代码高亮可能很费时。\n\n，你确定继续进行代码高亮？"
::msgcat::mcset zh_CN "File size warning" "文件过大警告"
::msgcat::mcset zh_CN "Windows shortcut file" "Windows 快捷方式文件"
#::msgcat::mcset zh_CN "The file" "文件" ;# defined in the "commons" section
::msgcat::mcset zh_CN "is a Windows shortcut and will not be opened!"\
                      "为 Windows 快捷方式，无法打开！"


# findreplace.tcl
::msgcat::mcset zh_CN "Dialog box already open!"\
                      "对话框已经打开！"
::msgcat::mcset zh_CN "The string" "字符串"
::msgcat::mcset zh_CN "could not be found in" "无法找到于"
::msgcat::mcset zh_CN "could not be found in any of the opened files !"\
                      "无法在所有打开的文件中找到！"
::msgcat::mcset zh_CN "replacements done" "完成替换动作"
::msgcat::mcset zh_CN "Find" "查找"
::msgcat::mcset zh_CN "No match found in the selection for"\
                      "选择区内找不到匹配"
::msgcat::mcset zh_CN "\nWould you like to look for it in the entire text?"\
                      "\n在全文内搜寻?"
::msgcat::mcset zh_CN "You are searching for an empty string!"\
                      "你正在搜索一空字串！"
::msgcat::mcset zh_CN "Re&place" "替换(&R)"
::msgcat::mcset zh_CN "Find what:" "查找："
::msgcat::mcset zh_CN "Replace with:" "替代成："
::msgcat::mcset zh_CN "Find &Next" "查找下一个(&N)"
::msgcat::mcset zh_CN "Replace &All" "全部替换(&a)"
::msgcat::mcset zh_CN "Direction" "方向"
::msgcat::mcset zh_CN "&Upwards" "向上查找(&U)"
::msgcat::mcset zh_CN "&Downwards" "向下查找(&D)"
::msgcat::mcset zh_CN "Text types" "文字类型"
::msgcat::mcset zh_CN "Find options" "查找选项"
::msgcat::mcset zh_CN "Match &whole word only"\
                      "匹配整个单词(&w)"
::msgcat::mcset zh_CN "Match &case" "大小写敏感(&c)"
::msgcat::mcset zh_CN "&Regular expression" "正则表达式(&R)"
::msgcat::mcset zh_CN "In all &opened files" "在所有打开文件内(&o)"
::msgcat::mcset zh_CN "In &selection only" "在选择区内(&s)"
::msgcat::mcset zh_CN "In typed te&xt" "在特定类型文字内(&x)"
::msgcat::mcset zh_CN "In a director&y" "在目录内(&y)"
::msgcat::mcset zh_CN "Find in files search options" "文件搜索选项"
::msgcat::mcset zh_CN "Directory r&ecurse search" "目录内递回搜寻(&e)"
::msgcat::mcset zh_CN "Include &hidden files" "包含隐藏文件(&h)"
::msgcat::mcset zh_CN "In files/file types:" "在以下文件类型内 :"
::msgcat::mcset zh_CN "In directory:" "在目录内 :"
::msgcat::mcset zh_CN "Search for file names" "以文件名搜索"
::msgcat::mcset zh_CN "You have given an empty search string. Are you sure\
                       you want to search among file names?"\
                      "你可能输入一个空文件名. 确定继续 ?"
::msgcat::mcset zh_CN "Wrapped around" "回到文件头继续搜索"
::msgcat::mcset zh_CN "Back at the first file searched!"\
                      "回到第一个找到的文件!"
::msgcat::mcset zh_CN "No more matches" "无法再找到匹配的"
::msgcat::mcset zh_CN "The string to regexp search for cannot be compiled by\
                       Tcl.\nCheck again, maybe certain characters should be\
                       escaped."\
                      "此正则运算无法被 TCL 编译。\n\
                      请再检查ㄧ次，也许有些字符需加上转义符号。"
::msgcat::mcset zh_CN "Any single character" "任一单一字符"
::msgcat::mcset zh_CN "Zero or more" "零次以上"
::msgcat::mcset zh_CN "One or more" "一次以上"
::msgcat::mcset zh_CN "Zero or one" "零或一次"
::msgcat::mcset zh_CN "Any one character in the set" "集合内任一字符"
::msgcat::mcset zh_CN "Any one character not in the set"\
                      "任一不在集合内的字符"
::msgcat::mcset zh_CN "Or" "或"
::msgcat::mcset zh_CN "Beginning of line" "行首"
::msgcat::mcset zh_CN "End of line" "行尾"
::msgcat::mcset zh_CN "Beginning of word" "单词开头"
::msgcat::mcset zh_CN "End of word" "单词结束"
::msgcat::mcset zh_CN "A letter" "字母"
::msgcat::mcset zh_CN "A decimal digit" "数字"
::msgcat::mcset zh_CN "An alphanumeric (letter or digit)" "字母或数字"
::msgcat::mcset zh_CN "A space or tab character"\
                      "空格或制表符"
::msgcat::mcset zh_CN "A rational number" "有理数"
::msgcat::mcset zh_CN "A floating point number" "实数"
::msgcat::mcset zh_CN "A Scilab comment" "Scilab 命令"
::msgcat::mcset zh_CN "A Scilab name" "Scilab 变量名"
::msgcat::mcset zh_CN "A Scilab continued line (with dots)"\
                      "Scilab 连续行 (行后为点)"


# findinfiles.tcl
::msgcat::mcset zh_CN "Directory access issue" "目录读取问题"
::msgcat::mcset zh_CN "The directory you specified cannot be reached. The\
                       search won't bring any result."\
                      "所指定的目录无法读取。搜寻无结果。"
::msgcat::mcset zh_CN "Some directories or files could not be open and were\
                       ignored during search.\n You might miss read access\
                       to them.\n\n The following ones were ignored"\
                      "部分目录或文件无法开启，因此搜索过程中忽略了它们。\n\
                       或许是读取权限的缺失造成。\n\n\
                       以下为忽略的文件和目录"
::msgcat::mcset zh_CN "Ignored files" "被忽略的文件"
::msgcat::mcset zh_CN "Searching in file:" "文件内搜索："
::msgcat::mcset zh_CN "End of file search:" "文件搜索结束："
::msgcat::mcset zh_CN "matches found" "符合项"
::msgcat::mcset zh_CN "searched files" "搜索过的文件"
::msgcat::mcset zh_CN "Constructing file list:" "建立文件清单："
::msgcat::mcset zh_CN "&Previous" "向前(&P)"
::msgcat::mcset zh_CN "&Next" "向后(&N)"
::msgcat::mcset zh_CN "Scipad search results" "Scipad 搜索结果"


# fonts.tcl
::msgcat::mcset zh_CN "Choose font" "选择字体"
::msgcat::mcset zh_CN "Family" "字体族"
::msgcat::mcset zh_CN "Style" "风格(Style)"
::msgcat::mcset zh_CN "&Bold" "粗体(&B)"
::msgcat::mcset zh_CN "&Italic" "斜体(&I)"
::msgcat::mcset zh_CN "&Underline" "下划线(&U)"
::msgcat::mcset zh_CN "Size" "大小"
::msgcat::mcset zh_CN "&Defaults" "缺省(&D)"
::msgcat::mcset zh_CN "Use as..." "用作..."
::msgcat::mcset zh_CN "&Text font" "文本字体(&T)"
::msgcat::mcset zh_CN "&Menu font" "菜单字体(&M)"
::msgcat::mcset zh_CN "Sample" "例子"
::msgcat::mcset zh_CN "Font size" "字体大小"
::msgcat::mcset zh_CN "Revert fonts" "恢复成缺省字体"
::msgcat::mcset zh_CN "Are you sure you want to revert all fonts to defaults?"\
                      "你确定要恢复成缺省字体？"


# FormatStringsForWatch.sci
::msgcat::mcset zh_CN " what you try to watch is not supported by the\
                       debugger"\
                      " 你所要查看的内容还未为 Scipad\
                       调试器支持"
::msgcat::mcset zh_CN " what you try to watch is of type "\
                      " 你所查看的类型为 "
::msgcat::mcset zh_CN " - this type is not supported by the debugger"\
                      " - 此类型还未为 Scipad 调试器支持"
::msgcat::mcset zh_CN "Type:" "类型 :"
::msgcat::mcset zh_CN "(unsupported)" "(未支持)"
::msgcat::mcset zh_CN "size:" "大小 :"
::msgcat::mcset zh_CN "real" "实数"
::msgcat::mcset zh_CN "complex" "复数"
::msgcat::mcset zh_CN "elements" "元素"
::msgcat::mcset zh_CN "graphic handle" "图形句柄"
::msgcat::mcset zh_CN "uncompiled function" "未编译的函数"
::msgcat::mcset zh_CN "compiled function" "已编译的函数"
::msgcat::mcset zh_CN "library" "库"
::msgcat::mcset zh_CN "pointer" "指针"
::msgcat::mcset zh_CN "size implicit index" "大小被隐含的指标"
::msgcat::mcset zh_CN "primitive" "基础"


# FormatWhereForWatch.sci
::msgcat::mcset zh_CN "Breakpoint" "断点"
::msgcat::mcset zh_CN "called at line" "在第"
::msgcat::mcset zh_CN "of exec file" "行被执行程序调用"
::msgcat::mcset zh_CN "of execstr instruction" "行被 execstr 的命令调用"
::msgcat::mcset zh_CN "called under pause" "在 pause 状态下被调用"
::msgcat::mcset zh_CN "of macro" "被巨集调用"


# infomessages.tcl
::msgcat::mcset zh_CN "Line:" "行:"
::msgcat::mcset zh_CN "Column:" "栏:"
::msgcat::mcset zh_CN "Line" "第"
::msgcat::mcset zh_CN "in" " 行在 "
::msgcat::mcset zh_CN "Logical line:" "逻辑行:"
::msgcat::mcset zh_CN " \[ReadOnly\]" " \[只读\]"
::msgcat::mcset zh_CN " (modified)" " (已修改)"


# inputtext.tcl
::msgcat::mcset zh_CN "No corresponding <" "没找到 <"
::msgcat::mcset zh_CN "> found!" "> ！"
::msgcat::mcset zh_CN "Code editing is not allowed during debug!"\
                      "调试过程中，无法编辑程序！"


# menues.tcl
::msgcat::mcset zh_CN "&File" "文件(&F)"
::msgcat::mcset zh_CN "&Edit" "编辑(&E)"
::msgcat::mcset zh_CN "&Search" "查找(&R)"
::msgcat::mcset zh_CN "E&xecute" "执行(&x)"
::msgcat::mcset zh_CN "&Debug" "调试(&D)"
::msgcat::mcset zh_CN "S&cheme" "代码(&c)"
::msgcat::mcset zh_CN "&Options" "选项(&O)"
::msgcat::mcset zh_CN "&Windows" "窗口(&W)"
::msgcat::mcset zh_CN "&Help" "帮助(&H)"

# menues.tcl - File menu
::msgcat::mcset zh_CN "&New" "新建(&N)"
::msgcat::mcset zh_CN "&Open..." "打开文件(&O)..."
::msgcat::mcset zh_CN "Open &in new" "在新窗口中打开(&u)"
::msgcat::mcset zh_CN "&horizontal tile" "水平层叠(&h)"
::msgcat::mcset zh_CN "&vertical tile" "垂直层叠(&v)"
::msgcat::mcset zh_CN "Op&en source of..." "打开函数源代码...(&e)"
::msgcat::mcset zh_CN "&Save" "保存(&S)"
::msgcat::mcset zh_CN "Save &as..." "另存为(&a)..."
::msgcat::mcset zh_CN "Save a&ll" "保存所有文件(&l)"
::msgcat::mcset zh_CN "&Revert..." "恢复(&v)..."
::msgcat::mcset zh_CN "Import &Matlab file..." "载入 &Matlab 文件..."
::msgcat::mcset zh_CN "Create help s&keleton..." "产生帮助文件架构(&k)..."
::msgcat::mcset zh_CN "Compile as &help page" "编译帮助文件(&h)"
::msgcat::mcset zh_CN "Open &function source" "打开函数源代码(&f)"
::msgcat::mcset zh_CN "Print Se&tup" "打印机设置(&t)"
::msgcat::mcset zh_CN "&Print" "打印(&P)"
::msgcat::mcset zh_CN "&Close file" "关闭文件(&C)"
::msgcat::mcset zh_CN "E&xit" "退出(&x)"

# menues.tcl - Edit menu
::msgcat::mcset zh_CN "&Undo" "撤销(&U)"
::msgcat::mcset zh_CN "&Redo" "重做(&R)"
::msgcat::mcset zh_CN "Cu&t" "剪切(&t)"
::msgcat::mcset zh_CN "B&lock cut" "剪切文字块(&l)"
::msgcat::mcset zh_CN "&Copy" "复制(&C)"
::msgcat::mcset zh_CN "&Paste" "粘贴(&P)"
::msgcat::mcset zh_CN "&Block paste" "粘贴文字块(&B)"
::msgcat::mcset zh_CN "&Delete" "删除(&D)"
::msgcat::mcset zh_CN "Select &All" "全选(&A)"
::msgcat::mcset zh_CN "Insert &Time/Date" "插入时间/日期(&T)"
::msgcat::mcset zh_CN "Co&mment selection" "注释选择区(&m)"
::msgcat::mcset zh_CN "U&ncomment selection"\
                      "清除选择区注释(&n)"
::msgcat::mcset zh_CN "&Indent selection" "缩进(&I)"
::msgcat::mcset zh_CN "Unin&dent selection" "清除缩进(&d)"

# menues.tcl - Search menu
::msgcat::mcset zh_CN "&Find..." "查找(&F)..."
::msgcat::mcset zh_CN "Find &Next" "查找下一个(&N)"
::msgcat::mcset zh_CN "&Replace..." "替换(&R)..."
::msgcat::mcset zh_CN "&Goto Line..." "跳至行(&G)..."

# menues.tcl - Exec menu
::msgcat::mcset zh_CN "&Load into Scilab" "载入 Scilab 中(&L)"
::msgcat::mcset zh_CN "&Evaluate selection" "执行选择部分(&E)"

# menues.tcl - Debug menu
::msgcat::mcset zh_CN "&Insert/Remove breakpoint" "插入/删除断点(&I)"
::msgcat::mcset zh_CN "Remove &all breakpoints" "清除所有断点(&a)"
::msgcat::mcset zh_CN "&Configure execution..." "配置执行(&C)..."
::msgcat::mcset zh_CN "Go to next b&reakpoint" "跳至下一断点(&r)"
::msgcat::mcset zh_CN "&Step by step" "下一步(&S)"
::msgcat::mcset zh_CN "Step &into" "进入(&i)"
::msgcat::mcset zh_CN "Step o&ver" "跳过(&v)"
::msgcat::mcset zh_CN "Step &out" "跳出(&o)"
::msgcat::mcset zh_CN "Run to re&turn point"\
                      "执行至返回点"
::msgcat::mcset zh_CN "Run to c&ursor" "执行至光标处(&u)"
::msgcat::mcset zh_CN "G&o on ignoring any breakpoint"\
                      "执行并忽略所有断点"
::msgcat::mcset zh_CN "Show &watch" "显示查看变量(&w)"
::msgcat::mcset zh_CN "&Break" "终止(&B)"
::msgcat::mcset zh_CN "Cance&l debug" "取消调试(&l)"

#    ::msgcat::mcset zh_CN "language scheme" "程序语言"
# menues.tcl - Scheme menu
::msgcat::mcset zh_CN "S&cilab" "S&cilab"
::msgcat::mcset zh_CN "&XML" "&XML"
::msgcat::mcset zh_CN "&none" "无(&n)"
::msgcat::mcset zh_CN "&Colorize" "代码高亮(&C)"

# menues.tcl - Options menu
::msgcat::mcset zh_CN "&Fonts..." "字体(&F)..."
::msgcat::mcset zh_CN "&Colors" "颜色(&C)"
::msgcat::mcset zh_CN "Colori&ze" "代码高亮(&z)"
::msgcat::mcset zh_CN "&Always" "一直"
::msgcat::mcset zh_CN "Ask for &big files" "文件过大时询问"
::msgcat::mcset zh_CN "&Never" "从不"
::msgcat::mcset zh_CN "Colorize \'&strings\'" "字符串高亮"
::msgcat::mcset zh_CN "Line n&umbers" "行号(&u)"
::msgcat::mcset zh_CN "&Hide" "隐藏(H)"
::msgcat::mcset zh_CN "&Left aligned" "左边对齐(L)"
::msgcat::mcset zh_CN "&Right aligned" "右边对齐(R)"
::msgcat::mcset zh_CN "Show c&ontinued lines" "显示连续行(&o)"
::msgcat::mcset zh_CN "Word &wrap" "自动换行(&W)"
::msgcat::mcset zh_CN "&Double-click behavior" "鼠标双击行为(&D)"
::msgcat::mcset zh_CN "&Linux" "&Linux"
::msgcat::mcset zh_CN "Windo&ws" "Windo&ws"
::msgcat::mcset zh_CN "&Scilab" "&Scilab"
::msgcat::mcset zh_CN "&Tabs and indentation" "制表位及缩进(&T)"
::msgcat::mcset zh_CN "Tab inserts &spaces" "插入空格代替制表符(&s)"
::msgcat::mcset zh_CN "&Indentation spaces" "缩进宽度(&I)"
::msgcat::mcset zh_CN "&Tab size (characters)" "制表符宽度(&T)"
::msgcat::mcset zh_CN "Use &keyword indentation" "使用关键字缩进(&k)"
::msgcat::mcset zh_CN "Com&pletion" "退出快捷键(&p)"
::msgcat::mcset zh_CN "&Tab" "&Tab"
::msgcat::mcset zh_CN "&Control-Tab" "&Control-Tab"
::msgcat::mcset zh_CN "&Alt-Tab" "&Alt-Tab"
::msgcat::mcset zh_CN "&Shift-Tab" "&Shift-Tab"
::msgcat::mcset zh_CN "C&ontrol-Alt-Tab" "C&ontrol-Alt-Tab"
::msgcat::mcset zh_CN "S&hift-Control-Tab" "S&hift-Control-Tab"
::msgcat::mcset zh_CN "Sh&ift-Alt-Tab" "Sh&ift-Alt-Tab"
::msgcat::mcset zh_CN "Shi&ft-Control-Alt-Tab" "Shi&ft-Control-Alt-Tab"
::msgcat::mcset zh_CN "File&names" "文件名(&n)"
::msgcat::mcset zh_CN "&Full path" "全名(&F)"
::msgcat::mcset zh_CN "Full path if &ambiguous"\
                      "混淆时使用全名(&a)"
::msgcat::mcset zh_CN "&Unambiguous pruned path" "短名(&U)"
::msgcat::mcset zh_CN "Windows menu &sorting" "窗口排序(&s)"
::msgcat::mcset zh_CN "&File opening order" "按文件开启顺序(&F)"
::msgcat::mcset zh_CN "&Alphabetically" "按字母顺序(&A)"
::msgcat::mcset zh_CN "&Most recently used" "按使用顺序(&M)"
::msgcat::mcset zh_CN "&Recent files" "最近文件数目(&R)"
::msgcat::mcset zh_CN "&Backup files depth" "备份文件深度(&B)"
::msgcat::mcset zh_CN "&Locale" "语言"
::msgcat::mcset zh_CN "&Bindings style"  "编辑器风格"
::msgcat::mcset zh_CN "Execution &errors" "执行错误(&e)"
::msgcat::mcset zh_CN "In Scilab &shell only" "只出现在Scilab &shell "
::msgcat::mcset zh_CN "Copied in a &message box" "复制到消息框(&m)"

# menues.tcl - Windows menu
::msgcat::mcset zh_CN "&Maximize" "窗口最大化(&M)"
::msgcat::mcset zh_CN "&Split" "上下分割(&S)"
::msgcat::mcset zh_CN "S&plit (side by side)" "左右分割(&p)"
::msgcat::mcset zh_CN "Sp&lit file" "上下分割文件(&l)"
::msgcat::mcset zh_CN "Spl&it file (side by side)" "左右分割文件(&i)"
::msgcat::mcset zh_CN "Tile all &vertically" "垂直层叠(&v)"
::msgcat::mcset zh_CN "Tile all &horizontally" "水平层叠(&h)"
::msgcat::mcset zh_CN "Space sashes &evenly" "窗口均分(&e)"
::msgcat::mcset zh_CN "Opened Files" "已打开的文件"

# menues.tcl - Help menu
::msgcat::mcset zh_CN "&Help..." "帮助..."
::msgcat::mcset zh_CN "&What's?..." "这是..?(&W)(查询选中函数的帮助)"
::msgcat::mcset zh_CN "&About" "关于(&A)"
::msgcat::mcset zh_CN "Adding &translations..." "翻译 Scipad...(&t)"
::msgcat::mcset zh_CN "Adding Scipad Translations..." "协助翻译 Scipad"
::msgcat::mcset zh_CN "&Bugs \& wishlist" "Bugs 和计划工作(&B)"
::msgcat::mcset zh_CN "Scipad Known bugs" "Scipad 已知 Bugs "
::msgcat::mcset zh_CN "Change&log" "Change&log"
::msgcat::mcset zh_CN "Changes in the Scipad codebase"\
                      "Scipad 代码更新列表"


# popupmenus.tcl
::msgcat::mcset zh_CN "Open the source of" "开启函数的源代码"
::msgcat::mcset zh_CN "AddWatch" "新增观察值"
::msgcat::mcset zh_CN "AddGenExp" "新增通用表达式"
::msgcat::mcset zh_CN "Jump to" "跳到"


# print.tcl
::msgcat::mcset zh_CN "Print Setup" "打印机设置"
::msgcat::mcset zh_CN "Print Command: " "打印命令："


# scilabexec.tcl
::msgcat::mcset zh_CN "No point in loading an empty file!"\
                      "无法载入一个空文件!"
::msgcat::mcset zh_CN "Silent file save failed" "无名文件无法保存"
::msgcat::mcset zh_CN "File could not be saved in a temporary\
                       location.\nOverwrite original file?"\
                      "临时目录内无法保存文件。\n覆盖原始文件？"
::msgcat::mcset zh_CN "Exec done" "执行结束"
::msgcat::mcset zh_CN "Matlab files" "Matlab 文件"
::msgcat::mcset zh_CN "Matlab file to convert" "转换 Matlab 文件"
::msgcat::mcset zh_CN "Scilab is converting, please hold on..."\
                      "Scilab 转换中，稍待..."
::msgcat::mcset zh_CN "Matlab file import" "导入 Matlab 文件"
::msgcat::mcset zh_CN "Conversion of the file failed, see the Scilab window\
                       for details -- Perhaps report the error text and the\
                       offending Matlab file to"\
                      "文件转换失败，详情请参考 Scilab\
                       窗口内容 -- 也许可到\
                       http://bugzilla.scilab.org\
                       报告错误信息及对应的 Matlab 文件。"
::msgcat::mcset zh_CN "Please select destination path for the xml source of\
                       the help file"\
                      "请选择 xml 帮助文件的路径"
::msgcat::mcset zh_CN "Older version of" "找到文件"
::msgcat::mcset zh_CN "found!" "的旧版本！"
::msgcat::mcset zh_CN "An old version of" "文件"
::msgcat::mcset zh_CN "already exists: open the old file instead?"\
                      "的旧版本已经存在: 开启旧版本？"
::msgcat::mcset zh_CN "Existing file" "已存在的文件"
::msgcat::mcset zh_CN "New skeleton" "新建框架"
::msgcat::mcset zh_CN "ScilabEval command cannot be passed to Scilab!"\
                      "ScilabEval 命令无法传送至 Scilab！"
::msgcat::mcset zh_CN "impossibleScilabEval_message"\
                      "警告: 命令无法传送至 Scilab,\
                       因此忽略.\n\n  最有可能是你没有 Scilab\
                       的 TMPDIR 目录的写入权限. \
                       可能产生不可预期的结果.\n\n \
                       出问题的命令为:\n\n-->"
::msgcat::mcset zh_CN "impossibleScilabEval_message2"\
                      "警告: 命令无法传送至 Scilab, 因此忽略， \
                       原因是命令过长.\n\n \
                       可能产生不可预期的结果.\n\n \
                      如果正在调试，请试着删除查看的变量.\n\n \
                       出问题的命令为:\n\n-->"
::msgcat::mcset zh_CN "(end of command skipped)" "(后面命令跳过)"
::msgcat::mcset zh_CN "Scilab working" "Scilab 运行中"
::msgcat::mcset zh_CN "Scilab is working, please wait for the prompt to\
                       execute this command!"\
                      "Scilab 运行中，请等候此命令的输出结果 !"
::msgcat::mcset zh_CN "Scilab is working, wait for the prompt to load file"\
                      "Scilab 运行中，请等候载入文件的结果"
::msgcat::mcset zh_CN "Scilab is working, wait for the prompt to execute the\
                       selection."\
                      "Scilab 运行中，请等候选择部分的执行结果。"
::msgcat::mcset zh_CN "Scilab is working, wait for the prompt to convert a\
                       Matlab file."\
                      "Scilab 运行中，请等候转换 Matlab 文件的结果."
::msgcat::mcset zh_CN "Scilab is working, wait for the prompt to compile the\
                       help file."\
                      "Scilab 运行中，请等候编译帮助文件的结果."
::msgcat::mcset zh_CN "Scilab is working, wait for the prompt to issue a\
                       debugger command."\
                      "Scilab 运行中，请等候调试命令的结果."
::msgcat::mcset zh_CN "Scilab execution error" "Scilab  执行错误"
::msgcat::mcset zh_CN "The shell reported an error while trying to execute "\
                      "执行  "
::msgcat::mcset zh_CN ": error " "时，Scilab Shell 发现错误: 错误 "
::msgcat::mcset zh_CN "at line " "第 "
::msgcat::mcset zh_CN " of " " 行，位于 "
::msgcat::mcset zh_CN "Execution aborted!" "执行被终止!"


# scipad.tcl
::msgcat::mcset zh_CN "NOT CONNECTED TO SCILAB" "未与 Scilab 连接"

# whichfun.tcl
::msgcat::mcset zh_CN "Being at line" "第"
::msgcat::mcset zh_CN ", function" "行, 函数"
::msgcat::mcset zh_CN "begins at" "起始位置为"
::msgcat::mcset zh_CN ", and there are" ", 共有"
::msgcat::mcset zh_CN "continued lines, i.e. we are at line"\
                      "行连续行，也就是目前为第"
::msgcat::mcset zh_CN "of" "行，此行位于"
::msgcat::mcset zh_CN "Function" "函数"
::msgcat::mcset zh_CN "line" "行"
::msgcat::mcset zh_CN "The cursor is not currently inside a function body."\
                      "光标目前不在函数内部。"
::msgcat::mcset zh_CN "Which function?" "哪一个函数？"



#preference variables description
::msgcat::mcset zh_CN "BGCOLOR" "编辑区背景色"
::msgcat::mcset zh_CN "BGLNMARGCOLOR" "行号背景色"
::msgcat::mcset zh_CN "FGCOLOR" "文字"
::msgcat::mcset zh_CN "FGLNMARGCOLOR" "行号"
::msgcat::mcset zh_CN "CURCOLOR" "光标"
::msgcat::mcset zh_CN "PARCOLOR" "圆括号 ()"
::msgcat::mcset zh_CN "BRAKCOLOR" "中括符 \\\[\\\]"
::msgcat::mcset zh_CN "BRACCOLOR" "大括符  {}"
::msgcat::mcset zh_CN "PUNCOLOR" "标点符号"
::msgcat::mcset zh_CN "INTFCOLOR" "内部函数"
::msgcat::mcset zh_CN "COMMCOLOR" "命令"
::msgcat::mcset zh_CN "SCICCOLOR" "Scicos 函数"
::msgcat::mcset zh_CN "OPCOLOR" "运算子"
::msgcat::mcset zh_CN "QTXTCOLOR" "字符文字"
::msgcat::mcset zh_CN "REMCOLOR" "注解"
::msgcat::mcset zh_CN "XMLCOLOR" "XML 标签"
::msgcat::mcset zh_CN "NUMCOLOR" "数字"
::msgcat::mcset zh_CN "SELCOLOR" "选择区"
::msgcat::mcset zh_CN "BREAKPOINTCOLOR" "断点背景"
::msgcat::mcset zh_CN "LFUNCOLOR" "程序库函数"
::msgcat::mcset zh_CN "PDEFCOLOR" "系统变量"
::msgcat::mcset zh_CN "FOUNDTEXTCOLOR" "符合条件的文字"
::msgcat::mcset zh_CN "CONTLINECOLOR" "连续行"
::msgcat::mcset zh_CN "USERFUNCOLOR" "使用者定义的的函数"



#overriding tcl-core dialogs (many are actually already found in
#  TKDIR/msgs/fr.msg)
::msgcat::mcset zh_CN "&Directory:" "目录(&D) :"
::msgcat::mcset zh_CN "File &name:" "文件名 (&n):"
::msgcat::mcset zh_CN "Files of &type:" "文件类型(&t) :"
#::msgcat::mcset zh_CN "&Save" "保存(&S)" # already included
::msgcat::mcset zh_CN "&Open" "打开(&O)"
::msgcat::mcset zh_CN "Open" "打开"
::msgcat::mcset zh_CN "Save As" "另存为"
::msgcat::mcset zh_CN "&Cancel" "取消(&C)"
::msgcat::mcset zh_CN "&Selection:" "选择(&S) :"
::msgcat::mcset zh_CN "&Yes" "是(&Y)"
::msgcat::mcset zh_CN "&No" "否(&N)"
::msgcat::mcset zh_CN "&Red" "红(&R)"
::msgcat::mcset zh_CN "&Green" "绿(&G)"
::msgcat::mcset zh_CN "&Blue" "蓝(&B)"
::msgcat::mcset zh_CN "Directory \"%1\$s\" does not exist."\
                      "目录 \"%1\$s\"不存在."

# helps.tcl
::msgcat::mcset zh_CN "About" "关于"
::msgcat::mcset zh_CN "aboutme_message"\
                      "\n Scipad 最初由 Joseph Acosta, joeja@mindspring.com 开发, \n\
                      并由 Scilab Consortium 修改, Enrico Segre 2003-2006 进一步改写.\n\n\
                       其他贡献者,\n 调试工具: François\
                       Vogel 2004-2008. \n 各国语言翻译:\n 瑞典文 \
                       Gunnar Lindholm，2005。\n 挪威文 Torbjørn Pettersen\
                       2005.\n 德文   Dirk Reusch 2005.\n 波兰文\
                       Bartosz Kosiorek 2005.\n 西班牙文 Juan Nicolás\
                       Quesada 2006.\n 中文 于仕琪 2008, 李运璋 2007. \n 丹麦文\
                       Kim B. Hansen 2007."
::msgcat::mcset zh_CN aboutme_license "SciPad is released under GPL"
