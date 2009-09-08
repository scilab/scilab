#     Scipad - programmer's editor and debugger for Scilab
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

# Scipad texts in Japanese by Hiroshi Saito(z-saito@guitar.ocn.ne.jp)
# in sync at least with v6.155

# commons
::msgcat::mcset ja_jp "Cancel" "キャンセル"
::msgcat::mcset ja_jp "Cance&l" "キャンセル(&l)"
::msgcat::mcset ja_jp "OK" "OK"
::msgcat::mcset ja_jp "All files" "全てのファイル"
::msgcat::mcset ja_jp "Add/Change" "追加/訂正"
::msgcat::mcset ja_jp "Add/Chan&ge" "追加/訂正(&g)"
::msgcat::mcset ja_jp "&Remove" "削除(&R)"
::msgcat::mcset ja_jp "Rem&ove" "削除(&o)"
::msgcat::mcset ja_jp "Untitled" "無題"
::msgcat::mcset ja_jp "Close" "閉じる"
::msgcat::mcset ja_jp "&Close" "閉じる(&C)"
::msgcat::mcset ja_jp "Error " "エラー "
::msgcat::mcset ja_jp "The file" "ファイル"
::msgcat::mcset ja_jp "Function" "関数"
::msgcat::mcset ja_jp "Line" "行"

# buffernavigation.tcl
::msgcat::mcset ja_jp "Hide" "隠す"
::msgcat::mcset ja_jp "Close this tile, keep content"\
                      "このタイルを閉じる, 内容を維持"
::msgcat::mcset ja_jp "Close this tile" "このタイルを閉じる"
::msgcat::mcset ja_jp "Goto Line?" "指定行へ跳ぶ?"
::msgcat::mcset ja_jp "Go to:" "跳ぶ:"
::msgcat::mcset ja_jp "&logical line" "実際の行(&l)"
::msgcat::mcset ja_jp "&physical line" "仮想の行(&p)"
::msgcat::mcset ja_jp "in &function" "関数で(&f)"
::msgcat::mcset ja_jp "in &current file" "現在のファイルで(&c)"
::msgcat::mcset ja_jp "Outside of function definition"\
                      "関数定義の外側"

# colorize.tcl
::msgcat::mcset ja_jp "Wait seconds while recolorizing file"\
                      "ファイルの色付けに二秒ほど待ってください"
::msgcat::mcset ja_jp "You can't do that while colorization is in progress.\
                       Please try again when finished."\
                      "色付けしている間は何もすることができません。\
                       終わらせて、再度試みてください。"
::msgcat::mcset ja_jp "Command forbidden during colorization"\
                      "コマンドは色付けの間は禁止されます"

# completion.tcl
::msgcat::mcset ja_jp "No possible completion" "完了できません"
::msgcat::mcset ja_jp "possible completions" "完了できます"
::msgcat::mcset ja_jp "Possible completions" "完了できます"

# db_argsmanagement.tcl
::msgcat::mcset ja_jp "Add" "追加"
::msgcat::mcset ja_jp "This variable can be watched but cannot be edited!"\
                      "この変数は、参照のみで編集することはできません!!"
::msgcat::mcset ja_jp "A new watch variable will be created."\
                      "新しい可視変数は、作られます。"
::msgcat::mcset ja_jp "Non editable variable" "編集できない変数"
::msgcat::mcset ja_jp "Variable:" "変数:"
::msgcat::mcset ja_jp "Expression:" "表現:"
::msgcat::mcset ja_jp "Value:" "値:"
::msgcat::mcset ja_jp "&Get current value from Scilab"\
                      "Scilabから現在の値を取得(&G)"
::msgcat::mcset ja_jp "Non watchable variable" "可視変数"
::msgcat::mcset ja_jp "Sorry, variable" "すみません、変数です"
::msgcat::mcset ja_jp "is special and cannot be watched."\
                      "それは特別で見ることができません。"

# db_bptsgui.tcl
::msgcat::mcset ja_jp "Breakpoints properties" "ブレーク・ポイント プロパティ"
::msgcat::mcset ja_jp "Enabled" "有効"
::msgcat::mcset ja_jp "Expression" "表現"
::msgcat::mcset ja_jp "Hit if" "ヒットのとき"
::msgcat::mcset ja_jp "istrue" "真のとき"
::msgcat::mcset ja_jp "haschanged" "変化有り"
::msgcat::mcset ja_jp "Watched" "監視"
::msgcat::mcset ja_jp "Hit count" "ヒット数"
::msgcat::mcset ja_jp "Break if" "中断のとき"
::msgcat::mcset ja_jp "Threshold" "敷居"
::msgcat::mcset ja_jp "always" "常時"
::msgcat::mcset ja_jp "equalto" "等しい"
::msgcat::mcset ja_jp "multipleof" "多数"
::msgcat::mcset ja_jp "greaterthan" "以上"
::msgcat::mcset ja_jp "lessthan" "以下"
::msgcat::mcset ja_jp "Reset hit" "リセット"
::msgcat::mcset ja_jp "Breakpoints vocabulary"\
                      "ブレーク・ポイント語彙"
::msgcat::mcset ja_jp "A breakpoint is identified by its location, i.e. its\
                       position (a line number) in a function."\
                      "ブレーク・ポイントは、その場所によって明らかにされます、\
                       関数(行番号)の位置に置いて下さい。"
::msgcat::mcset ja_jp "When the execution process encounters an enabled\
                       breakpoint, then this breakpoint is said to be\
                       reached."\
                      "処理中に有効なブレーク・ポイントに遭遇したとき、\
                       このブレーク・ポイントは達します。"
::msgcat::mcset ja_jp "When the breakpoint is reached and its associated\
                       conditional expression is true (or changed), then the\
                       breakpoint is said to be hit."\
                      "ブレーク・ポイントに到達し関連の条件式が真であるときに\
                       (あるいは変えられた)、ブレーク・ポイントは、ヒットするでしょう。"
::msgcat::mcset ja_jp "The hit count is the number of times the breakpoint\
                       has been hit."\
                      "ヒットカウントは、ブレーク・ポイントがヒットを受けた数です。"
::msgcat::mcset ja_jp "Execution will stop at a breakpoint if the hit count\
                       satisfies the selectable break condition."\
                      "ヒットカウントが選択できるブレーク条件を満たせば、\
                       実行は、ブレーク・ポイントで停止します。"

# db_bptsmanagement.tcl
::msgcat::mcset ja_jp "No breakpoint here!"\
                      "ここにはブレークポイントはありません!"
::msgcat::mcset ja_jp "Delete all breakpoints"\
                      "全てのブレークポイントを削除"
::msgcat::mcset ja_jp "Are you sure you want to delete all the existing\
                       breakpoints?"\
                      "全てのブレークポイントを削除しても良いですか?"
::msgcat::mcset ja_jp "All breakpoints deleted" "全てのブレークポイントは削除されました"
::msgcat::mcset ja_jp "No action taken" "いかなる活動も取られません"
::msgcat::mcset ja_jp "No breakpoint!" "ブレークポイントではありません!"

# db_configureexec.tcl
::msgcat::mcset ja_jp "Configure execution" "構成の実行"
::msgcat::mcset ja_jp "Function name:" "関数名:"
::msgcat::mcset ja_jp "&Obtain" "取得(&O)"
::msgcat::mcset ja_jp "Input arguments:" "入力引数:"
::msgcat::mcset ja_jp "The selected function has not been found in the\
                       opened files, maybe it was deleted since last\
                       configuration of the debugger.\nUse button \""\
                      "選択された機能は、開かれたファイルで見つけられません\
                       でした、たぶん、それは、最新のデバッガの配置から削除\
                       されました。ボタンを使用してください。"
::msgcat::mcset ja_jp "\" to refresh the list of available functions."\
                      "\" 利用できる機能のリストをリフレッシュします。"
::msgcat::mcset ja_jp "Function name or input arguments do not match\
                       definition of the function"\
                      "関数名、または入力引数は、関数の定義とマッチしません"
::msgcat::mcset ja_jp "in the file!\n\nCheck function name and arguments\
                       (names, order) in the configuration dialog.\
                       \nArguments order can be changed using drag and drop\
                       with right mouse button in the arguments listbox."\
                      "ファイルで!\n\n関数名と構成ダイアログの引数(名前、順序)\
                       確認してください。引数順序は、引数リストボックスで\
                       マウス右ボタンのドラッグ・アンド・ドロップを使うこと\
                       ができます。"
::msgcat::mcset ja_jp "Error in selected function name or arguments"\
                      "選択された関数名、または引数のエラーです"
::msgcat::mcset ja_jp "At least one input argument does not match definition\
                       of the function"\
                      "最低１つの入力引数は、関数の定義と一致しなければなりません"
::msgcat::mcset ja_jp "in the file!\n\nCheck function arguments (their\
                       names) in the configuration dialog."\
                      "ファイルで!\n\n関数名と構成ダイアログの引数(名前)\
                       を確認してください。"
::msgcat::mcset ja_jp "Error in selected function arguments"\
                      "選択関数の引数がエラーです"
::msgcat::mcset ja_jp "Main level code found" "主となるコードを見つけました"
::msgcat::mcset ja_jp "Scipad ancillary file" "Scipad補助ファイル"
::msgcat::mcset ja_jp "The current file contains only Scipad ancillaries\
                       that cannot be debugged in Scipad."\
                      "現ファイルは、Scipadでデバッグすることができない唯一\
                       のScipad補助を含みます。"
::msgcat::mcset ja_jp "Reserved function names are:\n"\
                      "予約関数名:\n"
::msgcat::mcset ja_jp "See help scipad for further details."\
                      "それ以上の詳細に関してscipadヘルプを参照して下さい。"
::msgcat::mcset ja_jp "This file contains main level code.\nDebug as a\
                       single .sce file?"\
                      "このファイルは、メインコードを含みます。\n識別子.sce\
                       ファイルとしてデバッグしますか?"
::msgcat::mcset ja_jp "No function found" "関数が見つかりません"
::msgcat::mcset ja_jp "This file contains no function and will be treated"\
                      "このファイルは、いかなる関数も無しで扱われます"
::msgcat::mcset ja_jp "as a single .sce file.\nStart debugging?"\
                      "識別子.sceファイル\nデバッグを開始しますか?"
::msgcat::mcset ja_jp "You must enable colorization to be able to launch the\
                       debugger!"\
                      "色付けを可能にして　デバッガを始めなくてはいけません!"
::msgcat::mcset ja_jp "The debugger needs colorization"\
                      "デバッガは色付けが必須です"
::msgcat::mcset ja_jp "Warning!\n\n  A function missing the closing\
                       \"endfunction\" has been found in the currently\
                       opened files.\n  Such syntax is now obsolete.\n\n \
                       Please terminate function "\
                      "警告!\n\n 現在の開いているファイルで、\"endfunction\"\
                       が見つかりました。関数の間違いです。\n現在の文法で用いら\
                       れません。\n\n関数を終了してください"
::msgcat::mcset ja_jp " and try again." " そして、もう一度試してください。"
::msgcat::mcset ja_jp "Note: This message might also be triggered by a\
                       string\n  quoted with single quotes and containing\
                       the word \"function\",\n  when \"Colorize\
                       \'strings\'\" is unchecked in the Options menu."\
                      "注意: このメッセージは、シングルクォートされた文字列\
                       引用によってきっかけになります。そして、オプション・\
                       メニューで「色付け文字列」のチェックをはずされる「関数」\
                       語を含みます。"
::msgcat::mcset ja_jp "This known bug can be worked around simply by\
                       checking \"Colorize \'&strings\'\"."\
                      "この既知のバグは、単に「色づけ文字列(&s)」を調べることに\
                       よって動作できます。"
::msgcat::mcset ja_jp "Unterminated function definition found"\
                      "終結しない関数定義が見つかりました"
::msgcat::mcset ja_jp "Warning!\n\n  One or more function is defined more\
                       than once in the currently opened files.\n  The\
                       debugger cannot cope with multiple copies of\
                       functions.\n  The following duplicates were\
                       detected:\n"\
                      "警告! ひとつ、またはより多くの関数は、現在の一度開かれた\
                       ファイルよりも多く定義されいます。デバッガは、関数の多数\
                       のコピーに対処することができません。以下の重複が発見され\
                       ました:"
::msgcat::mcset ja_jp "\n--> Definition of function "\
                      "\n--> 関数の定義 "
::msgcat::mcset ja_jp " found in:\n" " 基づく:\n"
::msgcat::mcset ja_jp "Duplicate function definitions found"\
                      "重複した関数が見つかりました"


# db_debugsession.tcl
::msgcat::mcset ja_jp "Too many breakpointed functions"\
                      "多すぎるブレークポイント関数"
::msgcat::mcset ja_jp "You have currently" "現在持っている"
::msgcat::mcset ja_jp "functions in your opened files." "開かれたファイルの関数。"
::msgcat::mcset ja_jp "Scilab supports a maximum of"\
                      "Scilabが最大を支えます、"
::msgcat::mcset ja_jp "possible breakpointed functions (see help setbpt)."\
                      "可能なブレークポイント関数(setbptヘルプを見てください)"
::msgcat::mcset ja_jp "Step-by-step hence cannot be performed."\
                      "段階的なことは実行することができません。"
::msgcat::mcset ja_jp "This command will actually run to the next breakpoint."\
                      "この命令は、現実には次のブレークポイントに達するでしょう。"
::msgcat::mcset ja_jp "Break hence cannot be performed."\
                      "ブレークすることができません。"
::msgcat::mcset ja_jp "Too many breakpoints" "多すぎるブレークポイント"
::msgcat::mcset ja_jp "Executing this command would require to set"\
                      "セットするためには、この命令の実行が必要です。"
::msgcat::mcset ja_jp "breakpoints in your opened files."\
                      "開かれたファイルのブレークポイント。"
::msgcat::mcset ja_jp "possible breakpoints in Scilab (see help setbpt)."\
                      "Scilabの可能なブレークポイント(setbptヘルプを見てください)"
::msgcat::mcset ja_jp "Cursor must be in a function"\
                      "カーソルは、関数内で可能です"
::msgcat::mcset ja_jp "This command cannot succeed if the function where the\
                       debug just stopped is not opened in Scipad!"\
                      "この命令は成功しませんでした、もし関数がちょうどデバッグ\
                       で止められたところではScipadで開けません!"
::msgcat::mcset ja_jp "Cannot run to return point"\
                      "戻り位置への実行ができません"
::msgcat::mcset ja_jp "Warning!\nThe line where the debugger just stopped\
                       contains a return statement (either endfunction,\
                       return or resume) but this statement is not the first\
                       one on the line."\
                      "警告!\nデバッガが止まった行は、戻り文を含みます。\
                       (終了関数、戻りまたは回復のどちらか) しかし、この文は、\
                       行の最初のものではありません。"
::msgcat::mcset ja_jp "As a consequence the debugger cannot predict if the\
                       next debug step will really jump outside of the\
                       current function, which is what you just asked the\
                       debugger to do."\
                      "デバッガは予期しない結果を得ました、次のデバッグステップ\
                       は、現在の関数の外に本当に跳ぶでしょう、それは、あなたが\
                       デバッガに指示したものです。"
::msgcat::mcset ja_jp "To avoid this unresolvable case, it is recommended to\
                       place the return statement alone on its line."\
                      "この解決できない事例を避けるために、その行に戻りの文だけ\
                       を置くことが推奨されます。"
::msgcat::mcset ja_jp "Do you want to see this warning again during this\
                       Scipad session?"\
                      "あなたは、再びこのScipadセッションで警告を見たいですか?"
::msgcat::mcset ja_jp "Unresolvable case found while running to return point"\
                      "戻りポイントのために、実行中に解決できない事象が見つかりました"
::msgcat::mcset ja_jp "No effect - The debugged file is not stuck"\
                      "結果なし- デバッグされたファイルは、スタックではありません"


# db_init.tcl
::msgcat::mcset ja_jp "Please wait..." "しばらくお待ちください..."


# db_states.tcl
::msgcat::mcset ja_jp "Currently no debug session"\
                      "現在はデバッグセッションではありません"
::msgcat::mcset ja_jp "Ready to start debug" "デバッグ開始準備ができました"
::msgcat::mcset ja_jp "Debug in progress" "デバッグ中"
::msgcat::mcset ja_jp "Scilab is back at the main level now."\
                      "Scilabはメインの状態に戻ります。"


# db_watch.tcl
::msgcat::mcset ja_jp "Watch" "可視"
::msgcat::mcset ja_jp "Watch variables:" "可視変数:"
::msgcat::mcset ja_jp "A&uto (local variables)"\
                      "自動(&u) (ローカル変数)"
::msgcat::mcset ja_jp "Au&to watch globals too"\
                      "自動(&t) 全体監視"
::msgcat::mcset ja_jp "Generic expressions:" "総称表現:"
::msgcat::mcset ja_jp "Call stack:" "スタック呼び出し:"
::msgcat::mcset ja_jp "Always on to&p" "常に前面へ(&p)"
::msgcat::mcset ja_jp "Hide watch &variables" "可視関数を隠す(&v)"
::msgcat::mcset ja_jp "Hide e&xpressions" "表現を隠す(&x)"
::msgcat::mcset ja_jp "Hide call &stack" "スタック呼び出しを隠す(&s)"
::msgcat::mcset ja_jp "&Dock" "減らす(&D)"
::msgcat::mcset ja_jp "&Add" "追加(&A)"
::msgcat::mcset ja_jp "&Filter errors" "フィルターエラー(&F)"
::msgcat::mcset ja_jp "Breakpoints" "ブレークポイント"
::msgcat::mcset ja_jp "Breakpointed functions" "ブレークポイント関数"
::msgcat::mcset ja_jp "Scilab busy indicator"\
                      "Scilab表示機能はふさがっています"
::msgcat::mcset ja_jp "Debugger state indicator"\
                      "デバッカ表示機能はふさがっています"
::msgcat::mcset ja_jp "Undefined elements are not legal in variable"\
                      "定義されない要素は変数に適切ではありません。"
::msgcat::mcset ja_jp ".\nThis variable will not be updated in Scilab."\
                      ".\nこの変数は、Scilabで更新されないでしょう。"
::msgcat::mcset ja_jp "Illegal undefined element found"\
                      "不確定の要素が見つかりました"
::msgcat::mcset ja_jp "evaluates to:" "評価します:"
::msgcat::mcset ja_jp "produces error" "エラーを引き起こす"
::msgcat::mcset ja_jp "Scipad cannot find source code for the selected\
                       function.\nPlease open the adequate file manually and\
                       try again."\
                      "Scipadは、ソース・コードを選択された関数で見つけること\
                       ができません。どうぞ手動で妥当なファイルを開いて、そして\
                       また試みて下さい。"
::msgcat::mcset ja_jp "Missing source code" "ソースコードの間違い"


# filecommands.tcl
::msgcat::mcset ja_jp "New script" "新しいスクリプト"
::msgcat::mcset ja_jp "New view on the same file"\
                      "同じファイルの新しいビュー"
::msgcat::mcset ja_jp "The contents of" "内容"
::msgcat::mcset ja_jp "may have changed, do you wish to save your changes?"\
                      "変わったかもしれません、あなたは、変更を保ちたいですか?"
::msgcat::mcset ja_jp "Save Confirm?" "保存しますか?"
::msgcat::mcset ja_jp "Open source of..." "ソースを開く..."
::msgcat::mcset ja_jp "Open source of:" "ソースを開く:"
::msgcat::mcset ja_jp "Weird drag&drop" "奇妙なドラッグ/ドロップ(&d)"
::msgcat::mcset ja_jp "is neither a valid filename nor a valid directory.\
                       Either you're dragging an object of another type, or\
                       you hit a bug of the dnd mechanism."\
                      "適切なファイルネームでもディレクトリーでもありません。\
                       あなたが別のタイプのオブジェクトをドラッグしたか、あるいは、\
                       dndメカニズムのバグに遭遇しました。"
::msgcat::mcset ja_jp "File not found" "ファイルは見つかりません"
::msgcat::mcset ja_jp "does not exist anymore. Do you want to create an\
                       empty file with the same name?"\
                      "今はもう存在しません。あなたは、同名で空のファイルを作りたい\
                       ですか?"
::msgcat::mcset ja_jp "Open file" "ファイルを開く"
::msgcat::mcset fr_fr "The file" "ファイル"
::msgcat::mcset ja_jp "is already opened! Save the current opened file to\
                       another name and reopen it from disk!"\
                      "すでに開かれます! 別名で現在の開かれたファイルを保存して、\
                       そしてディスクからそれを再開して下さい!"
::msgcat::mcset ja_jp "File" "ファイル"
::msgcat::mcset ja_jp "saved" "保存"
::msgcat::mcset ja_jp "cannot be written!" "書き込めません!"
::msgcat::mcset ja_jp "The contents of " "内容"
::msgcat::mcset ja_jp "has changed on disk, save it anyway?"\
                      "ディスクで変わりました、とにかく保存しますか?"
::msgcat::mcset ja_jp "The contents and/or properties of " "内容/特性、"
::msgcat::mcset ja_jp "has been modified outside of Scipad. Do you want to\
                       reload it?"\
                      "Scipadの外で修正されました。それをリロードしますか?"
::msgcat::mcset ja_jp "File has changed!" "ファイルは変わりました!"
::msgcat::mcset ja_jp "Save as" "名前を付けて保存"
::msgcat::mcset ja_jp "Scilab files" "Scilab ファイル"
::msgcat::mcset ja_jp "Scicos files" "Scicos ファイル"
::msgcat::mcset ja_jp "XML files" "XML ファイル"
::msgcat::mcset ja_jp "Wait seconds while loading and colorizing file"\
                      "色付けしたファイルをロードしている間、2分ほど待ってください。"
::msgcat::mcset ja_jp "The contents of this file may have changed, do you\
                       wish to save your changes?"\
                      "ファイルは変えられたかもしれません、変更分を保存しますか?"
::msgcat::mcset ja_jp "Revert" "戻す"
::msgcat::mcset ja_jp "to saved?" "保存しますか?"
::msgcat::mcset ja_jp "Revert Confirm?" "戻ります、確認?してください"
::msgcat::mcset ja_jp "Unreadable file" "読めないファイル"
::msgcat::mcset ja_jp "exists but is not readable!"\
                      "存在しますが読みやすくありません!"
::msgcat::mcset ja_jp "This file is rather large and colorization might take\
                       some time.\n\nColorize anyway?"\
                      "このファイルは大きいため色付けはある時間を要します。\
                       とにかく色付けしますか?"
::msgcat::mcset ja_jp "File size warning" "ファイルサイズの警告をします"
::msgcat::mcset ja_jp "Windows shortcut file" "ウインドウズのショートカットファイル"
::msgcat::mcset ja_jp "is a Windows shortcut and will not be opened!"\
                      "ウインドウズのショートカットは開けません!"

# findreplace.tcl
::msgcat::mcset ja_jp "Dialog box already open!"\
                      "ダイアログボックスは既にオープンしています!"
::msgcat::mcset ja_jp "The string" "文字列"
::msgcat::mcset ja_jp "could not be found in" "見つけられません"
::msgcat::mcset ja_jp "could not be found in any of the opened files !"\
                      "開かれたファイルのいずれにも見つけられません!"
::msgcat::mcset ja_jp "replacements done" "変更完了"
::msgcat::mcset ja_jp "Find" "検索"
::msgcat::mcset ja_jp "No match found in the selection for"\
                      "一致したものは選択から見つかりません"
::msgcat::mcset ja_jp "\nWould you like to look for it in the entire text?"\
                      "\n全体のテキストでそれを探したいですか?"
::msgcat::mcset ja_jp "You are searching for an empty string!"\
                      "空の文字列を検索しています!"
::msgcat::mcset ja_jp "Re&place" "置換(&p)"
::msgcat::mcset ja_jp "Find what:" "検索:"
::msgcat::mcset ja_jp "Replace with:" "置換:"
::msgcat::mcset ja_jp "Find &Next" "次を検索(&N)"
::msgcat::mcset ja_jp "Replace &All" "全て置換(&A)"
::msgcat::mcset ja_jp "Direction" "方向"
::msgcat::mcset ja_jp "&Upwards" "上へ(&U)"
::msgcat::mcset ja_jp "&Downwards" "下へ(&D)"
::msgcat::mcset ja_jp "Text types" "テキスト型"
::msgcat::mcset ja_jp "Find options" "検索オプション"
::msgcat::mcset ja_jp "Match &whole word only" "全体語の一致(&w)"
::msgcat::mcset ja_jp "Match &case" "部分一致(&c)"
::msgcat::mcset ja_jp "&Regular expression" "正規表現(&R)"
::msgcat::mcset ja_jp "In all &opened files" "開かれたファイル全て(&o)"
::msgcat::mcset ja_jp "In &selection only" "選択されたものだけ(&s)"
::msgcat::mcset ja_jp "In typed te&xt" "入力されたテキスト(&x)"
::msgcat::mcset ja_jp "In a director&y" "ディレクトリ(&y)"
::msgcat::mcset ja_jp "Find in files search options"\
                      "検索オプションで見つけたファイル"
::msgcat::mcset ja_jp "Directory r&ecurse search" "ディレクトリの再検索"
::msgcat::mcset ja_jp "Include &hidden files" "隠しファイルを含む(&h)"
::msgcat::mcset ja_jp "In files/file types:" "ファイル/ファイル型:"
::msgcat::mcset ja_jp "In directory:" "ディレクトリ:"
::msgcat::mcset ja_jp "Search for file names" "ファイル名の検索"
::msgcat::mcset ja_jp "You have given an empty search string. Are you sure\
                       you want to search among file names?"\
                      "空っぽの検索文字列を与えました。ファイル名の検索は確かですか？"
::msgcat::mcset ja_jp "Wrapped around" "検索が終わりました"
::msgcat::mcset ja_jp "Back at the first file searched!"\
                      "最初のファイルを見つけて戻りました!"
::msgcat::mcset ja_jp "No more matches" "これ以上の一致はありません"
::msgcat::mcset ja_jp "The string to regexp search for cannot be compiled by\
                       Tcl.\nCheck again, maybe certain characters should be\
                       escaped."\
                      "Tclでregexp検索への文字列を編集することができません。\
                       \n再度調べて下さい、たぶん確かな文字を外すべきです。"
::msgcat::mcset ja_jp "Any single character" "任意の一文字"
::msgcat::mcset ja_jp "Zero or more" "ゼロ以上"
::msgcat::mcset ja_jp "One or more" "ひとつ以上"
::msgcat::mcset ja_jp "Zero or one" "ゼロかひとつ"
::msgcat::mcset ja_jp "Any one character in the set"\
                      "何かしらの文字を設定"
::msgcat::mcset ja_jp "Any one character not in the set"\
                      "何の文字も設定しない"
::msgcat::mcset ja_jp "Or" "また"
::msgcat::mcset ja_jp "Beginning of line" "行の開始"
::msgcat::mcset ja_jp "End of line" "行の終わり"
::msgcat::mcset ja_jp "Beginning of word" "ワードの開始"
::msgcat::mcset ja_jp "End of word" "ワードの終わり"
::msgcat::mcset ja_jp "A letter" "手紙"
::msgcat::mcset ja_jp "A decimal digit" "10進数字"
::msgcat::mcset ja_jp "An alphanumeric (letter or digit)"\
                      "アルファベット(手紙、または10進数字)"
::msgcat::mcset ja_jp "A space or tab character" "スペース、またはタブ文字"
::msgcat::mcset ja_jp "A rational number" "有理数"
::msgcat::mcset ja_jp "A floating point number" "浮動小数点"
::msgcat::mcset ja_jp "A Scilab comment" "Scilabコメント"
::msgcat::mcset ja_jp "A Scilab name" "Scilab名"
::msgcat::mcset ja_jp "A Scilab continued line (with dots)"\
                      "Scilab 継続行(点).."


# findinfiles.tcl
::msgcat::mcset ja_jp "Directory access issue"\
                      "ディレクトリーのアクセス"
::msgcat::mcset ja_jp "The directory you specified cannot be reached. The\
                       search won't bring any result."\
                      "明確に述べたディレクトリーに達することができません。\
                       検索は、どんな結果もありません。"
::msgcat::mcset ja_jp "Some directories or files could not be open and were\
                       ignored during search.\n You might miss read access\
                       to them.\n\n The following ones were ignored"\
                      "いくつかのディレクトリ、またはファイルは開けません、\
                       検索の間に無視されました。\n それらは読むことができません。\
                       \n\n 以下のものは無視されました"
::msgcat::mcset ja_jp "Ignored files" "無視したファイル"
::msgcat::mcset ja_jp "Searching in file:" "検索ファイル:"
::msgcat::mcset ja_jp "End of file search:" "ファイル検索の終り:"
::msgcat::mcset ja_jp "matches found" "一致が見つかりました"
::msgcat::mcset ja_jp "searched files" "検索されたファイル"
::msgcat::mcset ja_jp "Constructing file list:" "構成ファイル・リスト:"
::msgcat::mcset ja_jp "&Previous" "戻る(&P)"
::msgcat::mcset ja_jp "&Next" "次へ(&N)"
::msgcat::mcset ja_jp "Scipad search results" "Scipad検索結果"


# fonts.tcl
::msgcat::mcset ja_jp "Choose font" "フォント選択"
::msgcat::mcset ja_jp "Family" "ファミリィ"
::msgcat::mcset ja_jp "Style" "スタイル"
::msgcat::mcset ja_jp "&Bold" "太い線(&B)"
::msgcat::mcset ja_jp "&Italic" "イタリック(&I)"
::msgcat::mcset ja_jp "&Underline" "下線(&U)"
::msgcat::mcset ja_jp "Size" "サイズ"
::msgcat::mcset ja_jp "&Defaults" "標準(&D)"
::msgcat::mcset ja_jp "Use as..." "使います..."
::msgcat::mcset ja_jp "&Text font" "テキストフォント(&T)"
::msgcat::mcset ja_jp "&Menu font" "メニューフォント(&M)"
::msgcat::mcset ja_jp "Sample" "サンプル"
::msgcat::mcset ja_jp "Font size" "フォントサイズ"
::msgcat::mcset ja_jp "Revert fonts" "フォントの復帰"
::msgcat::mcset ja_jp "Are you sure you want to revert all fonts to defaults?"\
                      "全てのフォントを標準に戻しますか?"


# FormatStringsForWatch.sci
::msgcat::mcset ja_jp " what you try to watch is not supported by the\
                       debugger"\
                      "試みようとしているものは、デバッガに支援されません"
::msgcat::mcset ja_jp " what you try to watch is of type "\
                      " 試みようとしているものに型があります "
::msgcat::mcset ja_jp " - this type is not supported by the debugger"\
                      " - この型は、デバッガに支援されません"
::msgcat::mcset ja_jp "Type:" "型:"
::msgcat::mcset ja_jp "(unsupported)" "(サポートされない)"
::msgcat::mcset ja_jp "size:" "サイズ:"
::msgcat::mcset ja_jp "real" "実数"
::msgcat::mcset ja_jp "complex" "複素数"
::msgcat::mcset ja_jp "elements" "要素"
::msgcat::mcset ja_jp "graphic handle" "グラフィックハンドル"
::msgcat::mcset ja_jp "uncompiled function" "コンパイルされていない関数"
::msgcat::mcset ja_jp "compiled function" "コンパイル済み関数"
::msgcat::mcset ja_jp "library" "ライブラリ"
::msgcat::mcset ja_jp "pointer" "ポインタ"
::msgcat::mcset ja_jp "size implicit index" "陰関数インデックス・サイズ"
::msgcat::mcset ja_jp "primitive" "原始"


# FormatWhereForWatch.sci
::msgcat::mcset ja_jp "Breakpoint" "ブレークポイント"
::msgcat::mcset ja_jp "called at line" "行で呼び出し"
::msgcat::mcset ja_jp "of exec file" "実行ファイルの"
::msgcat::mcset ja_jp "of execstr instruction" "文字列実行指示の"
::msgcat::mcset ja_jp "called under pause" "休止のもとで呼ばれました"
::msgcat::mcset ja_jp "of macro" "マクロの"

# helps.tcl
::msgcat::mcset ja_jp "About" "バージョン情報"
::msgcat::mcset ja_jp "Scipad_is_released_under_GPL"\
                      "SciPad は、GPLライセンスの下にリリースされています"


# infomessages.tcl
::msgcat::mcset ja_jp "Line:" "行:"
::msgcat::mcset ja_jp "Column:" "列:"
::msgcat::mcset fr_fr "Line" "行" 
::msgcat::mcset ja_jp "in" "内"
::msgcat::mcset ja_jp "Logical line:" "論理行:"
::msgcat::mcset ja_jp " \[ReadOnly\]" " \[読み取り専用\]"
::msgcat::mcset ja_jp " (modified)" " (修正)"


# inputtext.tcl
::msgcat::mcset ja_jp "No corresponding <" "一致しません <"
::msgcat::mcset ja_jp "> found!" "> 見つかりました!"
::msgcat::mcset ja_jp "Code editing is not allowed during debug!"\
                      "デバッグ中はコード編集できません!"

# menues.tcl
::msgcat::mcset ja_jp "&File" "ファイル(&F)"
::msgcat::mcset ja_jp "&Edit" "編集(&E)"
::msgcat::mcset ja_jp "&Search" "検索(&S)"
::msgcat::mcset ja_jp "E&xecute" "実行(&x)"
::msgcat::mcset ja_jp "&Debug" "デバッグ(&D)"
::msgcat::mcset ja_jp "S&cheme" "図解(&c)"
::msgcat::mcset ja_jp "&Options" "オプション(&O)"
::msgcat::mcset ja_jp "&Windows" "表示(&W)"
::msgcat::mcset ja_jp "&Help" "ヘルプ(&H)"

# menues.tcl - File menu
::msgcat::mcset ja_jp "&New" "新規(&N)"
::msgcat::mcset ja_jp "&Open..." "開く(&O)..."
::msgcat::mcset ja_jp "Open &in new" "新規で開く(&i)"
::msgcat::mcset ja_jp "&horizontal tile" "水平のタイル(&h)"
::msgcat::mcset ja_jp "&vertical tile" "垂直のタイル(&v)"
::msgcat::mcset ja_jp "Op&en source of..." "ソースを開く(&e)..."
::msgcat::mcset ja_jp "&Save" "保存(&S)"
::msgcat::mcset ja_jp "Save &as..." "名前を付けて保存(&a)..."
::msgcat::mcset ja_jp "Save a&ll" "全て保存(&l)"
::msgcat::mcset ja_jp "&Revert..." "元に戻す(&R)..."
::msgcat::mcset ja_jp "Import &Matlab file..." "Matlabファイルのインポート(&M).."
::msgcat::mcset ja_jp "Create help s&keleton..."\
                      "XMLヘルプの骨組みを生成.."
::msgcat::mcset ja_jp "Compile as &help page" "ヘルプ・ページとしてコンパイル(&h)"
::msgcat::mcset ja_jp "Open &function source"\
                      "関数ソースを開く(&f)"
::msgcat::mcset ja_jp "Print Se&tup" "印刷設定(&t)"
::msgcat::mcset ja_jp "&Print" "印刷(&P)"
::msgcat::mcset ja_jp "&Close file" "ファイルを閉じる(&C)"
::msgcat::mcset ja_jp "E&xit" "終了(&E)"

# menues.tcl - Edit menu
::msgcat::mcset ja_jp "&Undo" "元に戻す(&U)"
::msgcat::mcset ja_jp "&Redo" "やり直す(&R)"
::msgcat::mcset ja_jp "Cu&t" "切り取り(&t)"
::msgcat::mcset ja_jp "B&lock cut" "まとめて切り取る(&l)"
::msgcat::mcset ja_jp "&Copy" "コピー(&C)"
::msgcat::mcset ja_jp "&Paste" "貼り付け(&P)"
::msgcat::mcset ja_jp "&Block paste" "まとめて貼り付け(&B)"
::msgcat::mcset ja_jp "&Delete" "削除(&D)"
::msgcat::mcset ja_jp "Select &All" "全て選択(&A)"
::msgcat::mcset ja_jp "Insert &Time/Date" "日付の挿入(&T)"
::msgcat::mcset ja_jp "Co&mment selection" "コメントを選択(&m)"
::msgcat::mcset ja_jp "U&ncomment selection" "コメント解除(&n)"
::msgcat::mcset ja_jp "&Indent selection" "インデント選択(&I)"
::msgcat::mcset ja_jp "Unin&dent selection" "インデント解除(&d)"

# menues.tcl - Search menu
::msgcat::mcset ja_jp "&Find..." "検索(&F)..."
::msgcat::mcset ja_jp "Find &Next" "次を検索(&N)"
::msgcat::mcset ja_jp "&Replace..." "置換(&R)..."
::msgcat::mcset ja_jp "&Goto Line..." "指定行へ(&G)..."

# menues.tcl - Exec menu
::msgcat::mcset ja_jp "&Load into Scilab" "Scilab にロード(&L)"
::msgcat::mcset ja_jp "Load &all into Scilab" "Scilab に全てロード(&a)"
::msgcat::mcset ja_jp "&Evaluate selection" "評価を選択(&E)"

# menues.tcl - Debug menu
::msgcat::mcset ja_jp "&Insert/Remove breakpoint"\
                      "ブレークポイントの挿入/削除(&I)"
::msgcat::mcset ja_jp "&Edit breakpoints" "ブレークポイントの編集(&E)"
::msgcat::mcset ja_jp "Remove &all breakpoints" "全てのブレークポイントの削除(&a)"
::msgcat::mcset ja_jp "&Configure execution..." "構成の実行(&C)..."
::msgcat::mcset ja_jp "Go to next b&reakpoint" "次のブレークポイントへ(&r)"
::msgcat::mcset ja_jp "&Step by step" "順次(&S)"
::msgcat::mcset ja_jp "Step &into" "ステップに入る(&i)"
::msgcat::mcset ja_jp "Step o&ver" "ステップを越える(&v)"
::msgcat::mcset ja_jp "Step &out" "ステップを出る(&o)"
::msgcat::mcset ja_jp "Run to re&turn point" "戻り点へ実行(&t)"
::msgcat::mcset ja_jp "Run to c&ursor" "カーソルへ実行(&u)"
::msgcat::mcset ja_jp "G&o on ignoring any breakpoint"\
                      "ブレーク・ポイントを無視して続ける"
::msgcat::mcset ja_jp "Show &watch" "監視を表示(&w)"
::msgcat::mcset ja_jp "&Break" "ブレーク(&B)"
::msgcat::mcset ja_jp "Cance&l debug" "デバッグをキャンセル(&l)"

# menues.tcl - Scheme menu
::msgcat::mcset ja_jp "S&cilab" "&Scilab"
::msgcat::mcset ja_jp "&XML" "&XML"
::msgcat::mcset ja_jp "&none" "無し(&n)"
::msgcat::mcset ja_jp "&Colorize" "カラー設定(&C)"

# menues.tcl - Options menu
::msgcat::mcset ja_jp "&Fonts..." "フォント(&F)..."
::msgcat::mcset ja_jp "&Colors" "カラー(&C)"
::msgcat::mcset ja_jp "Colori&ze" "カラー設定(&o)"
::msgcat::mcset ja_jp "&Always" "常時(&S)"
::msgcat::mcset ja_jp "Ask for &big files" "大きいファイルを求める(&b)"
::msgcat::mcset ja_jp "&Never" "ない(N)"
::msgcat::mcset ja_jp "Colorize \'&strings\'" "文字列の色付け(&s)"
::msgcat::mcset ja_jp "Colorize &user functions" "ユーザ関数の色付け(&u)"
::msgcat::mcset ja_jp "Show c&ontinued lines" "継続行の表示(&o)"
::msgcat::mcset ja_jp "Line n&umbers" "行番号(&u)"
::msgcat::mcset ja_jp "&Hide" "隠す(&H)"
::msgcat::mcset ja_jp "&Left aligned" "左に並べる(&L)"
::msgcat::mcset ja_jp "&Right aligned" "右に並べる(&R)"
::msgcat::mcset ja_jp "Word &wrap" "ワードラップ(&w)"
::msgcat::mcset ja_jp "&Double-click behavior" "ダブル-クリックの挙動(&D)"
::msgcat::mcset ja_jp "&Linux" "Linux(&L)"
::msgcat::mcset ja_jp "Windo&ws" "Windows(&w)"
::msgcat::mcset ja_jp "&Scilab" "Scilab(&S)"
::msgcat::mcset ja_jp "&Tabs and indentation" "タブとインデント(&T)"
::msgcat::mcset ja_jp "Tab inserts &spaces" "タブ空白の挿入(&s)"
::msgcat::mcset ja_jp "&Indentation spaces" "インデント空白(&I)"
::msgcat::mcset ja_jp "&Tab size (characters)" "タブサイズ(文字)"
::msgcat::mcset ja_jp "Use &keyword indentation" "インデントキーワードを使用(&k)"
::msgcat::mcset ja_jp "Com&pletion" "コンパイル(&p)"
::msgcat::mcset ja_jp "&Tab" "タブ(&T)"
::msgcat::mcset ja_jp "&Control-Tab" "コントロール-タブ(&C)"
::msgcat::mcset ja_jp "&Alt-Tab" "&Alt+Tab"
::msgcat::mcset ja_jp "&Shift-Tab" "&Shift+Tab"
::msgcat::mcset ja_jp "C&ontrol-Alt-Tab" "C&trl+Alt+Tab"
::msgcat::mcset ja_jp "S&hift-Control-Tab" "S&hift+Ctrl+Tab"
::msgcat::mcset ja_jp "Sh&ift-Alt-Tab" "Sh&ift+Alt+Tab"
::msgcat::mcset ja_jp "Shi&ft-Control-Alt-Tab" "Shi&ft+Ctrl+Alt+Tab"
::msgcat::mcset ja_jp "File&names" "ファイル名(&n)"
::msgcat::mcset ja_jp "&Full path" "完全なPATH"
::msgcat::mcset ja_jp "Full path if &ambiguous" "曖昧なら完全PATH(&a)"
::msgcat::mcset ja_jp "&Unambiguous pruned path" "曖昧な短縮されたPATH(&U)"
::msgcat::mcset ja_jp "Windows menu &sorting" "並べられたウインドウズメニュー(&s)"
::msgcat::mcset ja_jp "&File opening order" "開くファイルの順序(&F)"
::msgcat::mcset ja_jp "&Alphabetically" "アルファベット順(&A)"
::msgcat::mcset ja_jp "&Most recently used" "最も最近使われたもの(&M)"
::msgcat::mcset ja_jp "&Recent files" "最近のファイル(&R)"
::msgcat::mcset ja_jp "&Backup files depth" "バックアップファイル深度"
::msgcat::mcset ja_jp "&Locale" "ロケール(&L)"
::msgcat::mcset ja_jp "Execution &errors" "実行エラー(&e)"
::msgcat::mcset ja_jp "&Bindings style" "バインディングスタイル(&B)"
::msgcat::mcset ja_jp "Enc&oding" "エンコーディング(&o)"
::msgcat::mcset ja_jp "E&xit options" "終了オプション(&x)"
::msgcat::mcset ja_jp "In Scilab &shell only" "ScilabではShellのみ(&s)"
::msgcat::mcset ja_jp "Copied in a &message box"\
                      "メッセージ・ボックスでコピーされました"

# menues.tcl - Windows menu
::msgcat::mcset ja_jp "&Maximize" "最大(&M)"
::msgcat::mcset ja_jp "&Split" "分割(&S)"
::msgcat::mcset ja_jp "S&plit (side by side)" "分割・並べて(&p)"
::msgcat::mcset ja_jp "Sp&lit file" "ファイル分割(&l)"
::msgcat::mcset ja_jp "Spl&it file (side by side)"\
                      "ファイル分割・並べて(&i)"
::msgcat::mcset ja_jp "Tile all &vertically" "すべて垂直(&v)"
::msgcat::mcset ja_jp "Tile all &horizontally" "すべて水平(&h)"
::msgcat::mcset ja_jp "Space sashes &evenly" "スペースを等間隔(&e)"
::msgcat::mcset ja_jp "Opened Files" "開かれたファイル"

# menues.tcl - Help menu
::msgcat::mcset ja_jp "&Help..." "ヘルプ(&H)..."
::msgcat::mcset ja_jp "&What's?..." "不明(&W)?..."
::msgcat::mcset ja_jp "&About" "バージョン情報(&A)"
::msgcat::mcset ja_jp "Adding &translations..." "翻訳の追加(&t)..."
::msgcat::mcset ja_jp "Adding Scipad Translations..."\
                      "Scipad 翻訳の追加..."
::msgcat::mcset ja_jp "&Bugs \& wishlist" "バグと希望リスト"
::msgcat::mcset ja_jp "Scipad Known bugs" "Scipadの既知のバグ"
::msgcat::mcset ja_jp "Change&log" "変更ログ(&l)"
::msgcat::mcset ja_jp "Changes in the Scipad codebase"\
                      "Scipad コードベースに変更"


# popupmenus.tcl
::msgcat::mcset ja_jp "Open the source of" "ソースを開く"
::msgcat::mcset ja_jp "AddWatch" "可視追加"
::msgcat::mcset ja_jp "AddGenExp" "正規化追加"
::msgcat::mcset ja_jp "Jump to" "ジャンプ"


# print.tcl
::msgcat::mcset ja_jp "Print Setup" "印刷設定"
::msgcat::mcset ja_jp "Print Command: " "印刷コマンド:"


# scilabexec.tcl
::msgcat::mcset ja_jp "No point in loading an empty file!"\
                      "空のファイルをロードしても無意味です!!"
::msgcat::mcset ja_jp "Silent file save failed"\
                      "無言のファイル・セーブは失敗しました"
::msgcat::mcset ja_jp "File could not be saved in a temporary\
                       location.\nOverwrite original file?"\
                      "ファイルは、一時場所保存できないかもしれ\
                       ません。\n元のファイルを上書きしますか?"
::msgcat::mcset ja_jp "Exec done" "実行完了"
::msgcat::mcset ja_jp "Matlab files" "Matlab ファイル"
::msgcat::mcset ja_jp "Matlab file to convert" "Matlab ファイルのコンバート"
::msgcat::mcset ja_jp "Scilab is converting, please hold on..."\
                      "Scilab はコンバート中, 待ってください..."
::msgcat::mcset ja_jp "Matlab file import" "Matlab ファイルインポート"
::msgcat::mcset ja_jp "Conversion of the file failed, see the Scilab window\
                       for details -- Perhaps report the error text and the\
                       offending Matlab file to"\
                      "ファイルの変換で失敗しました、詳細に関してScilabウインドウ\
                       を参照して下さい。 -- Matlabは不正なファイル化と間違った\
                       テキストを報告します..."
::msgcat::mcset ja_jp "Please select destination path for the xml source of\
                       the help file"\
                      "どうぞヘルプファイルのxmlソースのために目的のパスを選んで下さい"
::msgcat::mcset ja_jp "Older version of" "より古いバージョン"
::msgcat::mcset ja_jp "found!" "見つけました!"
::msgcat::mcset ja_jp "An old version of" "古いバージョン"
::msgcat::mcset ja_jp "already exists: open the old file instead?"\
                      "すでに存在します: 代わりに古いファイルを開きますか?"
::msgcat::mcset ja_jp "Existing file" "既存ファイル"
::msgcat::mcset ja_jp "New skeleton" "新しい骨組み"
::msgcat::mcset ja_jp "This feature is no longer available in Scilab 5 due\
                       to bug "\
                      "この特徴は、バグのためScilab ５でもはや利用できません"
::msgcat::mcset ja_jp "See details at " "詳細を見てください: "
::msgcat::mcset ja_jp "See also:" "見て下さい:"
::msgcat::mcset ja_jp "Note: Scilab 4.x and Scilab-gtk are offering this\
                       feature."\
                      "注：Scilab 4.xとScilab-gtkがこの特徴を提供しています。"
::msgcat::mcset ja_jp "A solution is to use a backported version of Scipad\
                       inside one of these environments, for instance:"\
                      "たとえば、解決策は、これらのScipadの戻ったひとつの内部\
                       環境のバージョンを使うことです:"
::msgcat::mcset ja_jp "Feature missing from Scilab 5"\
                      "Scilab 5 から失った特徴"
::msgcat::mcset ja_jp "ScilabEval command cannot be passed to Scilab!"\
                      "ScilabEval命令は、Scilabに渡されるはずがありません!"
::msgcat::mcset ja_jp "(end of command skipped)" "(命令の終わりを飛ばしました)"
::msgcat::mcset ja_jp "Scilab working" "Scilab 活動中"
::msgcat::mcset ja_jp "Scilab is working, please wait for the prompt to\
                       execute this command!"\
                      "Scilabは活動中です、どうぞプロンプトがこの命令を実行\
                       するのを待って下さい!"
::msgcat::mcset ja_jp "Scilab is working, wait for the prompt to load file"\
                      "Scilabは活動中、プロンプトがファイルをロードするまで待機します"
::msgcat::mcset ja_jp "Scilab is working, wait for the prompt to execute the\
                       selection."\
                      "Scilabは活動中、プロンプトが選択を実行するのを待って下さい。"
::msgcat::mcset ja_jp "Scilab is working, wait for the prompt to convert a\
                       Matlab file."\
                      "Scilabは活動中、プロンプトがMatlabファイルを変換するまで待って下さい。"
::msgcat::mcset ja_jp "Scilab is working, wait for the prompt to compile the\
                       help file."\
                      "Scilabは活動中、プロンプトがヘルプファイルを編集するのを待って下さい。"
::msgcat::mcset ja_jp "Scilab is working, wait for the prompt to issue a\
                       debugger command."\
                      "Scilabは活動中、プロンプトがデバッガ命令を出すのを待って下さい。"
::msgcat::mcset ja_jp "Scilab execution error"\
                      "Scilab実行エラー"
::msgcat::mcset ja_jp "The shell reported an error while trying to execute "\
                      "実行するよう試みている最中に、SHELLは間違いを報告しました"
::msgcat::mcset ja_jp ": error " ": エラー "
::msgcat::mcset ja_jp "at line " "行で "
::msgcat::mcset ja_jp " of " " の "
::msgcat::mcset ja_jp "Execution aborted!" "実行の中断!"

# scipad.tcl
::msgcat::mcset ja_jp "NOT CONNECTED TO SCILAB" "SCILAB に接続なし"

# whichfun.tcl
::msgcat::mcset ja_jp "Being at line" "行中"
::msgcat::mcset ja_jp ", function" ", 関数"
::msgcat::mcset ja_jp "begins at" "開始時"
::msgcat::mcset ja_jp ", and there are" ", また、それは"
::msgcat::mcset ja_jp "continued lines, i.e. we are at line"\
                      "継続した行です, 即ち、行にあります"
::msgcat::mcset ja_jp "of" "の"
::msgcat::mcset ja_jp "Function" "関数"
::msgcat::mcset ja_jp "line" "行"
::msgcat::mcset ja_jp "The cursor is not currently inside a function body."\
                      "カーソルは、現在、機能として持っていません。"
::msgcat::mcset ja_jp "Which function?" "機能は作用します?"

#overriding tcl-core dialogs 
::msgcat::mcset ja_jp "&Directory:" "ディレクトリ(&D)"
::msgcat::mcset ja_jp "File &name:" "ファイル名(&n):"
::msgcat::mcset ja_jp "Files of &type:" "ファイル型(&t):"
::msgcat::mcset fr_fr "&Save" "保存(&S)"
::msgcat::mcset ja_jp "&Open" "開く(&O)"
::msgcat::mcset ja_jp "Open" "開く"
::msgcat::mcset ja_jp "Save As" "名前を付けて保存"
::msgcat::mcset ja_jp "&Cancel" "キャンセル(&C)"
::msgcat::mcset ja_jp "&Selection:" "選択(&S):"
::msgcat::mcset ja_jp "&Yes" "はい(&Y)"
::msgcat::mcset ja_jp "&No" "いいえ(&N)"
::msgcat::mcset ja_jp "&Red" "赤(&R)"
::msgcat::mcset ja_jp "&Green" "緑(&G)"
::msgcat::mcset ja_jp "&Blue" "青(&B)"
::msgcat::mcset ja_jp "Directory \"%1\$s\" does not exist."\
                      "ディレクトリ \"%1\$s\" が見つかりません。"
