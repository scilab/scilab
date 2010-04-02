// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2008 - DIGITEO - Allan CORNET
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- Non-regression test for bug 6788 -->
// <-- INTERACTIVE TEST -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/show_bug.cgi?id=6788
//
// <-- Short Description -->
// toprint(filename): UTF8 chars in the file were mis-printed.

// To save some paper, this test is interactive


// zh_TW / Cyrillic / Japanese / Chinese / Thai / Corean
text=[ "中文測試", "азеазеясдвклгхклмвцмзер" ..
"Un Texte accentué pour vérifier qu'ils sont bien supportés",..
 "すべての人間は、生まれながらにして自由であり、かつ、尊厳と権利と について平等である。人間は、理性と良心とを授けられており、互いに同 胞の精神をもって行動しなければならない。" ..
"人 人 生 而 自 由, 在 尊 严 和 权 利 上 一 律 平 等。 他 们 赋 有 理 性 和 良 心, 并 应 以 兄 弟 关 系 的 精 神 相 对 待。" ..
"ราทุกคนเกิดมาอย่างอิสระ เราทุกคนมีความคิดและความเข้าใจเป็นของเราเอง เราทุกคนควรได้รับการปฏิบัติในทางเดียวกัน." ..
"모든 인간은 태어날 때부터 자유로우며 그 존엄과 권리에 있어 동등하다. 인간은 천부적으로 이성과 양심을 부여받았으며 서로 형제애의 정신으로 행동하여야 한다." ..
"لد جميع الناس أحرارًا متساوين في الكرامة والحقوق. وقد وهبوا عقلاً وضميرًا وعليهم أن يعامل بعضهم بعضًا بروح الإخاء"..
"כל בני האדם נולדו בני חורין ושווים בערכם ובזכיותיהם. כולם חוננו בתבונה ובמצפון, לפיכך חובה עליהם לנהג איש ברעהו ברוח של אחוה"
]; 

if toprint(SCI+'/modules/gui/tests/nonreg_tests/bug_6788.tst') <> %T then pause,end
