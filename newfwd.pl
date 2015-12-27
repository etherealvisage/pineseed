#!/usr/bin/perl
# Perl script by DWK to create a new .cpp/.h combination template
# Updated by ethereal to create .fwd file instead

my $project = 'Pineseed';

if(scalar(@ARGV) < 1) {
    die "Usage: $0 <C++-name-of-class>\n" .
        "  e.g. $0 Socket::UDPSocket\n";
}

process_class($ARGV[0]);

sub process_class {
    my $class = shift;
    my $prefix = "src/";
    
    while($class =~ /([^:]+)::/g) {
        $prefix .= lc($1) . '/';
    }
    
    $class =~ /([^:]+)$/;
    $prefix .= $1;
    
    print "Class prefix: \"$prefix\"\n";
    create_fwd_file($class, $prefix . ".fwd");
}

sub create_fwd_file {
    my $class = shift;
    my $file = shift;
    open(FILE, ">$file") or die;

    my $guard = $class;
    $guard =~ s/([A-Z]+)/substr($1, 0, 1) . lc(substr($1, 1))/eg;
    while($guard =~ s/([A-Z])([^:]*)$/'_' . lc($1) . $2/eg) {}
    $guard =~ s/([^_])::([^:]+)$/$1_$2/;    # put two underscores at last ::
    $guard =~ s/([^_])::([^_])/$1_$2/g;
    $guard =~ s/:://g;
    $guard = uc($project) . "_" . uc($guard) . "_H";

    my @namespaces = generate_namespaces($class);

    print FILE open_namespaces(@namespaces);
    print FILE "\n";
    $class =~ /([^:]+)$/;
    print FILE "class $1;\n";
    print FILE "\n";
    print FILE close_namespaces(@namespaces);
    print FILE "\n";

    close FILE;
}

sub generate_namespaces {
    my $class = shift;
    my @spaces = ($project);
    
    while($class =~ /([^:]+)::/g) {
        push(@spaces, $1);
    }
    
    return @spaces;
}

sub open_namespaces {
    my $return = '';
    for my $namespace (@_) {
        $return .= "namespace $namespace {\n";
    }
    
    return $return;
}

sub close_namespaces {
    my $return = '';
    for my $namespace (reverse @_) {
        $return .= "}  // namespace $namespace\n";
    }
    
    return $return;
}
