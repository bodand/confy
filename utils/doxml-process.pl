#!/usr/bin/perl

# ABSTRACT: Doxygen PerlMod output to AsciiDoc converter

use v5.26;
use strict;
use warnings;
use utf8;

use Data::Dumper;

use DoxyDocs;
no strict;
my $dox = $doxydocs; # eskü létezik
use strict;

close STDOUT;
open STDOUT, '>', 'doxy.adoc'
    or die "couldn't reopen STDIN: doxy.adoc: $!";

sub filter_classnames :prototype(_);
sub filter_files :prototype(_);
sub filter_functions :prototype(_);

sub render_text :prototype($);

sub format_class :prototype($_);
sub format_argument :prototype(_);
sub format_targument :prototype(_);
sub format_function :prototype($_;$);
sub format_params :prototype(@);
sub format_tparams :prototype(@);
sub format_base :prototype(_);

my @classes = grep {filter_classnames} $dox->{classes}->@*;
my @files = grep {filter_files} $dox->{files}->@*;
my @headers = grep {$_->{name} =~ /capture_stdio\.hpp$/} @files;

my @global_functions = ();
for my $hdr (@headers) {
  next unless defined $hdr;
  next unless defined $hdr->{functions};
  next unless defined $hdr->{functions}{members};
  for my $fn ($hdr->{functions}{members}->@*) {
    $fn->{_in_file} = $hdr->{name};
    push @global_functions, $fn;
  }
}

say "== Classes";
say format_class(3, $_) for @classes;
say "== Global free functions";
say format_function(3, $_) for @global_functions;

sub filter_classnames :prototype(_) {
  return $_[0]{name} !~ /^std::/;
}

sub filter_files :prototype(_) {
  return $_[0]{name} !~ /gtest_lite/;
}

sub filter_functions :prototype(_) {
  return 1;
}

sub format_function :prototype($_;$) {
  my ($base_lvl, $fn, $scope) = @_;
  if ($base_lvl > 5) {
    $base_lvl = 5;
    warn "capped base level to 5";
  }
  my $base = '=' x $base_lvl;

  my %arg_type_table = ();
  for my $arg ($fn->{parameters}->@*) {
    $arg_type_table{$arg->{declaration_name}} = $arg->{type}
  }

  my @tparams = ();
  for my $doc ($fn->{detailed}{doc}->@*) {
    next unless exists $doc->{templateparam};
    my @tpars = $doc->{templateparam}->@*;

    for my $tpar (@tpars) {
      my $tparam = {
          name => $tpar->{parameters}[0]{name},
          type => 'class',
          doc  => $tpar->{doc}
      };

      ##### HARDCODING IMMINENT #####
      # as far as I can tell, Doxygen PerlMod output doesn't include the type of the template
      # parameters, as in class/typename/NTTP.
      if ($tparam->{name} eq 'IStrm') {
        $tparam->{type} = 'std::istream*'
      }
      elsif ($tparam->{name} eq 'OStrm') {
        $tparam->{type} = 'std::ostream*';
      }
      ##### DANGER PASSED #####

      push @tparams, $tparam
    }
  }
  my $is_template = !!@tparams;

  my @params = ();
  for my $doc ($fn->{detailed}{doc}->@*) {
    next unless exists $doc->{params};
    my @pars = $doc->{params}->@*;

    for my $par (@pars) {
      my $name = $par->{parameters}[0]{name};
      push @params, {
          name => $name,
          type => $arg_type_table{$name} // '<<UNKNOWN>>',
          doc  => $par->{doc}
      };
    }
  }
  my $has_params = !!@params;

  my $name = $fn->{name};
  $name = "${scope}::$name" if defined $scope;

  my $include = '';
  $include = "#include <$fn->{_in_file}>" if exists $fn->{_in_file};

  my $type = '';
  $type = $fn->{type} . ' ' if exists $fn->{type};

  return <<"EOF"
$base $name()

$base= Synopsis

[source,cpp]
----
$include@{[ $is_template ? "\ntemplate<" . join(', ', map { format_targument } @tparams) . ">"
                 : "" ]}
$type$fn->{name}(@{[ join ', ', map { format_argument } $fn->{parameters}->@* ]})
----@{[ render_text $fn->{brief}{doc} ]}
@{[ $is_template ? "\n$base= Template arguments\n\n" . format_tparams(@tparams) . "\n"
                 : ""]}
@{[ $has_params ? "\n$base= Arguments\n\n" . format_params(@params) . "\n"
                : ""]}
$base= Description@{[ render_text $fn->{detailed}{doc} ]}

EOF

}

sub format_class :prototype($_) {
  my ($base_lvl, $class) = @_;
  if ($base_lvl > 5) {
    $base_lvl = 5;
    warn "capped base level to 5";
  }
  my $base = '=' x $base_lvl;

  my $is_template = exists $class->{template_parameters};
  my @tparams = ();
  if ($is_template) {
    for my $tparam ($class->{template_parameters}->@*) {
      push @tparams, {
          name => $tparam->{name} // 'unnamed',
          type => $tparam->{type},
          doc  => [ { type => 'text', content => 'PLACEHOLDER' } ] # TODO
      }
    }
  }

  my %functions = (
      public    => '',
      protected => '',
      private   => '',
  );

  for my $lvl (qw/public protected private/) {
    next unless exists $class->{"${lvl}_methods"};
    $functions{$lvl} = "\n" . join "\n", map {format_function $base_lvl + 2, $_, $class->{name}} $class->{"${lvl}_methods"}{members}->@*;
  }

  return <<"EOF";
$base $class->{name}

$base= Synopsis

[source,cpp]
----
#include <$class->{includes}{name}>

$class->{kind} $class->{name}@{[ exists($class->{base}) ? " : " . join(',', map {format_base} $class->{base}->@*) : "" ]};
----@{[ render_text $class->{brief}{doc} ]}
@{[ $is_template ? "\n$base= Template arguments\n\n" . format_tparams(@tparams) . "\n"
                 : ""]}
$base= Description@{[ render_text $class->{detailed}{doc} ]}

$base= Public members$functions{public}

$base= Protected members$functions{protected}

$base= Private members$functions{private}

EOF

}

sub format_base :prototype(_) {
  my ($base) = @_;
  my $virt = '';
  $virt = 'virtual' unless $base->{virtualness} eq 'non_virtual';
  return "$base->{protection} $virt$base->{name}"
}

sub format_tparams :prototype(@) {
  my (@tparams) = @_;

  my $joined = '';
  for my $tparam (@tparams) {
    my $doc_text = render_text($tparam->{doc});
    $doc_text =~ s/\A\s+|\s+\z//;
    $joined .= "$tparam->{type} _$tparam->{name}_:: $doc_text\n"
  }
  return $joined
}

sub format_params :prototype(@) {
  my (@params) = @_;

  my $joined = '';
  for my $param (@params) {
    my $doc_text = render_text($param->{doc});
    $doc_text =~ s/\A\s+|\s+\z//;
    $joined .= "$param->{type} _$param->{name}_:: $doc_text\n"
  }
  return $joined
}

sub format_argument :prototype(_) {
  my ($arg) = @_;
  return "$arg->{type} $arg->{declaration_name}"
}

sub format_targument :prototype(_) {
  my ($arg) = @_;
  return "$arg->{type} $arg->{name}"
}

sub render_text :prototype($) {
  state %type_callback = (
      parbreak => sub {"\n\n"},
      text     => sub {
        my ($args) = @_;
        return $args->{content}
      },
      url      => sub {
        my ($args) = @_; # TODO properly implement
        return $args->{content}
      },
      style    => sub {
        my ($args) = @_;
        if ($args->{style} eq 'code') {
          return '`';
        }
        elsif ($args->{style} eq 'italic') {
          return '_';
        }
        else {
          warn "unhandled style $args->{style}";
          return '';
        }
      },
      ''       => sub {''}
  );
  state $backup = sub {
    my ($args) = @_;
    warn "unhandled type $args->{type}";
    return '';
  };

  my ($blocks) = @_;
  my $joined = '';
  for my $block ($blocks->@*) {
    next unless defined $block->{type}; # requires special care
    my $handler;
    if (exists($type_callback{$block->{type}})) {
      $handler = $type_callback{$block->{type}}
    }
    $handler //= $backup;
    $joined .= $handler->($block);
  }
  chomp $joined;
  chomp $joined;
  return $joined;
}

__END__
=pod

=head1 NAME

doxml.process.pl - Doxygen PerlMod output to AsciiDoc converter

=head1 SYNOPSIS

doxml-process.pl INPUT OUTPUT

=cut

