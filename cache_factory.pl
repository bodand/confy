use v5.26;
use warnings;

my %assoc = (
    char        => 'char',
    uchar       => 'unsigned char',
    schar       => 'signed char',
    short       => 'short',
    ushort      => 'unsigned short',
    int         => 'int',
    uint        => 'unsigned',
    long        => 'long',
    ulong       => 'unsigned long',
    long_long   => 'long long',
    ulong_long  => 'unsigned long long',
    float       => 'float',
    double      => 'double',
    long_double => 'long double',
    bool        => 'bool',
    stdstr      => 'std::string',
    stdstrv     => 'std::string_view',
    cstr        => 'const char*',
);

for my $c (qw/stdstr stdstrv cstr/) {
  say <<"EOF"
/**
 * \\brief Non-caching specialization for the $assoc{$c} type.
 *
 * This specialization of cache_factor for $assoc{$c} implements the non-caching interface.
 * With this, one can request objects of type $assoc{$c} from the configuration, but since the
 * storage works under our hand here, this needs not be cached.
 */
template<>
struct cache_factory<$assoc{$c}> {
  /**
   * \\brief Creates (trivially) a $assoc{$c} object from a string.
   *
   * Arising from the storage implementation, this function implements a trivial getter
   * functionality.
   * Creates a $assoc{$c} type object from the stored string.
   *
   * \\param data The stored data in the key-value store.
   * \\return The new object of type $assoc{$c}.
   */
  [[nodiscard]] $assoc{$c}
  make(const std::string& data) const;
};
EOF
}

for my $c (qw/schar uchar char int uint long ulong long_long ulong_long short ushort bool float double long_double/) {
  my $cc = "${c}_cache";
  say <<"EOF";
/**
 * \\brief Caching specialization for constructing $assoc{$c} type objects.
 *
 * This specialization implements the caching interface for cache_factory<>.
 * Parses the passed-in string and constructs the new object.
 */
template<>
struct cache_factory<$assoc{$c}> {
    /**
     * \\brief The cache type constructed.
     *
     * When calling the construct member function, this is the concrete type of the cache returned.
     */
    using cache_type = $cc;

    /**
     * \\brief The parser and type constructor function.
     *
     * This function is used to construct an object of the requested `$assoc{$c}` type.
     * For this it parses the passed string and using the appropriate construction mechanism crates
     * the object.
     *
     * \\param data The string stored as the value, parsed for the data.
     * \\return The cache containing the parsed object, or `nullptr` if parsing couldn't succeed.
     */
    [[nodiscard]] std::unique_ptr<cache>
    construct(const std::string& data);
};
EOF
}

