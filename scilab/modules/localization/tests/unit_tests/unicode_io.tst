// ============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2008 - Yung-Jang Lee <yjlee123@gmail.com>
// Copyright (C) 2008 - INRIA - Sylvestre LEDRU <sylvestre.ledru@inria.fr>
//
//  This file is distributed under the same license as the Scilab package.
// ============================================================================

lines(0);

// zh_TW / Cyrillic / amharique / Japanese / Chinese / Thai / Corean
text=[ "中文測試", "азеазеясдвклгхклмвцмзер" ..
 "ድቅስድቅስግጅክልውሽችክልዝ" ..
"すべての人間は、生まれながらにして自由であり、かつ、尊厳と権利と について平等である。人間は、理性と良心とを授けられており、互いに同 胞の精神をもって行動しなければならない。" ..
"人 人 生 而 自 由, 在 尊 严 和 权 利 上 一 律 平 等。 他 们 赋 有 理 性 和 良 心, 并 应 以 兄 弟 关 系 的 精 神 相 对 待。" ..
"ราทุกคนเกิดมาอย่างอิสระ เราทุกคนมีความคิดและความเข้าใจเป็นของเราเอง เราทุกคนควรได้รับการปฏิบัติในทางเดียวกัน." ..
"모든 인간은 태어날 때부터 자유로우며 그 존엄과 권리에 있어 동등하다. 인간은 천부적으로 이성과 양심을 부여받았으며 서로 형제애의 정신으로 행동하여야 한다." ..
"لد جميع الناس أحرارًا متساوين في الكرامة والحقوق. وقد وهبوا عقلاً وضميرًا وعليهم أن يعامل بعضهم بعضًا بروح الإخاء"..
"כל בני האדם נולדו בני חורין ושווים בערכם ובזכיותיהם. כולם חוננו בתבונה ובמצפון, לפיכך חובה עליהם לנהג איש ברעהו ברוח של אחוה"
]; 

for i=1:size(text,"*") 

if msprintf(text(i)) <> text(i) then pause, end

//  Fortran-Style output<------- no yet fixed in this patch
//u=file('open','resultsA','unknown') //<--------Test 2 file encoding for Fortran Style output
//write(u,text(i),'(a)') // encoding wrong, not in system locale
//file('close',u)
// C style output

myFile="resultsA";
u=mopen(TMPDIR+"/"+myFile,'w');
mfprintf(u,text(i)+" plus %s",text(i));
mclose(u); 

if mgetl(TMPDIR+"/"+myFile,1) <> text(i) + " plus "+text(i) then pause, end


myStr=" with another %s";
myStr2=" with another ";
if msprintf(text(i)+myStr,text(i)) <> text(i)+myStr2+text(i) then pause, end

// Classic display
disp(text(i));

end


//title('title test'+text); t=0:0.1:2*%pi; //<------Test 7  text drawing test
//plot2d(t,[sin(t'),cos(t')],[-1,2]); 
//legends(['legends test'+text ;'text B '+text],[-1,2], opt=3 );
// ScilabVersion/OS/LANG/CONSOLE = EncodingPatch/WindowsXp/zh_TW.CP950/Dos Console
//about();                         //<------Test 8  Scilab gettext Test


