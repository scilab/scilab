#!/usr/bin/perl

# Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
# Copyright (C) 2008 - INRIA - Pierre MARECHAL <pierre.marechal@inria.fr>
#
# This file must be used under the terms of the CeCILL.
# This source file is licensed as described in the file COPYING, which
# you should have received as part of this distribution.  The terms
# are also available at
# http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt

use strict;
use Cwd;
use File::Basename;

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
my $rng             = $sci_modules_dir.'/helptools/schema/scilab.rng';

# tmp file
my $tmp             = $directory.'/tmp.txt';

# log file
my $log             = $directory.'/log.txt';

# stats
my $nb_bad_file      = 0;
my %bad_file_list;

my $lang;


# Initialisation du fichier de log
unless( open(LOG,'> '.$log) )
{
	print 'The file '.$log.' doesn\'t exist or write access denied'."\n";
	exit(0);
}

# Module list
my %modules;
if( $ARGV[0] ne '' )
{
	$modules{$ARGV[0]} = 1;
}
else
{
	%modules = get_module_list();
}

$lang = 'en_US';

foreach my $module (sort keys %modules)
{
	print "=================================================================\n";
	print $module."\n";
	print "=================================================================\n";
	
	my %xml_list = get_xml_list($module,$lang);
	
	foreach my $xmlfile (sort keys %xml_list)
	{
		print ' - '.$xmlfile;
		for( my $i =length($xmlfile) ; $i < 40 ; $i++ )
		{
			print '.';
		}
		
		print '['.$lang.'].....';
		
		my $nb_error = valid($module,$lang,$xmlfile);
		
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
}

$lang = 'fr_FR';

foreach my $module (sort keys %modules)
{
	print "=================================================================\n";
	print $module."\n";
	print "=================================================================\n";
	
	my %xml_list = get_xml_list($module,$lang);
	
	foreach my $xmlfile (sort keys %xml_list)
	{
		print ' - '.$xmlfile;
		for( my $i =length($xmlfile) ; $i < 40 ; $i++ )
		{
			print '.';
		}
		
		print '['.$lang.'].....';
		
		my $nb_error = valid($module,$lang,$xmlfile);
		
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
}

close(LOG);

print "\n\n";
my $count = 0;

foreach my $bad_file (sort keys %bad_file_list)
{
	$count++;
	printf(" %02d",$count);
	print ' - '.$bad_file;
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
	my %list;
	my $module   = $_[0];
	my $language = $_[1];
	
	unless( chdir($sci_modules_dir.'/'.$module.'/help/'.$language) )
	{
		return %list;
	}
	
	my @xmlfiles = <*.xml>;
	
	foreach my $xmlfile (@xmlfiles)
	{
		$list{$xmlfile} = 1;
	}
	
	return %list;
}

# ==============================================================================
# valid
# ==============================================================================

sub valid
{
	my $module   = $_[0];
	my $language = $_[1];
	my $xmlfile  = $_[2];
	
	# xml file path
	my $xmlfile_path = $sci_modules_dir.'/'.$module.'/help/'.$language.'/'.$xmlfile;
	
	# construction de la commande
	my $cmd  = 'xmllint --noout --relaxng '.$rng.' '.$xmlfile_path.' ';
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
		if( $_ eq $xmlfile_path.' validates'."\n" )
		{
			$nb_error  = 0;
			$error_str = '';
			last;
		}
		
		if( (!($_ =~ m/IDREF attribute linkend references an unknown ID/)) &&
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
		$bad_file_list{$xmlfile_path} = $nb_error;
		
		print LOG "=================================================================\n";
		print LOG $sci_modules_dir.'/'.$module.'/help/'.$language.'/'.$xmlfile."\n";
		print LOG "=================================================================\n";
		print LOG $error_str;
	}
	
	return $nb_error;
}

# ==============================================================================
# del_tmp_file
# ==============================================================================

sub del_tmp_file
{
	unlink($tmp);
	unlink($log);
}
