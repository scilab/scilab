#!/usr/bin/perl

# Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
# Copyright (C) 2008 - INRIA - Pierre MARECHAL <pierre.marechal@inria.fr>
#
# Copyright (C) 2012 - 2016 - Scilab Enterprises
#
# This file is hereby licensed under the terms of the GNU GPL v2.0,
# pursuant to article 5.3.4 of the CeCILL v.2.1.
# This file was originally licensed under the terms of the CeCILL v2.1,
# and continues to be available under such terms.
# For more information, see the COPYING file which you should have received
# along with this program.

use strict;
use Cwd;
use File::Basename;
use XML::Simple;
use Data::Dumper;

# Current directory
my $directory           = getcwd();

if( dirname($0) ne '.' )
{
	$directory .= '/'.dirname($0);
}

if( dirname($0) =~ m/^\// )
{
	$directory = dirname($0);
}

# modules dir path
my $sci_modules_dir = $directory;
$sci_modules_dir =~ s/\/helptools\/src\/perl//g;

# schema path
my $rng = $sci_modules_dir.'/helptools/schema/scilab.rng';

# tmp file
my $tmp = $directory.'/tmp.txt';

# log file
my $log = $directory.'/log.txt';

# XML list
my %xmllist;

# stats
my $nb_bad_file      = 0;
my %bad_file_list;

# Initialisation du fichier de log
unless( open(LOG,'> '.$log) )
{
	print 'The file '.$log.' doesn\'t exist or write access denied'."\n";
	exit(0);
}

# ==============================================================================
# First step : get the XML list
# ==============================================================================

get_xml_list($ARGV[0]);


# ==============================================================================
# Second step : valid each XML file
# ==============================================================================

my $xmllist_size = 0;

foreach my $xmlfile (sort keys %xmllist)
{
	$xmllist_size++;
}

my $count = 0;

foreach my $xmlfile (sort keys %xmllist)
{
	$count++;
	
	my $xmlfile_print = 'SCI/modules'.substr($xmlfile,length($sci_modules_dir));
	
	printf('%04d/%04d - %s ',$count,$xmllist_size,$xmlfile_print);
	
	for( my $i =length($xmlfile) ; $i < 120 ; $i++ )
	{
		print '.';
	}
	
	my $nb_error = valid($xmlfile);
	
	if( $nb_error == 0)
	{
		print 'OK !';
	}
	else
	{
		print "\t".$nb_error.' ERRORS';
	}
	
	print "\n";
}

# ==============================================================================
# Third step : Summary
# ==============================================================================

print "\n\n";
$count = 0;

foreach my $bad_file (sort keys %bad_file_list)
{
	$count++;
	printf(" %02d",$count);
	print ' - '.$bad_file.' ';
	for( my $i =length($bad_file) ; $i < 100 ; $i++ )
	{
		print '.';
	}
	print $bad_file_list{$bad_file}." ERRORS\n";
}

# ==============================================================================
# get_module_list
# ==============================================================================

sub get_module_list
{
	my %list;
	
	unless( chdir($sci_modules_dir) )
	{
		print 'The directory '.$sci_modules_dir.' doesn\'t exist or read access denied'."\n";
		del_tmp_file();
		exit(0);
	}
	
	my @candidates = <*>;
	
	foreach my $candidate (@candidates)
	{
		if( -e $sci_modules_dir.'/'.$candidate.'/help' )
		{
			$list{$candidate} = 1;
		}
	}
	
	return %list;
}

# ==============================================================================
# get_xml_list
# ==============================================================================

sub get_xml_list
{
	my $dir      = $_[0];
	my @list_dir;
	
	my $current_directory;
	
	# On enregistre le répertoire dans lequel on se situe à l'entrée de la fonction
	my $previous_directory = getcwd();
	
	chdir($dir);
	
	@list_dir = <*>;
	
	foreach my $list_dir (@list_dir)
	{
		$current_directory = getcwd();
		
		if( (-d $list_dir) && ( ! ($list_dir =~ m/^scilab_[a-z][a-z]_[A-Z][A-Z]_help$/ )) )
		{
			get_xml_list($current_directory.'/'.$list_dir);
		}
		
		if( (-f $list_dir)
		   && ($list_dir =~ m/\.xml$/)
		   && ($list_dir ne 'master.xml')
		   && ($list_dir ne 'master_help.xml') )
		{
			$xmllist{$current_directory.'/'.$list_dir} = 1;
		}
	}
	
	chdir($previous_directory);
}

# ==============================================================================
# valid
# ==============================================================================

sub valid
{
	my $xmlfile  = $_[0];
	
	# construction de la commande
	my $cmd  = 'xmllint --noout --relaxng '.$rng.' '.$xmlfile.' ';
	$cmd    .= '> '.$tmp.' 2>&1';
	
	# Lancement de la commande
	system($cmd);
	
	# Vérification
	
	my $nb_error  = 0;
	my $error_str = '';
	
	unless( open(TMP,$tmp) )
	{
		print 'Le fichier '.$tmp.' n\'a pu être ouvert en lecture'."\n";
		del_tmp_file();
		exit(0);
	}
	
	while(<TMP>)
	{
		if( $_ eq $xmlfile.' validates'."\n" )
		{
			$nb_error  = 0;
			$error_str = '';
			last;
		}
		
		if( (!($_ =~ m/IDREF attribute linkend references an unknown ID/)) &&
		   (!($_ =~ m/Did not expect element ulink there/)) &&
		   (!($_ =~ m/element imagedata: Relax-NG validity error :/)) &&
		   (!( $_ =~ /fails to validate/)) )
		{
			$nb_error++;
			$error_str .= $_;
		}
	}
	
	close(TMP);
	
	if( $nb_error > 0 )
	{
		$nb_bad_file++;;
		$bad_file_list{$xmlfile} = $nb_error;
		
		print LOG "=================================================================\n";
		print LOG $xmlfile."\n";
		print LOG "=================================================================\n";
		print LOG $error_str;
	}
	
	return $nb_error;
}
