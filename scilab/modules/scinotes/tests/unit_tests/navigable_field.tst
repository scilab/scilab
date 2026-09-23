// Scilab ( https://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2026 - Dassault Systèmes S.E. - Cédric DELAMARRE
//
// For more information, see the COPYING file which you should have received
// along with this program.
//
// <-- INTERACTIVE TEST -->

// Same field expression with and without a call.
//
// Execute the assignments, then check:
// 1. mycos has the macro style on all three lines.
// 2. Adding () must not turn mycos back into a field token.
// 3. Hover all occurrences: each must resolve acoshm documentation.
// 4. Ctrl+click all occurrences: each must open the acoshm source.

st.mycos = acoshm
st.mycos
st.mycos()
