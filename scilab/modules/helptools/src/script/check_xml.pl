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

$0 =~ s/^\.\///;

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
my $rng          = $sci_modules_dir.'/helptools/schema/scilab.rng';
#my $rng             = $sci_modules_dir.'/helptools/schema/docbook.rng';

# tmp file
my $tmp             = $directory.'/tmp.txt';

# log file
my $log             = $directory.'/log.txt';

# XML list
my %xmllist;
my %idlist;

# stats
my $nb_bad_file      = 0;
my %bad_file_list;

my %languages;
$languages{'fr_FR'} = 1;
$languages{'en_US'} = 1;
$languages{'pt_BR'} = 1;

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

# ==============================================================================
# First step : get the XML list
# ==============================================================================

foreach my $module (sort keys %modules)
{
	foreach my $language (sort keys %languages)
	{
		my $this_directory = $sci_modules_dir.'/'.$module.'/help/'.$language;
		
		if( -d $this_directory )
		{
			get_xml_list($this_directory,$language);
		}
	}
}

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
	
	my $nb_error  = valid($xmlfile);
	$nb_error    += get_id_list($xmlfile,$xmllist{$xmlfile});
	
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
	my $language = $_[1];
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
			get_xml_list($current_directory.'/'.$list_dir,$language);
		}
		
		if( (-f $list_dir)
		   && ($list_dir =~ m/\.xml$/)
		   && ($list_dir ne 'master.xml')
		   && ($list_dir ne 'master_help.xml') )
		{
			$xmllist{$current_directory.'/'.$list_dir} = $language;
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

# ==============================================================================
# del_tmp_file
# ==============================================================================

sub del_tmp_file
{
	unlink($tmp);
	unlink($log);
}

# ==============================================================================
# get_id_list
# ==============================================================================

sub get_id_list
{
	my $xmlfile  = $_[0];
	my $language = $_[1];
	
	my $xml_ref  = XMLin($xmlfile);
	my %xml_hash = %$xml_ref;
	
	my $nb_error  = 0;
	my $error_str = '';
	
	my %list = ();
	
	# First step : get the list
	# ==========================================================================
	
	# xml:id of this refentry
	
	if( exists($xml_hash{'xml:id'}) )
	{
		$list{$xml_hash{'xml:id'}} = 1;
	}
	
	# Other xml:id of this refentry
	
	if( exists($xml_hash{'refnamediv'}) )
	{
		my $refnamediv_ref = $xml_hash{'refnamediv'};
		
		# One refnamediv in this XML file
		if( (ref($refnamediv_ref) eq 'HASH') && exists($refnamediv_ref->{'xml:id'}) )
		{
			$list{$refnamediv_ref->{'xml:id'}} = 1;
		}
		
		# Several refnamediv in this XML file
		elsif( ref($refnamediv_ref) eq 'ARRAY' )
		{
			my @refnamediv_tab = @$refnamediv_ref;
			
			foreach my $refnamediv (@refnamediv_tab)
			{
				my %refnamediv_hash = %$refnamediv;
				
				if(exists($refnamediv->{'xml:id'}) )
				{
					$list{$refnamediv->{'xml:id'}} = 1;
				}
			}
		}
	}
	
	# Second step : Check
	# ==========================================================================
	
	foreach my $id (sort keys %list)
	{
		my $final_id = $language.'_'.$id;
		
		if( exists($idlist{$final_id}) )
		{
			# ERROR
			
			if( exists($bad_file_list{$xmlfile}) )
			{
				$bad_file_list{$xmlfile} = $bad_file_list{$xmlfile} + 1;
			}
			else
			{
				$nb_bad_file++;;
				$bad_file_list{$xmlfile} = 1;
			}
			
			print LOG "=================================================================\n";
			print LOG $xmlfile."\n";
			print LOG "=================================================================\n";
			print LOG 'The xml:id "'.$id.'" is already used in the file "'.$idlist{$final_id}.'"'."\n";
			
			$nb_error++;
		}
		else
		{
			$idlist{$final_id} = $xmlfile;
		}
	}
	
	return $nb_error;
}
# ==============================================================================
# check_links
# ==============================================================================

sub check_links
{
	my $xmlfile  = $_[0];
	my $language = $_[1];
	
	my %links    = ();
	
	my $nb_error = 0;
	
	my $xml_ref  = XMLin($xmlfile);
	get_links($xml_ref,\%links);
	
	foreach my $link (sort keys %links)
	{
		if( ! exists($idlist{$language.'_'.$link}) )
		{
			# ERROR
			
			if( exists($bad_file_list{$xmlfile}) )
			{
				$bad_file_list{$xmlfile} = $bad_file_list{$xmlfile} + 1;
			}
			else
			{
				$nb_bad_file++;;
				$bad_file_list{$xmlfile} = 1;
			}
			
			print LOG "=================================================================\n";
			print LOG $xmlfile."\n";
			print LOG "=================================================================\n";
			print LOG 'The id "'.$link.'" hasn\'t been found'."\n";
			
			$nb_error++;
		}
	}
	
	return $nb_error;
}

# ==============================================================================
# get_links
# ==============================================================================

sub get_links
{
	my $ref        = $_[0];
	my $links_ref  = $_[1];
	
	if( ref($ref) eq 'HASH' )
	{
		if( exists($ref->{'link'}) )
		{
			my $linkref = $ref->{'link'};
			
			if( (ref($linkref) eq 'HASH') && exists($linkref->{'linkend'}) )
			{
				$links_ref->{$linkref->{'linkend'}} = 1;
			}
		}
		
		my %ref_hash = %$ref;
		
		foreach my $item (keys %ref_hash)
		{
			get_links($ref_hash{$item},$links_ref);
		}
	}
	elsif( ref($ref) eq 'ARRAY' )
	{
		my @ref_tab = @$ref;
		
		foreach my $item (@ref_tab)
		{
			get_links($item,$links_ref);
		}
	}
}
