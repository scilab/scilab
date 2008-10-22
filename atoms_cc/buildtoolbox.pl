#!/usr/bin/perl -w

# buildtoolbox.pl
# Usage: buildtoolbox.pl toolbox-archive [config file [stage]]

use strict;
use Cwd;

my ($TOOLBOXFILE, # Toolbox archive to compile
    $TOOLBOXNAME, # Name of the toolbox
    $STAGE); # Current stage

# Save standard I/O for common_exec
open OLD_STDOUT, ">&STDOUT";
open OLD_STDERR, ">&STDERR";
open OLD_STDIN, "<&STDIN";

# common_log(message, type):
#    Print a log message. Second argument is the type of the
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
	print "[$type] $message \n";
}

# common_enter_stage(stage):
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

# common_die(message):
#    Called when a problem happens
sub common_die {
	my $message = shift;
	common_log($message, "!");
	common_leave_stage();
	
	while(wait() > 0) { };
	close LOGFILE;
	exit(1);
}

# common_exec(command, args..., [opts]):
#    Execute given command, places its outputs to log files. If last argument
#    is a reference to a hash, it's considered as options for the function.
#    Right now, only one option is available, "stderr_to_stdout", which do the
#    same as 2>&1 in shell.
#    Returns a file handle on STDOUT.
#    Die if return code is non-zero or if standard error is non-empty.
sub common_exec {
	# pretty_arg:
	#     Human-readable form of the arguments array
	sub pretty_arg {
		my $_ = shift;
		if(/\s|["']/) {
			s/"/\\"/g;
			s/^/"/;
			s/$/"/;
		}
		return $_;
	}
	
	my $refopts = pop if ref($_[-1]) eq "HASH";
	my %opts;
	   %opts = %$refopts if defined($refopts);
	
	my $cmd = join(" ", map { pretty_arg $_ } @_);
	my $commandnum = 1;
	
	# Find commandnum: log files are (stage)-1.out for first
	# command of (stage), (stage)-2.out for second command of stage,
	# and so on
	$commandnum++ while(-e "$STAGE-$commandnum.out");
	
	my $stdout = "$STAGE-$commandnum.out";
	my $stderr = "$STAGE-$commandnum.err";
	
	common_log("$cmd\nstdout=$stdout\nstderr=$stderr", "\$");
	
	# Setup I/O for subprocess
	open STDOUT, ">$stdout";
	open STDERR, ">$stderr";
	
	if(defined($opts{"stderr_to_stdout"})) {
		close STDERR;
		open STDERR, ">&STDOUT";
	}
	
	close STDIN;
	
	# Exec suprocess
	system { $_[0] } @_;
	
	# Restore I/O
	open STDIN, "<&OLD_STDIN";
	open STDOUT, ">&OLD_STDOUT";
	open STDERR, ">&OLD_STDERR";

	common_log("$?", "?");
	common_die("\"$cmd\" failed (non-zero exit code)") if($? != 0);
	common_die("\"$cmd\" failed (non-empty error output)") if(-s $stderr);
	
	open my ($fd), $stdout;
	
	return $fd;
}

# common_exec_scilab(script):
#     Execute scilab script
sub common_exec_scilab {
	my $script = shift;
	$script = "try; $script; catch; write(%io(2), lasterror()); end; quit;";
	
	my $scilab = "scilex" if($^O =~ /mswin/i);
	   $scilab = "scilab" unless(defined($scilab));
	
	return common_exec($scilab, "-nwni", "-nb", "-e", $script);
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
	
	my $fd = common_exec("tar", "-tf", $TOOLBOXFILE);
	
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
	my $fd = common_exec("unzip", "-l", $TOOLBOXFILE);
	
	while(<$fd>) {
		if(((/^\s*-+/)...(/^\s*-+/)) && !/^\s*-+/) { # Delete header & footer
			# zip output format: size date time filename
			/\s*\d+\s+\d+-\d+-\d+\s+\d+:\d+\s+(.+)/ or common_die "Bad output of unzip";
			chomp $1;
			$files{$1} = 1;
		}
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

# read_file_from_tgz(filename):
#    Extract given file from the .zip archive
sub read_file_from_tgz {
	my $filename = shift;
	return common_exec("tar", "-xOf", $TOOLBOXFILE, "$TOOLBOXNAME/$filename");
}

# read_file_from_tgz(filename):
#    Extract given file from the .tar.gz archive
sub read_file_from_zip {
	my $filename = shift;
	return common_exec("unzip", "-p", $TOOLBOXFILE, "$TOOLBOXNAME/$filename");
}

# read_file_from_archive(filename):
#   Extract given file from the archive
sub read_file_from_archive {
	if(is_zip()) {
		return read_file_from_zip(@_);
	}
	else {
		return read_file_from_tgz(@_);
	}
}

# read_description(*description):
#   Check if DESCRIPTION file is correct, and parse it (return a hash
#   field => value).
#   First argument is a file descriptor for the DESCRIPTION file (see
#   get_description)
sub read_description {
	my $fd = shift;
	my @required = qw(Toolbox Version Title Author Maintainer License
	                  Description ScilabVersion Category);
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

# read_description_functions(*description_functions):
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

# check_tree(%tree):
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
	                  changelog.txt builder.sce);
	push(@required, "etc/$TOOLBOXNAME.start");
	push(@required, "etc/$TOOLBOXNAME.quit");
	
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
		qr#^help/([a-z][a-z]_[A-Z][A-Z])/[^/]+\.xml$# => sub{ "help/$1/build_help.sce" },
		qr#^help/([a-z][a-z]_[A-Z][A-Z])/build_help.sce$# => sub{ "help/$1/addchapter.sce" },
		qr#^help/([a-z][a-z]_[A-Z][A-Z])/addchapter.sce$# => sub{ "help/builder_help.sce" },
		qr#^sci_gateway/builder_gateway.sce$# => sub{ "sci_gateway/loader_gateway.sce" },
		qr#^macros/.+\.sc[ie]$# => sub{ "macros/buildmacros.sce" });
	
	# Build constraints for allowed languages
	my %languages = (
		"c" => qr/[ch]/,
		"fortran" => qr/f/);
	
	foreach (keys %languages) {
		# if src/(lang) has source files, src/(lang)/builder_(lang).sce must exist
		$constraints{qr#^src/($_)/.+\.$languages{$_}$#} = sub{ "src/$1/builder_$1.sce" };
		
		# if sci_gateway/(lang) has C sources, sci_gateway/(lang)/builder_gateway_(lang).sce
		# must exist
		$constraints{qr#^sci_gateway/($_)/.+[ch]$#} = sub{ "sci_gateway/$1/builder_gateway_$1.sce" };
		
		# if src/(lang)/builder_(lang).sce exist, src/builder_src.sce must exist
		$constraints{qr#^src/$_/builder_$_.sce$#} = sub{ "src/builder_src.sce" };
		
		# if sci_gateway/(lang)/builder_gateway_(lang).sce exist, sci_gateway/builder_gateway.sce must exist
		$constraints{qr#^sci_gateway/$_/builder_gateway_$_.sce$#} = sub{ "sci_gateway/builder_gateway.sce" };
	}
	
	# Check constraints
	foreach my $constraint (keys %constraints) {
		foreach my $file (keys %tree) {
			if($file =~ $constraint) {
				my $required = $constraints{$constraint}();
				common_die "Invalid archive: \"$&\" needs \"$required\", which isn't in the archive"
					unless(defined($tree{$required}));
			}
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
	
	common_leave_stage();
}

# stage_unpack:
#     Extract the archive
sub stage_unpack {
	common_enter_stage("unpack");
	
	if(is_zip()) {
		common_exec("unzip", "-o", $TOOLBOXFILE);
	}
	else {
		common_exec("tar", "-xvf", $TOOLBOXFILE,
			{'stderr_to_stdout' => 1});
	}
	
	common_leave_stage();
}

# stage_makeenv:
#    Build up the environment
sub stage_makeenv {
	common_enter_stage("makeenv");
	# TODO
	common_leave_stage();
}

# compare_versions:
#    Returns -1 if version a < version b, 0 if equals, 1 else
sub compare_versions {
	my $versa = shift;
	my $versb = shift;
	my @va = split(/\./, $versa);
	my @vb = split(/\./, $versb);
	
	if($#va < $#vb) {
		return -compare_versions($versb, $versa);
	}
	
	for(my $i = 0; $i < $#vb; ++$i) {
		return  1 if $va[$i] > $vb[$i];
		return -1 if $va[$i] < $vb[$i];
	}
	
	return 1 if($#va > $#vb);
	return 0;
}

# stage_tbdeps:
#    Install toolbox dependencies
sub stage_tbdeps {
	my $fd;
	my @depsarray;
	my (%deps, %desc);
	
	common_enter_stage("tbdeps");
	
	# We alreay made the check, reading description should be OK
	open $fd, "$TOOLBOXNAME/DESCRIPTION";
	%desc = read_description($fd);
	close($fd);
	
	# Make a hash depname => depvers
	@depsarray = split(/\s*,\s*/, $desc{"Depends"} || "");
	foreach (@depsarray) {
		if(/^(\S+?)\s*\(([<>]?=)\s*([^)]+)\)$/) { # toolbox-name (version-comparator version)
			$deps{$1} = "$2$3";
		}
		else {
			$deps{$_} = "*";
		}
	}
	
	common_log("Dependencies: " . join(",", map { "$_ $deps{$_}" } keys %deps));
	
	# Install dependencies
	close(common_exec_scilab("installToolbox('$_',1,'$deps{$_}')")) foreach(keys %deps);
	
	# Find toolboxes directory
	$fd = common_exec_scilab("printf('path: %s\\n', cd(atomsToolboxDirectory()))");
	
	my $tbpath;
	while(<$fd>) {
		if(/^path: (.+?)\r?$/) {
			$tbpath = $1;
			last;
		}
	}
	
	if(!defined($tbpath)) {
		common_die("Can't find toolboxes directory");
	}
	
	common_log("Toolboxes directory: $tbpath\n");
	
	# Check if required toolboxes are installed
	foreach my $dep (keys %deps) {
		common_log("Checking $dep");
		if(! -r "$tbpath/$dep/DESCRIPTION") {
			common_die("Needed toolbox \"$dep\" is not installed");
		}
		
		next if($deps{$dep} eq "*");
		
		open $fd, "$tbpath/$dep/DESCRIPTION";
		my %desc2 = read_description($fd);
		close $fd;
		
		$deps{$dep} =~ /^([<>]?=)(.+)$/;
		
		# You can see this as "installed_version - required_version"
		my $cmp = compare_versions($desc2{"Version"}, $2);
		
		if($1 eq ">=" && $cmp == -1) { # <=> !($cmp >= 0) <=> !(installed >= required)
			common_die("We need \"$2\" >= $1, but version $desc2{Version} installed");
		}
		elsif($1 eq "=" && $cmp != 0) {
			common_die("We need \"$2\" == $1, but version $desc2{Version} installed");
		}
		elsif($cmp == 1) {  # <=> !($cmp <= 0) <=> !(installed <= required)
			common_die("We need \"$2\" <= $1, but version $desc2{Version} installed");
		}
	}
	
	common_leave_stage();
}

# stage_sysdeps:
#    Install system dependencies
sub stage_sysdeps {
	common_enter_stage("sysdeps");
	# TODO
	common_leave_stage();
}

# stage_build:
#     Run the build script
sub stage_build {
	common_enter_stage("build");
	
	# Generate ccbuilder.sce (see __DATA__ section)
	common_log("Generating ccbuilder.sce");
	my $ccbuilder;
	$ccbuilder .= $_ while(<DATA>);
	open CCBUILDER, ">ccbuilder.sce";
	print CCBUILDER $ccbuilder;
	close CCBUILDER;
	common_log("Generated ccbuilder.sce:\n$ccbuilder");
	
	# Run build script
	common_log("Running ccbuilder.sce");
	my $fd = common_exec_scilab("chdir('$TOOLBOXNAME'); exec('../ccbuilder.sce')");
	
	# Check result
	common_log("Checking build result");
	my $done = 0;
	
	while(<$fd>) {
		$done = 1 if(/^atoms_cc_builder:done\r?$/);
		if(/^atoms_cc_ilib_compile:\s*(.+?)\s*$/) {
			common_die("Generated library \"$1\" is invalid") unless($1 && ! -d $1 && (-x $1 || $^O =~ /win/i));
		}
	}
	
	# fixme: need to check if everything was OK in macros/help generation
	
	common_die("builder.sce script didn't terminate normally") unless($done);
	common_leave_stage();
}

# stage_pack:
#     Make the archive
sub stage_pack {
	common_enter_stage("pack");
	
	my @files = qw(readme.txt license.txt changelog.txt DESCRIPTION-FUNCTIONS
		DESCRIPTION macros src help sci_gateway demos tests locales includes loader.sce);
	push(@files, "etc/$TOOLBOXNAME.start");
	push(@files, "etc/$TOOLBOXNAME.quit");
	
	my $output = $TOOLBOXFILE;
	$output =~ s/(\.zip|\.tar.gz)$//;
	$output .= "-bin";
	
	common_log("Making binary .tar.gz archive ($output.tar.gz)");
	common_exec("tar", "-cvf", "$output.tar.gz", (map { "$TOOLBOXNAME/$_" } @files),
		{"stderr_to_stdout" => 1});
	common_log("Making binary .zip archive ($output.zip)");
	common_exec("zip", "-r", "$output.zip", map { "$TOOLBOXNAME/$_" } @files);
	
	common_leave_stage();
}

# stage_cleanenv:
#     Clean up the environment
sub stage_cleanenv {
	common_enter_stage("cleanenv");
	# TODO
	common_leave_stage();
}

# Init global vars, check arguments
open LOGFILE, ">build.log";

$STAGE = "";

$TOOLBOXFILE = shift;
if(!defined($TOOLBOXFILE)) {
	common_die "Toolbox source file required";
}

if(! -r $TOOLBOXFILE) {
	common_die "$TOOLBOXFILE doesn't exists or can't be read";
}

$TOOLBOXNAME = $1 if ($TOOLBOXFILE =~ /^([^.]+)/);

common_log "Toolbox: $TOOLBOXNAME";
common_log "Source file: $TOOLBOXFILE";

stage_check;
stage_unpack;
stage_makeenv;
stage_tbdeps;
stage_sysdeps;
stage_build;
stage_pack;
stage_cleanenv;

close LOGFILE;
close OLD_STDERR;
close OLD_STDOUT;
close OLD_STDIN;

# Overwrite some scilab functions to get its return value and extra infos
__DATA__
predef(0);
funcprot(0);

old_ilib_compile = ilib_compile;
function libn = ilib_compile(lib_name,makename,files,ldflags,cflags,fflags)
    libn = old_ilib_compile(lib_name,makename,files,ldflags,cflags,fflags);
    mprintf("\natoms_cc_ilib_compile:%s/%s\n", pwd(), libn);
endfunction

exec("builder.sce");
mprintf("\natoms_cc_builder:done\n");

