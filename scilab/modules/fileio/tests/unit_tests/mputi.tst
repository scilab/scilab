// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2013 - Scilab Enterprises - Antoine ELIAS
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- CLI SHELL MODE -->

function writedata(name, data)
    f = mopen(TMPDIR + "/" + name + ".dat", "wb");
    for i=1:size(data)
        mput(data(i), name)
    end
    mclose(f);
endfunction

function res = readdata(name, len)
    f = mopen(TMPDIR + "/" + name + ".dat", "rb");
    res = mget(len, "uc", f);
    mclose(f);
endfunction

u8 = uint8(123); //0x7B
u16 = uint16(45678);//0xB26E
u32 = uint32(123456);//0x0001E240
s8 = int8(-123); //0x85
s16 = int16(-23456);//0xA460
s32 = int32(-123456);//0xFFFE1DC0

ref_l   = hex2dec(["85" "FF" "FF" "FF" "60" "A4" "FF" "FF" "C0" "1D" "FE" "FF"]);
ref_ll  = hex2dec(["85" "FF" "FF" "FF" "60" "A4" "FF" "FF" "C0" "1D" "FE" "FF"]);
ref_s   = hex2dec(["85" "FF" "60" "A4" "C0" "1D"]);
ref_sl  = hex2dec(["85" "FF" "60" "A4" "C0" "1D"]);
ref_ul  = hex2dec(["7B" "00" "00" "00" "6E" "B2" "00" "00" "40" "E2" "01" "00"]);
ref_us  = hex2dec(["7B" "00" "6E" "B2" "40" "E2"]);
ref_lb  = hex2dec(["FF" "FF" "FF" "85" "FF" "FF" "A4" "60" "FF" "FE" "1D" "C0"]);
ref_sb  = hex2dec(["FF" "85" "A4" "60" "1D" "C0"]);
ref_ulb = hex2dec(["00" "00" "00" "7B" "00" "00" "B2" "6E" "00" "01" "E2" "40"]);
ref_usb = hex2dec(["00" "7B" "B2" "6E" "E2" "40"]);
ref_ull = hex2dec(["7B" "00" "00" "00" "6E" "B2" "00" "00" "40" "E2" "01" "00"]);
ref_usl = hex2dec(["7B" "00" "6E" "B2" "40" "E2"]);

//integer in long ( little endian )
writedata("l", list(s8, s16, s32));
assert_checkequal(readdata("l", 12), ref_l);

//integer in little endian long
writedata("ll", list(s8, s16, s32));
assert_checkequal(readdata("ll", 12), ref_ll);

//integer in short
writedata("s", list(s8, s16, s32));
assert_checkequal(readdata("s", 6), ref_s);

//integer in little endian short
writedata("sl", list(s8, s16, s32));
assert_checkequal(readdata("sl", 6), ref_sl);

//integer in unsigned long
writedata("ul", list(u8, u16, u32));
assert_checkequal(readdata("ul", 12), ref_ul);

//integer in unsigned short
writedata("us", list(u8, u16, u32));
assert_checkequal(readdata("us", 6), ref_us);

//integer in big endian long
writedata("lb", list(s8, s16, s32));
assert_checkequal(readdata("lb", 12), ref_lb);

//integer in big endian short
writedata("sb", list(s8, s16, s32));
assert_checkequal(readdata("sb", 6), ref_sb);

//unsigned integer in unsigned big endian long
writedata("ulb", list(u8, u16, u32));
assert_checkequal(readdata("ulb", 12), ref_ulb);

//unsigned integer in unsigned big endian short
writedata("usb", list(u8, u16, u32));
assert_checkequal(readdata("usb", 6), ref_usb);

//unsigned integer in unsigned little endian long
writedata("ull", list(u8, u16, u32));
assert_checkequal(readdata("ull", 12), ref_ull);

//unsigned integer in unsigned little endian short
writedata("usl", list(u8, u16, u32));
assert_checkequal(readdata("usl", 6), ref_usl);
