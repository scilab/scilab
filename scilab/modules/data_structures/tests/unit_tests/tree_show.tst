// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2019 - Samuel GOUGEON
//
// This file is hereby licensed under the terms of the GNU GPL v2.0,
// pursuant to article 5.3.4 of the CeCILL v.2.1.
// This file was originally licensed under the terms of the CeCILL v2.1,
// and continues to be available under such terms.
// For more information, see the COPYING file which you should have received
// along with this program.
// =============================================================================

// <-- INTERACTIVE TEST -->

// unitary tests of tree_show()
// =============================================================================

// INPUT STRUCTURE
// ===============
clear f
films(1) = struct("Title", "A.I. Artificial Intelligence",..
                  "Year", 2001, ..
                  "Director", "Steven Spielberg", ..
                  "Duration", 140);
films(2) = struct("Title", "I, robot", ..
                  "Year", 2004, ..
                  "Director", "Alex Proyas", ..
                  "Duration", 120);
films(1,2) = struct("Title", "Gravity", ..
                    "Year", 2013, ..
                    "Director", "Alfonso Cuarón", ..
                    "Duration", 100);
films(2,2) = struct("Title", "2001: A space odyssey", ..
                    "Year", 1968, ..
                    "Director", "Stanley Kubrick", ..
                    "Duration", 141);
// 1)
tree_show(films(:), "films(:) (by indices)");
// Checking
// --------
// * films(:) is a vector => displayed indices are simple.
// * Root title = films(:) (by indices) [4x1 struct]
// * Main items : 1: ↴  2: ↴  3: ↴  4:↴   (in blue and folded)
//   1: Subitems: Title: "A.I. Artificial Intelligence" 
//                Year: 2001
//                Director: "Steven Spielberg"
//                Duration: 140
//                Title:, Year:, Director: and Duration: are in blue
//                Their values are in black
//   Same thing for 2:, 3:, and 4:.

// 2)
tree_show(films, "films (by indices)");
// Checking
// --------
// * films is a matrix => displayed indices are detailled: (i,j,..)
// * Root title = films (by indices) [2x2 struct]
// * Same as in test 1), except that 1: ↴  is now (1,1): ↴
//                                   2: ↴  is now (2,1): ↴
//                                   3: ↴  is now (1,2): ↴
//                                   4: ↴  is now (2,2): ↴
// ArrayByFields option
// --------------------
// 3)
tree_show(films(:), "films(:) (by fields)",, %t);
// Checking
// --------
// films(:) is a vector => displayed indices are simple.
// Root title = films(:) (by fields) [4x1 struct]
// Main items = Title:, Year:, Director: and Duration:  (in blue and folded)
//   Unfold "Title": Subitems:
//           1: "A.I. Artificial Intelligence" 
//           2: "I, robot"
//           3: "Gravity"
//           4: "2001: A space odyssey"
//       1: 2: 3: and 4: are in blue. Titles are in black.
//   Same thing for Year:, Director: and Duration:


// 4)
tree_show(films, "films (by fields)",, %t);
// Checking
// --------
// Same as 3), except:
//  * films is a matrix => displayed indices are detailled: (i,j,..)
//  * Root title = films (by fields) [2x2 struct]


// 5) EMPTY | VOID COMPONENTS (& list)
// ===================================
L = list([], ..
         list(), ..
         list(,%pi), ..     // first element is void
         struct(), ..
         sparse([],[]), ..
         {}, ..
         mlist(""), ..
         tlist(""));
tree_show(L, "Empty or void components");
// Checking
// --------
// The following tree must be displayed (all indices in blue, contents in black):
//    Empty or void components [list]
//       1: []
//       2: list()
//       3: [list]
//          1:
//          2: 3.1415927
//       4: struct()
//       5: sparse([],[],[0,0])
//       6: { }
//       7: mlist("")
//       8: tlist("")


// 6) STYLING THE MAIN TITLE WITH HTML
// ===================================
rootTitle = "<html><font color=""red"" size=""4""><b>Films</b> (Science fiction)</font>";
tree_show(films, rootTitle, ,%t)
// Checking
// --------
// In the main title:
//  "Films"             must be in bold red and slightly bigger
//  "(Science fiction)" must be in red normal size
//  "[2x2 struct]"      must be in black normal size


// 7) STYLING ADDRESSES OR/AND CONTENTS
// ====================================
// 7.a) Removing default addresses style => in black
tree_show(films, , "", %t)
// Check that fields names and indices are in black

// 7.b) Setting fields names and indices in small bold
tree_show(films, , "<font size=""2""><b>$</b></font>", %t)
// Check that fields names and indices are in small bold black

// 7.c) Setting contents in italic
tree_show(films, , "$<i>", %t)
// Check that fields names and indices are in normal black,
//  and contents in italic black

// 7.d) Setting addresses in small red, and contents in italic
tree_show(films, , "<font color=""red""><small>$</small></font><i>", %t)
// Check that fields names and indices are in small red,
//  and contents is in italic black


// 8) INPUT CELLS ARRAY
// ====================
c = {
    [%t %f %f],                             list(,)(1),     ["This" "is" "some" "text"]
     int8([1 2 3 ; 4 5 6]),                 sin,    list(%pi,"abcd",,(%i-%s)^3)
     [-%inf, -1, 0, %i, 7.1, 3.3e20, %inf, %nan], logspace, {}
     sparse([0 1 2; 3 0 4]),                iolib,          films(1,:)
    };
// Expected display (with blue indices and fields names):
//
// [4x3 cell]
//    (1,1) : [T F F]
//    (2,1) : int8([1, 2, 3 ; 4, 5, 6])
//    (3,1) : [-Inf, -1, 0, i, 7.1, 3.300D+20, Inf, Nan]
//    (4,1) : sparse([1, 2 ; 1, 3 ; 2, 1 ; 2, 3], [1 ; 2 ; 3 ; 4], [2, 3])
//    (1,2) : $:-1:3
//    (2,2) : fptr
//    (3,2) : logspace() (42 lines)
//    (4,2) : iolib library of 6 functions @ SCI\modules\io\macros\
//    (1,3) : ["This", "is", "some", "text"]
//    (2,3) :  [list]
//        1: 3.1415927
//        2: "abcd"
//        3: 
//        4: -i+3s+(i3)s^2-s^3
//    (3,3) :  { }
//    (4,3) :  [struct]
//        Title: "A.I. Artificial Intelligence"
//        Year: 2001
//        Director: "Steven Spielberg"
//        Duration: 140

