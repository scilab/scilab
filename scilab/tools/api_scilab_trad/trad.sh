#!/bin/sh

if test $# -eq 0; then
    echo "This script converts a stackX gateway to api_scilab.h"
    echo "This is just a helper, not a magic tool"
    echo "It expects at least one argument"
    echo ""
    echo "Syntax : $0 <path_to_a_file>"
    exit 1
fi

FILES=$1

if test ! -f "$FILES"; then
    echo "Cannot find $FILES"
    exit 2
fi
ASTYLE_CMD="astyle --pad-header -n --pad-oper --indent-col1-comments --indent-switches --style=linux --indent=spaces=4 -A1 -q"
echo "Converting $FILES"

$ASTYLE_CMD $FILES
sed -i -e "s|CheckRhs(\(.*\), \(.*\))|CheckInputArgument(pvApiCtx, \1, \2)|g" $FILES

sed -i -e "s|CheckLhs(\(.*\), \(.*\))|CheckOutputArgument(pvApiCtx, \1, \2)|g" $FILES
sed -i -e "s|PutLhsVar()|ReturnArguments(pvApiCtx)|g" $FILES


###########################################

grep '#include "api_scilab.h"' $FILES
if test $? -ne 0; then # Could not file api_scilab
    # add it
    sed -i '0,/#include \"\(.*\)/s//#include \"api_scilab.h\"\n#include \"\1/' $FILES
fi

###########################################

grep 'SciErr sciErr' $FILES
if test $? -ne 0; then # Could not file SciErr
    # add it
    sed  -i "s/CheckInputArgument\(.*\)/SciErr sciErr;\CheckInputArgument\1/" $FILES

fi

#IN="GetRhsVar(2,MATRIX_OF_DOUBLE_DATATYPE,&m2,&n2,&l2);"

getProfileDouble() {
    POSITION="$1"
    NBROW="$2"
    NBCOL="$3"
    VAR="$4"
    OUT="   //get variable address of the input argument\n    sciErr = getVarAddressFromPosition(pvApiCtx, $POSITION, \&piAddr$POSITION);\n    if (sciErr.iErr)\n    {\n        printError(\&sciErr, 0);\n        return 1;\n    }//MYMARK$POSITION\n    sciErr = getMatrixOfDouble(pvApiCtx, piAddr$POSITION, \&$NBROW, \&$NBCOL, \&$VAR);\n    if (sciErr.iErr)\n    {\n        printError(\&sciErr, 0);\n        return 1;\n    }"

}

###########################################
#  CreateVarFromPtr => createMatrixOfString
createProfileString() {
    POSITION="$1"
    NBROW="$2"
    NBCOL="$3"
    VAR="$4"
    OUT="/* Create the string matrix as return of the function */\nsciErr = createMatrixOfString(pvApiCtx, nbInputArgument(pvApiCtx) + $POSITION, $NBROW, $NBCOL, $VAR);\nfree($VAR); // Data have been copied into Scilab memory\nif (sciErr.iErr)\n{\n    free($VAR); // Make sure everything is cleanup in case of error\n    printError(\&sciErr, 0);\n    return 1;}"
}

###########################################
#  CreateVarFromPtr => createMatrixOfString
createProfileSimpleString() {
    POSITION="$1"
    NBROW="$2"
    NBCOL="$3"
    VAR="$4"
    OUT="/* Create the string matrix as return of the function */\niRet = createSingleString(pvApiCtx, nbInputArgument(pvApiCtx) + $POSITION, $VAR);\nfree($VAR); // Data have been copied into Scilab memory\nif (iRet)\n{\n    freeAllocatedSingleString($VAR);\n    return 1;}"
}

#  CreateVarFromPtr => createMatrixOfDouble
createProfileDouble() {
    POSITION="$1"
    NBROW="$2"
    NBCOL="$3"
    VAR="$4"
    OUT="/* Create the matrix as return of the function */\nsciErr = createMatrixOfDouble(pvApiCtx, nbInputArgument(pvApiCtx) + $POSITION, $NBROW, $NBCOL, $VAR);\nfree($VAR); // Data have been copied into Scilab memory\nif (sciErr.iErr)\n{\n    free($VAR); // Make sure everything is cleanup in case of error\n    printError(\&sciErr, 0);\n    return 1;}"
}

##########################################

#  CreateVar => allocMatrixOfDouble
createProfileDoubleAlloc() {
    POSITION="$1"
    NBROW="$2"
    NBCOL="$3"
    VAR="$4"
    OUT="/* Create the matrix as return of the function */\nsciErr = allocMatrixOfDouble(pvApiCtx, nbInputArgument(pvApiCtx) + $POSITION, $NBROW, $NBCOL, $VAR); if (sciErr.iErr)\n{\n    printError(\&sciErr, 0);\n    return 1;}"
}

##########################################

getProfileString() {
    POSITION="$1"
    NBROW="$2"
    NBCOL="$3"
    VAR="$4"
    SOURCE="tools/api_scilab_trad/stringtemplate.txt"
    sed -e "s|@POSITION@|$POSITION|g" -e "s|@MYMARK@|MYMARK$POSITION|g" -e "s|@NBROW@|$NBROW|g"  -e "s|@NBCOL@|$NBCOL|g" -e "s|@VAR@|$VAR|g" $SOURCE > foo.txt
    sed -i -n 'H;${g;s/\n/\\n/g;p}' foo.txt
    OUT=$(cat foo.txt)
}

##########################################
getProfileSingleString() {
    POSITION="$1"
    NBROW="$2"
    NBCOL="$3"
    VAR="$4"
    FILE="$5"
    SOURCE="tools/api_scilab_trad/singlestringtemplate.txt"
    echo     sed -e "s|@POSITION@|$POSITION|g" -e "s|@MYMARK@|MYMARK$POSITION|g" -e "s|@NBROW@|$NBROW|g"  -e "s|@NBCOL@|$NBCOL|g" -e "s|@VAR@|$VAR|g" $SOURCE
    sed -e "s|@POSITION@|$POSITION|g" -e "s|@MYMARK@|MYMARK$POSITION|g" -e "s|@NBROW@|$NBROW|g"  -e "s|@NBCOL@|$NBCOL|g" -e "s|@VAR@|$VAR|g" $SOURCE > foo.txt
    sed -i -n 'H;${g;s/\n/\\n/g;p}' foo.txt
    OUT=$(cat foo.txt)
    sed -i -e "s|cstk($VAR)|"$VAR"char|g" $5
}

##########################################

INT="GetRhsVar"
while test "$(grep $INT $FILES|grep MATRIX_OF_DOUBLE_DATATYPE)" != ""; do
    RHS=$(grep $INT $FILES|grep MATRIX_OF_DOUBLE_DATATYPE|head -1)
    if test "$RHS" != ""; then
# Split the values
        ARGS=$(echo $RHS|sed -e "s|.*(\(.*\), .*, &\(.*\), &\(.*\), &\(.*\));|\1 \2 \3 \4|")
        getProfileDouble $ARGS
        sed -i -e "s|$RHS|$OUT|g" $FILES
    fi
done

##########################################
#  CreateVarFromPt => createMatrixOfDouble

CREATEINT="CreateVarFromPtr"
while test "$(grep $CREATEINT $FILES|grep MATRIX_OF_DOUBLE_DATATYPE)" != ""; do
    LHS=$(grep $CREATEINT $FILES|grep MATRIX_OF_DOUBLE_DATATYPE|head -1)
    if test "$LHS" != ""; then
# Split the values
        ARGS=$(echo $LHS|sed -e "s|.*(.* + \(.*\), .*, &\(.*\), &\(.*\), &\(.*\));|\1 \2 \3 \4|")
        createProfileDouble $ARGS
        sed -i -e "s|$LHS|$OUT|g" $FILES
    fi
done

##########################################
#  CreateVarFromPtr => createMatrixOfString

CREATEINT="CreateVarFromPtr"
while test "$(grep $CREATEINT $FILES|grep MATRIX_OF_STRING_DATATYPE)" != ""; do
    LHS=$(grep $CREATEINT $FILES|grep MATRIX_OF_STRING_DATATYPE|head -1)
    if test "$LHS" != ""; then
# Split the values
        ARGS=$(echo $LHS|sed -e "s|.*(.* + \(.*\), .*, &\(.*\), &\(.*\), \(.*\));|\1 \2 \3 \4|")
        createProfileString $ARGS
        sed -i -e "s|$LHS|$OUT|g" $FILES
    fi
done

##########################################
#  CreateVarFromPtr => createMatrixOfString

CREATEINT="CreateVarFromPtr"
while test "$(grep $CREATEINT $FILES|grep ' STRING_DATATYPE')" != ""; do
    LHS=$(grep $CREATEINT $FILES|grep ' STRING_DATATYPE'|head -1)
    if test "$LHS" != ""; then
# Split the values
        ARGS=$(echo $LHS|sed -e "s|.*(.* + \(.*\), .*, &\(.*\), &\(.*\), &\(.*\));|\1 \2 \3 \4|")
#        echo "$ARGS"
        createProfileSimpleString $ARGS
        sed -i -e "s|$LHS|$OUT|g" $FILES
    fi
done

##########################################
#  CreateVar => allocMatrixOfDouble
CREATEINT="CreateVar"
while test "$(grep $CREATEINT $FILES|grep MATRIX_OF_DOUBLE_DATATYPE)" != ""; do
    LHS=$(grep $CREATEINT $FILES|grep MATRIX_OF_DOUBLE_DATATYPE|head -1)
    if test "$LHS" != ""; then
# Split the values
        ARGS=$(echo $LHS|sed -e "s|.*(.* + \(.*\), .*, &\(.*\), &\(.*\), &\(.*\));|\1 \2 \3 \4|")
        createProfileDoubleAlloc $ARGS
        sed -i -e "s|$LHS|$OUT|g" $FILES
    fi
done

##########################################
# Matrix of string #

CREATEINT="GetRhsVar"
while test "$(grep $CREATEINT $FILES|grep MATRIX_OF_STRING_DATATYPE)" != ""; do
    LHS="$(grep $CREATEINT $FILES|grep MATRIX_OF_STRING_DATATYPE|head -1)"

    if test "$LHS" != ""; then

# Split the values
        ARGS=$(echo $LHS|sed -e "s|.*(\(.*\), .*, &\(.*\), &\(.*\), &\(.*\));|\1 \2 \3 \4|")
        getProfileString $ARGS
        sed -i -e "s|$LHS|$OUT|g" $FILES
    fi
done


##########################################
# Single string #

CREATEINT="GetRhsVar"
while test "$(grep $CREATEINT $FILES|grep STRING_DATATYPE)" != ""; do
    LHS="$(grep $CREATEINT $FILES|grep STRING_DATATYPE|head -1)"

    if test "$LHS" != ""; then
# Split the values
        ARGS=$(echo $LHS|sed -e "s|.*(\(.*\), .*, &\(.*\), &\(.*\), &\(.*\));|\1 \2 \3 \4|")
        getProfileSingleString $ARGS $FILES
        sed -i -e "s|$LHS|$OUT|g" $FILES
    fi
done

##########################################

sed -i -e "s|LhsVar(\(.*\))|AssignOutputVariable(pvApiCtx, \1)|g" $FILES
sed -i -e "s| Rhs | nbInputArgument(pvApiCtx) |g" $FILES
sed -i -e "s|(Rhs |(nbInputArgument(pvApiCtx) |g" $FILES
sed -i -e "s| Lhs | nbOutputArgument(pvApiCtx) |g" $FILES
sed -i -e "s|(Lhs |(nbOutputArgument(pvApiCtx) |g" $FILES
sed -i -e "s|(VarType(\(.*\)).*==.*sci_matrix)|(isDoubleType(pvApiCtx,TODO_ADDRESS_OF_\1))|g" $FILES
sed -i -e "s|(GetType(\(.*\)).*==.*sci_matrix)|(isDoubleType(pvApiCtx,TODO_ADDRESS_OF_\1))|g" $FILES
sed -i  -e "s|(VarType(\(.*\)).*==.*sci_strings)|(isStringType(pvApiCtx,TODO_ADDRESS_OF_\1))|g" $FILES
sed -i  -e "s|(GetType(\(.*\)).*==.*sci_strings)|(isStringType(pvApiCtx,TODO_ADDRESS_OF_\1))|g" $FILES


##########################################

# if we have isStringType ... getVarAddressFromPosition. We need to switch them
# since the address is not known yet
$ASTYLE_CMD $FILES &> /dev/null
for (( i=1; i <= 10; i++ ));do
    ORIG="if \(is([a-zA-z]*)Type\(pvApiCtx, TODO_ADDRESS_OF_$i\)\).*{(.*)sciErr = getVarAddressFromPosition\(pvApiCtx, $i, (.*)\);(.*)MYMARK$i"

    perl -0777 -pe "s/$ORIG/\2sciErr = getVarAddressFromPosition\(pvApiCtx, $i, \3\);\4\nif \(is\1Type\(pvApiCtx, piAddr$i\)) {/sg"  $FILES > $FILES.TMP
    mv $FILES.TMP $FILES
done

# if we have VarType(X) != YYY ... getVarAddressFromPosition.
# We need to switch them since the address is not known yet
$ASTYLE_CMD $FILES &> /dev/null
for (( i=1; i <= 10; i++ ));do
    ORIG="if \(VarType\($i\) != sci_strings\).*{(.*)sciErr = getVarAddressFromPosition\(pvApiCtx, $i, (.*)\);(.*)MYMARK$i"
    perl -0777 -pe "s/$ORIG/sciErr = getVarAddressFromPosition\(pvApiCtx, $i, \2\);\3\nif \(!isStringType\(pvApiCtx, piAddr$i\)) {\1/sg"  $FILES > $FILES.TMP
    mv $FILES.TMP $FILES
done




##########################################


$ASTYLE_CMD $FILES &> /dev/null
