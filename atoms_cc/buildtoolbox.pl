#!/usr/bin/perl -w

# buildtoolbox.pl
# Usage: buildtoolbox.pl toolbox-archive [config file [stage]]

use strict;
use Cwd;

my ($TOOLBOXFILE, # Toolbox archive to compile
    $TOOLBOXNAME, # Name of the toolbox
	$STAGE); # Current stage

# common_log:
#    Print a log message. Seconf argument is the type of the
#    message:
#     " " for a normal message
#     "!" for an error
#     ">" when starting a stage
#     "<" when terminating a stage
#     "$" when running a command
#     "?" for the return code of previous command
sub common_log {
	my $message = shift;
	my $type = shift || " ";
	
	# Check message format: any newline must start by a space,
	# no new line at end of message
	$message =~ s/(?<=\n)(?!\s|$)/ /g;
	chomp $message;
	
	print LOGFILE "[".time()."]${type}${message}\n";
}

# common_enter_stage:
#    Common stuff while starting a new stage
sub common_enter_stage {
	$STAGE = shift;
	common_log($STAGE, ">");
}

# common_leave_stage:
#    Common stuff while ending new stage
sub common_leave_stage {
	common_log($STAGE, "<");
}

# common_die:
#    Called when a problem happens
sub common_die {
	my $message = shift;
	common_log($message, "!");
	common_leave_stage();
	
	while(wait() > 0) { };
	close LOGFILE;
	exit(1);
}

# common_exec:
#    Execute given command, places its outputs to log files.
#    Returns a file handle on STDOUT
#    Die if return code is non-zero
sub common_exec {
	my $cmd = shift;
	my $commandnum = 1;
	
	# Find commandnum: log files are (stage)-1.out for first
	# command of (stage), (stage)-2.out for second command of stage,
	# and so on
	$commandnum++ while(-e "$STAGE-$commandnum.out");
	
	my $stdout = "$STAGE-$commandnum.out";
	my $stderr = "$STAGE-$commandnum.err";
	
	common_log("$cmd\nstdout=$stdout\nstderr=$stderr", "\$");
	
	my $pid = fork();
	if($pid == 0) {
		open STDOUT, ">$stdout";
		open STDERR, ">$stderr";
		exec $cmd;
	}
	else {
		waitpid($pid, 0);
		common_log("$?", "?");
		if($? != 0) {
			common_die("\"$cmd\" failed");
		}
	}
	
	open my ($fd), $stdout;
	
	return $fd;
}

# is_zip:
#    Return true if toolbox file extension is zip
sub is_zip {
	return $TOOLBOXFILE =~ /\.zip$/;
}

# get_tree_from_tgz:
#   Get all files (names) of the compressed (in tar.gz) sources
sub get_tree_from_tgz {
	my %files;
	
	my $fd = common_exec("tar -tzf ${TOOLBOXFILE}");
	
	while(<$fd>) {
		chomp;
		$files{$_} = 1;
	}
	
	close $fd;
	return %files;
}

# get_tree_from_zip:
#   Get all files (names) of the compressed (in zip) sources
sub get_tree_from_zip {
	my (%files, $line);
	
	# tail & head are here to skip header & footer
	my $fd = common_exec("unzip -l ${TOOLBOXFILE} | tail -n +4 | head -n -2");
	
	while(<$fd>) {
		# zip output format: size date time filename
		/\s*\d+\s+\d+-\d+-\d+\s+\d+:\d+\s+(.+)/ or common_die "Bad output of unzip";
		chomp $1;
		$files{$1} = 1;
	}
	
	close $fd;
	return %files;
}

# get_tree:
#   Get all files (names) of the compressed sources, in a hash
#   (hash values are meaningless, set to 1)
sub get_tree {
	if(is_zip()) {
		return get_tree_from_zip();
	}
	else {
		return get_tree_from_tgz();
	}
}

# read_file_from_tgz:
#    Extract given file from the .zip archive
sub read_file_from_tgz {
	my $filename = shift;
	return common_exec("tar -xOzf ${TOOLBOXFILE} ${TOOLBOXNAME}/$filename");
}

# read_file_from_tgz:
#    Extract given file from the .tar.gz archive
sub read_file_from_zip {
	my $filename = shift;
	return common_exec("unzip -c ${TOOLBOXFILE} ${TOOLBOXNAME}/$filename | tail -n +3 | head -n -1");
}

# read_file_from_archive:
#   Extract given file from the archive
sub read_file_from_archive {
	if(is_zip()) {
		return read_file_from_zip(@_);
	}
	else {
		return read_file_from_tgz(@_);
	}
}

# read_description:
#   Check if DESCRIPTION file is correct, and parse it (return a hash
#   field => value).
#   First argument is a file descriptor for the DESCRIPTION file (see
#   get_description)
sub read_description {
	my $fd = shift;
	my @required = qw(Toolbox Version Title Author Maintainer
	                  Description License Category);
	my @optional = qw(Date Depends URL Entity);
	my (%infos, $key, $val);
	my (%lines, %correct);
	
	# Populate hash
	while(<$fd>) {
		common_die "\":\" not followed by a space at line $." if(/:(?! )/);
		if(/:/) { # New field
			($key, $val) = split(/: /, $_, 2);
			$infos{$key} = $val;
			$lines{$key} = $.;
			$correct{$key} = 0;
		}
		else { # Continuation of previous field
			$infos{$key} .= $_;
		}
	}
	
	# Check presence of required fields, mark them as correct
	foreach (@required) {
		if(!defined($infos{$_})) {
			common_die "Mandatory field \"$_\" not defined";
		}
		else {
			$correct{$_} = 1;
		}
	}
	
	# Mark optional fields as correct
	foreach (@optional) {
		if(defined($infos{$_})) {
			$correct{$_} = 1;
		}
	}
	
	# Check that there's no incorrect (= unknown) fields
	foreach (keys(%infos)) {
		if($correct{$_} == 0) {
			common_die "Unknown field \"$_\" (defined at line $lines{$_})";
		}
	}
	
	chomp %infos;
	return %infos;
}

# read_description_functions:
#   Parse DESCRIPTION-FUNCTIONS file (and check it, too). Like DESCRIPTION,
#   first argument is a file descriptor. Returns a hash function name =>
#   function description
sub read_description_functions {
	my $fd = shift;
	my (%funcs, $func, $desc);
	
	while(<$fd>) {
		if(/-/ && !/ - /) {
			common_die "\"-\" not surrounded by spaces at line $.";
		}
		
		if(/-/) { # New field
			($func, $desc) = split(/ - /, $_, 2);
			$funcs{$func} = $desc;
		}
		else { # Previous function description continuation
			$funcs{$func} .= $_;
		}
	}
	
	chomp %funcs;
	
	return %funcs;
}

# check_tree:
#   Given a source tree of a toolbox (see get_tree), check if it is correct
#   (required files are present, files are at their right place, and so on...)
sub check_tree {
	my %tree = @_;
	my %newtree;
	
	# Check that all files are under a root which has the same name as the toolbox
	# Delete this root to simplify other tests
	foreach (keys %tree) {
		if(s#^\Q$TOOLBOXNAME\E(/|$)##) {
			$newtree{$_} = 1 if $_;
		}
		else {
			common_die "Incorrect archive: \"$_\" is not a child of \"$TOOLBOXNAME\"";
		}
	}
	%tree = %newtree;
	
	# Check that basic files are here
	my @required = qw(DESCRIPTION DESCRIPTION-FUNCTIONS readme.txt license.txt
	                  builder.sce loader.sce);
	push(@required, "etc/$TOOLBOXNAME.start");
	push(@required, "etc/$TOOLBOXNAME.end");
	
	foreach (@required) {
		if(!defined($tree{$_})) {
			common_die "Incorrect archive: required file \"$_\" not present";
		}
	}
	
	# macros/ must contain only .sci and .sce files
	foreach (grep { $_ =~ m#^macros/# } keys %tree) {
		if(!/(\.sc[ie]|\/)$/) {
			common_die "Incorrect archive: macros/ must contain only .sci and .sce files".
			    " (\"$_\" found)";
		}
	}
	
	# All fortran files must be in src/fortran
	foreach (grep { $_ =~ /\.f$/} keys %tree) {
		if(!m#^src/fortran/#) {
			common_die "Incorrect archive: \"$_\" is a fortran source and hence has to be in ".
			    "src/fortran";
		}
	}

	# All c files must be in src/c or sci_gateway/{c,fortran}
	foreach (grep { $_ =~ /\.[ch]$/} keys %tree) {
		if(!m#^(src/c|sci_gateway/(c|fortran))/#) {
			common_die "Incorrect archive: \"$_\" is a C source and hence has to be in ".
			    "src/c, sci_gateway/c or sci_gateway/fortran";
		}
	}
	
	# Constraints: if $key exists, $constraints{$key} must exist
	my %constraints = (
		qr#help/.+\.xml$# => "help/buildhelp.sce",
		qr#macros/.+\.sc[ie]$# => "macros/buildmacros.sce");
	
	# Build constraints for allowed languages
	my %languages = (
		"c" => qr/[ch]/,
		"fortran" => qr/f/);
	
	foreach (keys %languages) {
		# if src/(lang) has source files, src/(lang)/buildsrc_(lang).sce must exist
		$constraints{qr#^src/$_/.+\.$languages{$_}$#} = "src/$_/buildsrc_$_.sce";
		
		# if sci_gateway/(lang) has C sources, sci_gateway/(lang)/buildgateway_(lang).sce
		# must exist
		$constraints{qr#^sci_gateway/$_/.+[ch]$#} = "sci_gateway/$_/buildgateway_$_.sce";
		
		# if src/(lang)/buildsrc_(lang).sce exist, src/buildsrc.sce must exist
		$constraints{qr#^src/$_/buildsrc_$_.sce$#} = "src/buildsrc.sce";
		
		# if sci_gateway/(lang)/buildgateway_(lang).sce exist, sci_gateway/buildgateway.sce must exist
		$constraints{qr#^sci_gateway/$_/buildgateway_$_.sce$#} = "sci_gateway/buildgateway.sce";
	}
	
	# Check constraints
	foreach my $constraint (keys %constraints) {
		my $required = $constraints{$constraint};
		my @found = grep { $_ =~ $constraint } keys %tree;
		if(@found && !defined($tree{$required})) {
			common_die "Invalid archive: \"$found[0]\" needs \"$required\", which isn't in the archive";
		}
	}
}

# stage_check:
#   Perform basic checks
sub stage_check {
	common_enter_stage("check");
	
	if(is_zip()) {
		common_log("Detected ZIP format");
	}
	else {
		common_log("Detected TAR+GZIP format");
	}
	
	# Check tree
	common_log("Checking archive structure");
	my %tree = get_tree();
	common_log("Archive files:\n" . join("\n", sort keys %tree));
	check_tree(%tree);
	
	# Check DESCRIPTION
	common_log("Checking DESCRIPTION");
	my $fd = read_file_from_archive("DESCRIPTION");
	my %desc = read_description($fd);
	common_log("Computed DESCRIPTION:\n" .
		join("\n", map { "$_: $desc{$_}" } sort keys %desc));
	
	# Check toolbox name
	if($TOOLBOXNAME ne $desc{"Toolbox"}) {
		common_die "Detected toolbox name ($TOOLBOXNAME) different from ".
		    "DESCRIPTION version ($desc{Toolbox})";
	}
	
	# Check version
	my $version = $1 if ($TOOLBOXFILE =~ /[^.]+\.([^-]+)/);
	if(!defined($version)) {
		common_die "Can't detect version from archive name ($TOOLBOXFILE)";
	}
	
	if($version ne $desc{"Version"}) {
		common_die "Detected version ($version) different from DESCRIPTION ".
		    "version ($desc{Version})";
	}
	
	# Check DESCRIPTION-FUNCTIONS
	common_log("Checking DESCRIPTION-FUNCTIONS");
	$fd = read_file_from_archive("DESCRIPTION-FUNCTIONS");
	my %funcs = read_description_functions($fd);
	common_log("Computed DESCRIPTION-FUNCTIONS:\n" .
		join("\n", map { "$_: $funcs{$_}" } sort keys %funcs));
	
	common_leave_stage("check");
}

# Init global vars, check arguments
$TOOLBOXFILE = shift;
if(!defined($TOOLBOXFILE)) {
	common_die "Toolbox source file required";
}

if(! -r $TOOLBOXFILE) {
	common_die "$TOOLBOXFILE doesn't exists or can't be read";
}

$TOOLBOXNAME = $1 if ($TOOLBOXFILE =~ /^([^.]+)/);

open LOGFILE, ">build.log";

common_log "Toolbox: $TOOLBOXNAME";
common_log "Source file: $TOOLBOXFILE";

stage_check;

close LOGFILE;
