// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2020 - Samuel GOUGEON
//
// This file is hereby licensed under the terms of the GNU GPL v2.0,
// pursuant to article 5.3.4 of the CeCILL v.2.1.
// This file was originally licensed under the terms of the CeCILL v2.1,
// and continues to be available under such terms.
// For more information, see the COPYING file which you should have received
// along with this program.

function %chars = percentchars()
    // Internal function called by scilab.start
    // http://bugzilla.scilab.org/9221

    %chars.greek.lower       = "αβδεϵζηθικλμνξοπρστυφϕχψωάϐέήϑίϊϰόϱςύϋΰϖώ";
    %chars.greek.upper       = "ΑΒΓΔΕΖΗΘϴΙΪΚΛΜΝΞΟΠΡΣΤΥΫΦΧΨΩ KΩ℧";
    %chars.subsupscript      = "₀₁₂₃₄₅₆₇₈₉₊₋₌₍ₐₕₖₗₘₙₒₚₛₜₓ₎ ⁰¹²³⁴⁵⁶⁷⁸⁹⁺⁻⁼⁽ªⁱⁿº⁾";
    %chars.maths.frac        = "½ ⅓ ¼ ⅕ ⅙ ⅛ ⅔ ⅖ ¾ ⅗ ⅜ ⅘ ⅚ ⅝ ⅞";
    %chars.maths.sets        = "ℂⅅℍℕℙℚℝℤ∉∈∊∍∋∌∅∖⋂∩⋃∪⊂⊆⊄⊅⊇⊃⋐⋑⋒⋓";
    %chars.maths.operators   = "−±∓∔÷×∏∐∑√∛∜⋅⋆⊕⊖⊝⊗⊘⊙⊚⊛⊜⊞⊟⊠⊡";
    %chars.maths.comparisons = "⋘≪≤≲≳≥≫⋙≠≃≄≅≈∽∝≍≐≔≕≝";
    %chars.maths.integdiff   = "∂∇∆∫∬∭∮∯∰∱⨑∲";
    %chars.maths.geometry    = "°′″∟⊾∠∡∢⊥∥∦∺∻≎⊿⋕";
    %chars.maths.logical     = "∀∃∄∧∨⋀⋁⊼⊽⊻∁∴∵∎≍";
    %chars.maths.misc        = "⋮⋰⋯⋱ ′ ″ ∞ ‰ ‱";
    %chars.physics           = "Å ℃ ℉ ħ";
    %chars.arrows.base       = "←↖↑↗→↘↓↙↔↕↵↱↴☇⇐⇑⇒⇓⇔↺↻⊣⊥⊢⊤";
    %chars.arrows.thick      = "⬅⬆➞⬇➚➙➘➔➜➡➥➦➧➨➼➽";
    %chars.symbols           = "…⋈⋔⋄♢◊♤♧♡♦♠♣♥©®℗Ⓢ™℡♀♂⌘✠♻☎«»◬⨺⨹⨻¶";
    %chars.stars             = "⋆★☆✫✯✰✭✡⊛⍟✪❂✻✼✹✸✶";
    %chars.currencies        = "¤ $ ¢ € £ ¥ 円 元 ₪ ฿ ¢ ₫ ₣";

    %chars.lang.latin = "abcdefghijklmnopqrstuvwxyz";
    %chars.lang.LATIN = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";
    %chars.lang.latin.voydiac = "áàâãäāăåéêèëēěėíîïóòôõöőōúùûŭũūůüűýŷÿ";
    %chars.lang.LATIN.voydiac = "ÁÀÂÃÄĀĂÅÉÊÈËĒĚĖÍÎÏÓÒÔÕÖŐŌÚÙÛŬŨŪŮÜŰÝŶŸ";
    %chars.lang.latin.condiac = "ćĉčçċďđǵĜ ğġĥħĵķĺļłńňņŕřŗśŝšşťţŵźžż";
    %chars.lang.LATIN.condiac = "ĆĈČĊÇĎĐ ĝĢĞĠĤĦĴĶĹĻŁŃŇŅŔŘŖŚŜŠŞŤŢŴŹŽŻ";
    %chars.lang.latin.special = "æ Æ œ Œ ℬ ъ Ъ Þ þ ⅆ ℇ ℈ ℰ ƒ ⨍ ℊ ℱ ℏ ℎ ℋ ℌ ℐ ℑ ℳ № ℘ ℛ ℜ";
    %chars.lang.russian = "абвгдеёжзийклмнопрстуфхцчшщъыьэюя";
    %chars.lang.RUSSIAN = "АБВГДЕЁЖЗИЙКЛМНОПРСТУФХЦЧШЩЪЫЬЭЮЯ";
    %chars.lang.japanese.hiragana.a = "あかさたなはまやらわ がざだばぱ";
    %chars.lang.japanese.hiragana.e = "えけせてねへめ れゑ げぜでべぺ";
    %chars.lang.japanese.hiragana.i = "いきしちにひみ りゐ ぎじぢびぴ";
    %chars.lang.japanese.hiragana.o = "おこそとのほもよろをんごぞどぽ";
    %chars.lang.japanese.hiragana.u = "うくすつぬふむゆる  ぐずづぶぷ";
    %chars.lang.arabic.alone1 = "ش س ز ر ذ د خ ح ج ث ت ب ا ء";
    %chars.lang.arabic.alone2 = "ي و ه ن م ل ك ق ف غ ع ظ ط ض ص";
endfunction
