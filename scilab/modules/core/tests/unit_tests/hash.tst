// ============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2021 - Stéphane MOTTELET
//
//  This file is distributed under the same license as the Scilab package.
// ============================================================================

// <-- CLI SHELL MODE -->
// <-- NO CHECK REF -->

// ============================================================================
// Unitary tests for gethash function
// ============================================================================


digest = ['crc32','md5','sha1','sha256','sha3-224','sha3-256','sha3-384','sha3-512'];

//string input

string_hash = [
    "8323a0b1"
    "4924d6315f6f9a498282b9c9330fc820"
    "1d23d6925090f8cef74824afe32d360a6ead6cac"
    "c2c4e5c0e42527cfc92b972f0422b2c2303992b5903306aed5aeff986b7ba826"
    "3bf781999b6d6cd319236ab77e2394d57ed056badf4ad7929ed89e29"
    "bee2e86e041728142695b1f0c1803d4dea25f67dc3626109014912316c7317a4"
    "4357510cb6c88f3f4662a296506d42429f907efca02a15c6728d46dc8e19366d08dfba171c2ea4a9be7ac7830c01c367"
    "6cf86d9a623598a79c240691bb3f9457828b5ba8bd5f7df1752008cf84b721bd2c24723e568292884ee661fc73ac85431671810b14af8d12bc1648e719520b1d"  ];

for i = 1:size(digest,"*")
    one_hash = hash("Zut ! Je crois que le chien Sambuca préfère le whisky revigorant au doux porto.", digest(i))
    assert_checkequal(one_hash, string_hash(i))
end

text_file_hash = [
    "8e32a629"
    "6f40c70a22495e62700df1bc43203423"
    "4c641cbdb7b3183afeb3ed4563c6d81a924b6184"
    "bc648dae436dc77915d9c3d38048cd3c871aafde135bf9f9bc073ee12c036627"
    "560d2928ad6b7e811fd4e80d05549cca51439f22a23496e198c6e018"
    "4814c7f5626b42d7202b1bf2b52af8162152e2f2a4e7f334fae7db6b17cb9d2a"
    "7b36e8d6e72467d17dd7f81c167f8902ca7b643bb6381d3e492e63745a6b53e15b313d7c7b7eb83c552da29f7bc84cb1"
    "7246dfd75501c49092ac1a4ba74e77c261ff71e62240e03ab0b27a69bf1b66676d34509ebd0f504ea24b81bb3da07bb3ce6a396888b82c4fe31faecc08cb8b90"
]

//text file input

mputl("Zut ! Je crois que le chien Sambuca préfère le whisky revigorant au doux porto.", fullfile(TMPDIR,"test.txt"))

for i = 1:size(digest,"*")
    disp(1)
    one_hash = hash(mopen(fullfile(TMPDIR,"test.txt"), "rt"), digest(i));
    assert_checkequal(one_hash, text_file_hash(i));
end

//binary file input

binary_file_hash = [
    "a2b126f3"
    "cd877ee3685812ffd8000ed10852569a"
    "807ce1c71f40d4e79ae81ce5250576e409278c0b"
    "2416f3ed3d430f50c11eea529f26b15e75d96435cee7abb415e6cb564f194020"
    "e817772022b6d507031440eb223e17946123a0516792ecc4f984efe8"
    "97862206f9a9c5760608583a80da205349814d09bf8fbaa03fc0d79c34fadd7f"
    "f8ae19169ed119e3464f8931fd7f454b9b570b7843ba134e5b0d5a5a7f28c3399c9af1c5009d894bcac5cbe65d82eb09"
    "99ea84e99247a9d601c5df3bb7569c710421fa1d40a706b343867aa365b7ec5d2f7130f0677b143204ed50fe8af39b54a47befa173f7330f821c315ad7d832ce"]

a = [
    1.0062539  -2.3196406  -1.0655583  -0.8323352  -0.311238
   -1.6001632  -1.1988964   0.8024138  -0.8414186  -0.3727657
    1.2037786   0.453251   -1.1135879  -0.808766    0.6113321
    1.715435    1.2332608  -0.5096602  -1.8584267  -0.692309
   -0.8262646   0.2984183  -0.7418754  -0.5721659   0.522145
]

fd = mopen(fullfile(TMPDIR,"test.bin"), 'wb');
mput(a, 'd', fd);
mclose(fd);

for i = 1:size(digest,"*")
    one_hash = hash(mopen(fullfile(TMPDIR,"test.bin")),digest(i));
    assert_checkequal(one_hash,binary_file_hash(i));
end
