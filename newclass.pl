#!/usr/bin/perl
# Perl script by DWK to create a new .cpp/.h combination template

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
    create_cpp_file($class, $prefix . ".cpp");
    create_h_file($class, $prefix . ".h");
}

sub create_cpp_file {
    my $class = shift;
    my $file = shift;
    open(FILE, ">$file") or die "Cannot open file";
    
    my @namespaces = generate_namespaces($class);
    
    $class =~ /([^:]+)$/;
    print FILE "#include \"$1.h\"\n";
    print FILE "\n";
    print FILE open_namespaces(@namespaces);
    print FILE "\n";
    print FILE close_namespaces(@namespaces);
    
    close FILE;
}

sub create_h_file {
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
    
    print FILE "#ifndef $guard\n";
    print FILE "#define $guard\n";
    print FILE "\n";
    print FILE open_namespaces(@namespaces);
    print FILE "\n";
    $class =~ /([^:]+)$/;
    print FILE "class $1 {\n";
    print FILE "public:\n";
    print FILE "};\n";
    print FILE "\n";
    print FILE close_namespaces(@namespaces);
    print FILE "\n";
    print FILE "#endif\n";
    
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
