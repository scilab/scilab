#!/usr/bin/perl

# Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
# Copyright (C) 2009-2010 - DIGITEO - Pierre MARECHAL <pierre.marechal@scilab.org>
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

# perl script directory
# ------------------------------------------------

my $directory = getcwd();

if( dirname($0) ne '.' )
{
	$directory .= '/'.dirname($0);
	$directory =~ s/\/\.\//\//g;
}

if( dirname($0) =~ m/^\// )
{
	$directory = dirname($0);
}

# SCI & modules paths
# ------------------------------------------------

my $SCI =  $directory;
$SCI    =~ s/\/modules\/development_tools\/src\/perl\/xml2test//g;

my $sci_modules_dir = $SCI.'/modules';


# Managed languages
# ------------------------------------------------
my %languages;
$languages{'en_US'} = 1;


# Module list
# ------------------------------------------------
my %modules;
if( $ARGV[0] ne '' )
{
	$modules{$ARGV[0]} = 1;
}
else
{
	%modules = get_module_list();
}

# modules dir path
# ------------------------------------------------

my %valid_tags;
$valid_tags{'File_gateway'} = 1;
$valid_tags{'File_scilab'}  = 1;
$valid_tags{'Lib_name'}     = 1;
$valid_tags{'Func_list'}    = 1;

# # XML list
# ------------------------------------------------
my %xmllist;



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
			get_xml_list($this_directory,$module);
		}
	}
}

# ==============================================================================
# Second step : parse each XML file
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
	my $module = $xmllist{$xmlfile};
	
	my $xmlfile_print = 'SCI/modules'.substr($xmlfile,length($sci_modules_dir));
	printf('%04d/%04d - %s'."\n",$count,$xmllist_size,$xmlfile_print);
	my %tags = get_tag_values($xmlfile);
	
	# Check found tags
	# ==========================================================================
	
	foreach my $tag (sort keys %valid_tags)
	{
		if( (! exists($tags{$tag}) ) || ($tags{$tag} eq '') )
		{
			print "\t".'ERROR : Tag "'.$tag.'" has not been found in the file "'.$xmlfile.'"'."\n";
			exit(0);
		}
		
		if( $tag eq 'File_gateway' )
		{
			unless( $tags{$tag} =~ m/^SCI(.)*\.(c|cpp)$/ )
			{
				print "\t".'ERROR : Value of the tag "'.$tag.'" found in the file "'.$xmlfile.'" is not valid'."\n";
				exit(0);
			}
		}
		
		if( $tag eq 'File_scilab' )
		{
			unless( $tags{$tag} =~ m/^SCI(.)*\.(tst)$/ )
			{
				print "\t".'ERROR : Value of the tag "'.$tag.'" found in the file "'.$xmlfile.'" is not valid'."\n";
				exit(0);
			}
		}
	}
	
	# Get the test content
	# ==========================================================================
	
	$tags{'scilab_code'} = get_scilab_code($xmlfile);
	
	if( $tags{'scilab_code'} eq '' )
	{
		print "\t".'ERROR : The scilab code has not been found in the file "'.$xmlfile.'"'."\n";
		exit(0);
	}
	
	# Get the gateway content
	# ==========================================================================
	
	$tags{'gateway_code'} = get_gateway_code($xmlfile);
	
	if( $tags{'gateway_code'} eq '' )
	{
		print "\t".'ERROR : The gateway code has not been found in the file "'.$xmlfile.'"'."\n";
		exit(0);
	}
	
	# write the gateway code
	# ==========================================================================
	
	my $gateway_file = $tags{'File_gateway'};
	$gateway_file = $SCI . substr($gateway_file,3);
	write_gateway_code($gateway_file,\%tags);
	print "\t".'The file "'.$gateway_file.'" code has been created'."\n";
	
	
	# write the scilab code
	# ==========================================================================
	
	my $scilab_file = $tags{'File_scilab'};
	$scilab_file = $SCI . substr($scilab_file,3);
	write_scilab_code($scilab_file,\%tags);
	print "\t".'The file "'.$scilab_file.'" code has been created'."\n";
	
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
	my $module   = $_[1];
	
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
			get_xml_list($current_directory.'/'.$list_dir,$module);
		}
		
		if( (-f $list_dir)
		   && ($list_dir =~ m/\.xml$/)
		   && ($list_dir ne 'master.xml')
		   && ($list_dir ne 'master_help.xml') )
		{
			unless( open(XMLFILE,$list_dir) )
			{
				die 'get_xml_list: The file "'.$current_directory.'/'.$list_dir.'" cannot be opened'."\n";
			}
			
			while(<XMLFILE>)
			{
				$_ =~ s/^\s+//;
				$_ =~ s/\s+$//;
				
				if( $_ =~ /^<!--File_gateway:\s(.)+-->$/ )
				{
					$xmllist{$current_directory.'/'.$list_dir} = $module;
					last;
				}
			}
			
			close(XMLFILE);
		}
	}
	
	chdir($previous_directory);
}

# ==============================================================================
# get_tag_values
# ==============================================================================

sub get_tag_values
{
	my $xmlfile = $_[0];
	my %tags;
	
	unless( open(XMLFILE,$xmlfile) )
	{
		die 'get_tag_values: The file "'.$xmlfile.'" cannot be opened'."\n";
	}
	
	while(<XMLFILE>)
	{
		$_ =~ s/^\s+//;
		$_ =~ s/\s+$//;
		
		if( $_ =~ /^<!--[A-Z][a-z_]*:\s(.)+-->$/ )
		{
			$_ =~ s/<!--//;
			$_ =~ s/-->//;
			$_ =~ s/^\s+//;
			$_ =~ s/\s+$//;
			
			my $start = index($_,':');
			my $tag   = substr($_,0,$start);
			
			if( ! exists($valid_tags{$tag}) )
			{
				next;
			}
			
			my $value = substr($_,$start+1);
			$value =~ s/^\s+//;
			$value =~ s/\s+$//;
			
			$tags{$tag} = $value;
		}
	}
	
	close(XMLFILE);
	
	return %tags;
}

# ==============================================================================
# get_gateway_code
# ==============================================================================

sub get_gateway_code
{
	my $xmltree = XMLin($_[0]);  # $_[0] : path absolu du fichier XML
	
	my $refsections  = $xmltree->{'refsection'};
	my @fields       = @$refsections;
	
	foreach my $field (@fields)
	{
		if(  (exists($field->{'programlisting'}->{'role'}) ) &&
		   ($field->{'programlisting'}->{'role'} eq 'code_gateway') )
		{
			$field->{'programlisting'}->{'content'} =~ s/\s+\n/\n/g;
			return $field->{'programlisting'}->{'content'}."\n";
		}
	}
	
	return '';
}

# ==============================================================================
# get_scilab_code
# ==============================================================================

sub get_scilab_code
{
	my $xmltree = XMLin($_[0]);  # $_[0] : path absolu du fichier XML
	
	my $refsections  = $xmltree->{'refsection'};
	my @fields       = @$refsections;
	
	foreach my $field (@fields)
	{
		if((exists($field->{'programlisting'}->{'role'}) )
		   && ($field->{'programlisting'}->{'role'} eq 'code_scilab'))
		{
			my $scilab_code = $field->{'programlisting'}->{'content'}."\n";
			$scilab_code =~ s/\s+\n/\n/g;
			$scilab_code =~ s/then(\s)+error\((\s)*\"failed\"(\s)*\)(\s)*[;,](\s)*end/then pause;end/g;
			return $scilab_code;
		}
	}
	
	return '';
}

# ==============================================================================
# write_gateway_code
# ==============================================================================

sub write_gateway_code
{
	my $fileout = $_[0];
	my $tagsref = $_[1];
	my %tags    = %$tagsref;
	
	unless( open(FILEOUT,'> '.$fileout) )
	{
		print "\t".'Le fichier "'.$fileout.'" n\'a pu être ouvert en écriture'."\n";
		exit(0);
	}
	
	# Ecriture de l'entête
	# ==========================================================================
	
	unless( open(LICENSE,$directory.'/gateway_code_license.txt') )
	{
		print "\t".'Le fichier "'.$directory.'/gateway_code_license.txt" n\'a pu être ouvert en lecture'."\n";
		exit(0);
	}
	
	while(<LICENSE>)
	{
		print FILEOUT $_;
	}
	
	close(LICENSE);
	
	# Ecriture des includes
	# ==========================================================================
	
	unless( open(INCLUDES,$directory.'/includes.txt') )
	{
		print "\t".'Le fichier "'.$directory.'/includes.txt" n\'a pu être ouvert en lecture'."\n";
		exit(0);
	}
	
	while(<INCLUDES>)
	{
		print FILEOUT $_;
	}
	
	close(INCLUDES);
	
	# Ecriture du code
	# ==========================================================================
	$tags{'gateway_code'} =~ s/\s+\n/\n/g;
	print FILEOUT $tags{'gateway_code'};
	
	# Fermeture du fichier de sortie
	# ==========================================================================
	close(FILEOUT);
}

# ==============================================================================
# write_scilab_code
# ==============================================================================

sub write_scilab_code
{
	my $fileout = $_[0];
	my $tagsref = $_[1];
	my %tags    = %$tagsref;
	
	# TMPDIR management
	# ==========================================================================
	
	my $TMPDIR = basename($fileout);
	$TMPDIR  =~ s/\.tst$//g;
	$TMPDIR  = 'pathconvert(TMPDIR+"/'.$TMPDIR.'")';
	
	# table management (ilib_build 2nd input argument)
	# ==========================================================================
	
	my @functions = split(/,/,$tags{'Func_list'});
	
	my $table_str = '[';
	
	for( my $i=0 ; $i<length(@functions) ; $i++ )
	{
		if( $i > 0)
		{
			$table_str .= ';';
		}
		
		$table_str .= '"'.$functions[$i].'","'.$functions[$i].'"';
	}
	
	$table_str .= ']';
	
	
	# C file management
	# ==========================================================================
	
	my $cfile         = 'SCI+"'.substr($tags{'File_gateway'},3).'"';
	
	my $cfiletmpdir   = basename($fileout);
	$cfiletmpdir      =~ s/\.tst$//g;
	$cfiletmpdir      =  'pathconvert(TMPDIR+"/'.$cfiletmpdir.'/'.basename(substr($tags{'File_gateway'},3)).'",%F)';
	
	# Open the fileout file
	# ==========================================================================
	
	unless( open(FILEOUT,'> '.$fileout) )
	{
		print "\t".'Le fichier "'.$fileout.'" n\'a pu être ouvert en écriture'."\n";
		exit(0);
	}
	
	# Ecriture de l'entête
	# ==========================================================================
	
	unless( open(LICENSE,$directory.'/scilab_code_license.txt') )
	{
		print "\t".'Le fichier "'.$directory.'/scilab_code_license.txt" n\'a pu être ouvert en lecture'."\n";
		exit(0);
	}
	
	while(<LICENSE>)
	{
		print FILEOUT $_;
	}
	
	close(LICENSE);
	
	# Ecriture de la ligne ilib_build
	# ==========================================================================
	
	print FILEOUT 'ilib_verbose(0);'."\n";
	print FILEOUT 'mkdir('.$TMPDIR.');'."\n";
	print FILEOUT 'cd('.$TMPDIR.');'."\n";
	print FILEOUT 'copyfile('.$cfile.','.$cfiletmpdir.');'."\n";
	print FILEOUT 'cflags = "-I"+SCI+"/modules/localization/includes";'."\n";
	
	print FILEOUT 'ilib_build(';
	print FILEOUT '"'.$tags{'Lib_name'}.'",';                          # lib_name
	print FILEOUT $table_str.',';                                      # table
	print FILEOUT '"'.basename(substr($tags{'File_gateway'},3)).'",';  # files
	print FILEOUT '[],';                                               # libs
	print FILEOUT '"",';                                               # makename
	print FILEOUT '"",';                                               # ldflags
	print FILEOUT 'cflags);'."\n";                                     # cflags
	
	print FILEOUT 'exec("loader.sce");'."\n";
	
	# Ecriture du code
	# ==========================================================================
	print FILEOUT $tags{'scilab_code'};
	
	# Fermeture du fichier de sortie
	# ==========================================================================
	close(FILEOUT);
}
