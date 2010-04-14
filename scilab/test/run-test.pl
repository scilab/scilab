#!/usr/bin/perl
use Switch;
use File::Basename;
use File::Path;
use Cwd;

$IsWindows = 0;

my $all_good = 0;
my $all_cira = 0;
my $all_syntax = 0;
my $all_control = 0;
my $bad_good = 0;
my $bad_cira = 0;
my $bad_syntax = 0;
my $bad_control = 0;
my $progress = 0;
my $original_stdout = null;
my $original_stderr = null;

##-------------------
## OPTION Management
##-------------------
my $long_compare_old_scilab_option = "--compare-old-scilab";
my $short_compare_old_scilab_option = "-C";
my $long_display_tree_option = "--display-tree";
my $short_display_tree_option = "-D";
my $long_quiet_option = "--quiet";
my $short_quiet_option = "-Q";
my $long_help_option = "--help";
my $short_help_option = "-H";


my $quiet = 0;
my $displaytree = 0;
my $compareoldscilab = 0;

my $SCILAB = "scilab -nw";

open(OLDOUT,">&STDOUT");
open(OLDERR,">&STDERR");

# Current directory
my $directory = getcwd();

if( dirname($0) ne "." )
{
    $directory .= "/" . dirname($0);
}

if( dirname($0) =~ m/^\// )
{
    $directory = dirname($0);
}

# modules dir path
my $Yasp_Path = $directory;
$Yasp_Path =~ s/\/test/\//g;

my $Yasp_Bin = "bin/scilab";

if(@ARGV < 1)
{
    usage();
    exit(1);
}

###################
## Select option ##
###################
for($i = 1 ; $i < @ARGV ; $i++)
{
    switch($ARGV[$i])
    {
	case "$long_compare_old_scilab_option"
	{
	    $compareoldscilab = 1;
	}
	case "$short_compare_old_scilab_option"
	{
	    $compareoldscilab = 1;
	}
	case "$long_display_tree_option"
	{
	    $displaytree = 1;
	}
	case "$short_display_tree_option"
	{
	    $displaytree = 1;
	}
	case "$long_quiet_option"
	{
	    $quiet = 1;
	}
	case "$short_quiet_option"
	{
	    $quiet = 1;
	}
	case "$long_help_option"
	{
	    usage();
	    exit;
	}
	case "$short_help_option"
	{
	    usage();
	    exit;
	}
	default
	{
	    usage();
	    exit(1);
	}
    }
}

if($^O eq "MSWin32" && $quiet == 0)
{
    eval('use Win32::Console');
    $CONSOLE = new Win32::Console(Win32::Console::STD_OUTPUT_HANDLE());
    $IsWindows = 1;
}

if($quiet == 1)
{
    close(STDOUT);
    open(STDOUT, ">out.log");
    close(STDERR);
    open(STDERR, ">err.log");
}

###############################
## Select set of test to run ##
###############################
switch($ARGV[0])
{
    case "control"
    {
	control();
    }
    case "good"
    {
	good();
    }
    case "syntax"
    {
	syntax();
    }
    case "cira"
    {
	cira();
    }
    case "progress"
    {
	$progress = 1;
	good();
	if($bad_good == 0)
	{
	    cira();
	}
    }
    case "all"
    {
	good();
	syntax();
	cira();
	control();
    }
    case "dummy"
    {
	dummy();
	exit;
    }
    case "$long_help_option"
    {
	usage();
	exit;
    }
    case "$short_help_option"
    {
	usage();
	exit;
    }
    default
    {
	usage();
	exit(1);
    }
}

if($quiet == 1)
{
    close(STDOUT);
    open(STDOUT,">&OLDOUT");
    close(STDERR);
    open(STDERR,">&OLDERR");

    if($^O eq "MSWin32")
    {
	eval('use Win32::Console');
	$CONSOLE = new Win32::Console(Win32::Console::STD_OUTPUT_HANDLE());
	$IsWindows = 1;
    }
    $quiet = 0;
}

InfoMsg(" ---*****************---\n");
InfoMsg(" -*-                 -*-\n");
InfoMsg(" -*- RESULTS SUMMARY -*-\n");
InfoMsg(" -*-                 -*-\n");
InfoMsg(" ---*****************---\n");

if( $all_good != 0)
{
    $good_good = $all_good - $bad_good;
    StartMsg("Good tests\n");
    InfoMsg( $all_good . " test(s)\n");
    GoodMsg($good_good . " test(s) passed\n");
    BadMsg($bad_good . " test(s) failed\n");
    StartMsg(($good_good/$all_good)*100 . "% passed\n");
    print "\n";
}

if( $all_cira != 0)
{
    $good_cira = $all_cira - $bad_cira;
    StartMsg("Cira tests\n");
    InfoMsg( $all_cira . " test(s)\n");
    GoodMsg($good_cira . " test(s) passed\n");
    BadMsg($bad_cira . " test(s) failed\n");
    StartMsg(($good_cira/$all_cira)*100 . "% passed\n");
    print "\n";
}

if( $all_syntax != 0)
{
    $good_syntax = $all_syntax - $bad_syntax;
    StartMsg("Syntax tests\n");
    InfoMsg( $all_syntax . " test(s)\n");
    GoodMsg($good_syntax . " test(s) passed\n");
    BadMsg($bad_syntax . " test(s) failed\n");
    StartMsg(($good_syntax/$all_syntax)*100 . "% passed\n");
    print "\n";
}

if( $all_control != 0)
{
    $good_control = $all_control - $bad_control;
    StartMsg("Control tests\n");
    InfoMsg( $all_control . " test(s)\n");
    GoodMsg($good_control . " test(s) passed\n");
    BadMsg($bad_control . " test(s) failed\n");
    StartMsg(($good_control/$all_control)*100 . "% passed\n");
    print "\n";
}

#######################
## GOOD set of tests ##
#######################
sub good()
{
    StartMsg("[ Lets Go for A set of good tests ]\n");
    while (defined($nextname = <good/*.sci good/*.sce>))
    {
	$all_good = $all_good + 1;
	InfoMsg("\n[ ----==== Running Test $nextname ====---- ]\n");
	if ($compareoldscilab == 1)
	{
	    process_compare_old_scilab($nextname, $bad_good);
	}
	else
	{
	    if($displaytree == 1)
	    {
		process_display_tree($nextname, $bad_good);
	    }
	    else
	    {
		$RetVal = system($Yasp_Path . $Yasp_Bin . " -nw --no-exec -f " . $nextname);
		if($RetVal == 0)
		{
		    GoodMsg("Test Passed\n\n");
		}
		else
		{
		    $bad_good += 1;
		    BadMsg("Test Failed\n\n");
		    if($progress == 1)
		    {
			$RetVal = system($Yasp_Path . $Yasp_Bin . " -nw --no-exec --parse-trace -f " . $nextname);
			exit;
		    }
		}
	    }
	}
    }
    return 0;
}

##########################
## CONTROL set of tests ##
##########################
sub control()
{
    StartMsg("[ Lets Go for A set of control tests ]\n");
    while (defined($nextname = <control/*.sci control/*.sce>))
    {
	$all_control = $all_control + 1;
	InfoMsg("\n[ ----==== Running Test $nextname ====---- ]\n");
	if($displaytree == 1)
	{
	    process_display_tree($nextname, $bad_control);
	}
	else
	{
	    $RetVal = system($Yasp_Path . $Yasp_Bin . " -nw --no-exec -f " . $nextname);
	    if($RetVal == 0)
	    {
		GoodMsg("Test Passed\n\n");
	    }
	    else
	    {
		$bad_control += 1;
		BadMsg("Test Failed\n\n");
		if($progress == 1)
		{
		    $RetVal = system($Yasp_Path .  $Yasp_Bin . " -nw --no-exec --parse-trace -f " . $nextname);
		    exit;
		}
	    }
	}
    }
    return 0;
}

#######################
## CIRA set of tests ##
#######################
sub cira()
{
    $bad_cira = 0;
    StartMsg("[ Lets Go for A set of CIRA tests ]\n");
    while (defined($nextname = <CIRA/avant/*.sci CIRA/avant/*.sce CIRA/apres/*.sci CIRA/apres/*.sce>))
    {
	$all_cira = $all_cira + 1;
	InfoMsg("[ ----==== Running Test $nextname ====---- ]\n");

	if($displaytree == 1)
	{
	    process_display_tree($nextname, $bad_cira);
	}
	else
	{
	    $RetVal = system($Yasp_Path . $Yasp_Bin . " -nw --no-exec -f " . $nextname);
	    if($RetVal == 0)
	    {
		GoodMsg("Test Passed\n\n");
	    }
	    else
	    {
		$bad_cira += 1;
		BadMsg("Test Failed\n\n");
		if($progress == 1)
		{
		    $RetVal = system($Yasp_Path . $Yasp_Bin . " -nw --no-exec --parse-trace -f " . $nextname);
		}
	    }
	}
    }
    return 0;
}

#########################
## SYNTAX set of tests ##
#########################
sub syntax()
{
    StartMsg("[ Lets Go for A set of syntax tests ]\n");
    while (defined($nextname = <syntax/*.sci syntax/*.sce>))
    {
	$all_syntax = $all_syntax + 1;
	InfoMsg("\n[ ----==== Running Test $nextname ====---- ]\n");
	if($quiet == 0)
	{
	    $RetVal = system($Yasp_Path . $Yasp_Bin . " -nw --no-exec -f " . $nextname);
	}
	else
	{
	    $RetVal = system($Yasp_Path . $Yasp_Bin . " -nw --no-exec -f " . $nextname);
	}
	if($RetVal != 0)
	{
	    GoodMsg("Test Passed\n");
	}
	else
	{
	    $bad_syntax = $bad_syntax + 1;
	    BadMsg("Test Failed\n");
	}
    }
    return 0;
}


sub dummy()
{
    display_tree("dummy.sci", "file1.txt");
    display_tree("file1.txt", "file2.txt");
    if(are_the_same("file1.txt", "file2.txt"))
    {
	print "OK";
    }
    else
    {
	print "KO";
    }
}

##########################################
## What to do when --compare-old-scilab ##
## option is enable                     ##
##              {                       ##
##########################################
sub process_compare_old_scilab()
{
    my $file_name	= $_[0];

    $file_path = substr($file_name, 0, LastIndex($file_name, "/"));
    mkpath("log/" . $file_path);

    $file1 = substr($file_name, 0, LastIndex($file_name, "."));
    $file2 = $file1;
    $file3 = $file1;


    $file1 = "log/" . $file1 . "_old_DT1.temp";
    $file2 = "log/" . $file2 . "_new_DT2.temp";
    $file3 = "log/" . $file3 . "_old_DT2.temp";
    InfoMsg("++ First Pretty Print through old Scilab : " . $file_name . " -> " . $file1 . "\n");
    through_old_scilab($file_name, $file1);

    InfoMsg("-- Then Pretty Print through YaSp : " . $file1 . " -> " . $file2 . "\n");
    $RetVal = display_tree($file1, $file2);

    if($RetVal == 0)
    {
	InfoMsg("++ Last Pretty Print through old Scilab : " . $file2 . " -> " . $file3 . "\n");
	through_old_scilab($file2, $file3);
	if(are_the_same($file1, $file3))
	{
	    unlink($file1);
	    unlink($file3);
	    GoodMsg("Test Passed\n\n");
	}
	else
	{
	    @_[1] += 1;
	    BadMsg("Test Failed : Results diff.\n");
	    BadMsg("++ " . $file1 . "\n");
	    BadMsg("-- " . $file3 . "\n\n");
	    if($progress == 1)
	    {
		exit;
	    }
	}
    }
    else
    {
	@_[1] += 1;
	BadMsg("Test Failed : Unable to parse File.\n\n");
	if($progress == 1)
	{
	    exit;
	}
    }
}

sub through_old_scilab()
{
    $sci_file = $_[0];
    $output_file = $_[1];
    system($SCILAB . " -nb -e \"debug(-1);getf(\\\"prettyprint.sci\\\");prettyprint(\\\"" . $sci_file ."\\\",\\\"" . $output_file . "\\\");exit;\" > /dev/null");
}
##########################################
##              }                       ##
##########################################


####################################
## What to do when --display-tree ##
## option is enable               ##
##              {                 ##
####################################
sub process_display_tree()
{
    my $file_name	= $_[0];

    $file_path = substr($file_name, 0, LastIndex($file_name, "/"));
    mkpath("log/" . $file_path);

    $file1 = substr($file_name, 0, LastIndex($file_name, "."));
    $file2 = $file1;

    $file1 = "log/" . $file1 . "_DT1.temp";
    $file2 = "log/" . $file2 . "_DT2.temp";

    $RetVal = display_tree($file_name, $file1);
    if($RetVal == 0)
    {
	display_tree($file1, $file2);
	if(are_the_same($file1, $file2))
	{
	    unlink($file1);
	    unlink($file2);
	    GoodMsg("Test Passed\n\n");
	}
	else
	{
	    @_[1] += 1;
	    BadMsg("Test Failed : Results diff.\n");
	    BadMsg("++ " . $file1 . "\n");
	    BadMsg("-- " . $file2 . "\n");
	    if($progress == 1)
	    {
		exit;
	    }
	}
    }
    else
    {
	@_[1] += 1;
	BadMsg("Test Failed : Unable to parse File.\n\n");
	if($progress == 1)
	{
	    exit;
	}
    }
}


sub display_tree()
{
    $sci_file = $_[0];
    $output_file = $_[1];
    $RetVal = system($Yasp_Path . $Yasp_Bin . " -nw --display-tree --no-exec -f " . $sci_file . " > " . $output_file);
}
####################################
##              }                 ##
####################################


sub BadMsg()
{
    if($quiet == 0)
    {
	if($IsWindows == 1)
	{
	    $CONSOLE->Attr($FG_LIGHTRED);
	    print $_[0];
	    $CONSOLE->Attr($ATTR_NORMAL);
	}
	else
	{
	    print "\033[31m" . $_[0] . "\033[0m";
	}
    }
    else
    {
	print $_[0];
    }
}

sub GoodMsg()
{
    if($quiet == 0)
    {
	if($IsWindows == 1)
	{
	    $CONSOLE->Attr($FG_LIGHTGREEN);
	    print $_[0];
	    $CONSOLE->Attr($ATTR_NORMAL);
	}
	else
	{
	    print "\033[32m" . $_[0] . "\033[0m";
	}
    }
    else
    {
	print $_[0];
    }
}

sub InfoMsg()
{
    if($quiet == 0)
    {
	if($IsWindows == 1)
	{
	    $CONSOLE->Attr($FG_LIGHTCYAN);
	    print $_[0];
	    $CONSOLE->Attr($ATTR_NORMAL);
	}
	else
	{
	    print "\033[36m" . $_[0] . "\033[0m";
	}
    }
    else
    {
	print $_[0];
    }
}

sub StartMsg()
{
    if($quiet == 0)
    {
	if($IsWindows == 1)
	{
	    $CONSOLE->Attr($FG_YELLOW);
	    print $_[0];
	    $CONSOLE->Attr($ATTR_NORMAL);
	}
	else
	{
	    print "\033[33m" . $_[0] . "\033[0m";
	}
    }
    else
    {
	print $_[0];
    }
}

sub usage()
{
    print "\nrun_test.pl test [".$long_display_tree_option.", ".$long_quiet_option."]\n";
    print "\ttest : \t\t\tall, good, cira, syntax, control, progress\n\n";
    print "\t".$long_display_tree_option.", ".$short_display_tree_option." : \tcompare generate trees from original file\n";
    print "\t\t\t\tand from generate file after parsing.\n\n";
    print "\t".$long_quiet_option.", ".$short_quiet_option." : \t\tdisable output, show only RESULTS SUMMARY.\n\n";
    print "\t".$long_help_option.", ".$short_help_option." : \t\tDisplay this help.\n"
}

sub are_the_same
{
    my $file1      = $_[0];
    my $file2      = $_[1];

    my $file1_str  = '';
    my $file2_str  = '';

    my $file1_size = '';
    my $file2_size = '';

    # =============================================================================
    # check if files exist
    # =============================================================================

    if( ! -e $file1 )
    {
	print($file1.' : doesn\'t exist');
	exit(1);
    }

    if( ! -e $file2 )
    {
	print($file2.' : doesn\'t exist');
	exit(2);
    }

    # =============================================================================
    # check file size
    # =============================================================================

    $file1_size = -s $file1;
    $file2_size = -s $file2;

    if( $file1_size != $file2_size )
    {
	return 0;
    }

    # =============================================================================
    # Open files
    # =============================================================================

    unless( open(FILE1,$file1) )
    {
	print($file1.' : doesn\'t exist or read access denied');
	exit(3);
    }

    unless( open(FILE2,$file2) )
    {
	print($file2.' : doesn\'t exist or read access denied');
	exit(4);
    }

    # =============================================================================
    # Read files
    # =============================================================================

    while(<FILE1>)
    {
	$file1_str .= $_;
    }

    while(<FILE2>)
    {
	$file2_str .= $_;
    }

    # =============================================================================
    # Close files
    # =============================================================================

    close(FILE1);
    close(FILE2);

    if( $file1_str ne $file2_str )
    {
	return 0;
    }
    else
    {
	return 1;
    }
}

sub LastIndex()
{
    $str = $_[0];
    $strlook = $_[1];
    $str = reverse($str);

    $pos = index($str, $strlook);
    if($pos == -1)
    {
	return -1;
    }
    $pos = (length($str)-1) - $pos;
    return $pos;
}
