use v5.26;
use warnings;

my %assoc = (
    char => 'char',
    uchar => 'unsigned char',
    schar => 'signed char',
    short => 'short',
    ushort => 'unsigned short',
    int => 'int',
    uint => 'unsigned',
    long => 'long',
    ulong => 'unsigned long',
    long_long => 'long long',
    ulong_long => 'unsigned long long',
    float => 'float',
    double => 'double',
    long_double => 'long double',
    bool => 'bool',
);

my %name = (
    char => 'Character',
    uchar => 'Unsigned character',
    schar => 'Signed character',
    short => 'Short integer',
    ushort => 'Unsigned short integer',
    int => 'Integer',
    uint => 'Unsigned integer',
    long => 'Long integer',
    ulong => 'Unsigned long integer',
    long_long => 'Long long integer',
    ulong_long => 'Unsigned long long integer',
    float => 'Floating point',
    double => 'Double precision floating point',
    long_double => 'Long double precision floating point',
    bool => 'Boolean logical value',
);

my %tr = (
    char => ' char',
    uchar => 'n unsigned char',
    schar => ' signed char',
    short => ' short',
    ushort => 'n unsigned short',
    int => 'n int',
    uint => 'n unsigned',
    long => ' long',
    ulong => 'n unsigned long',
    long_long => ' long long',
    ulong_long => 'n unsigned long long',
    float => ' float',
    double => ' double',
    long_double => ' long double',
    bool => ' bool',
);

for my $c (qw/schar uchar char int uint long ulong long_long ulong_long short ushort bool float double long_double/) {
    my $cc = "${c}_cache";
    my $oc = $cc . '(';
    say <<"EOF";
/**
 *  \\brief $name{$c} cache
 *
 *  This cache type is responsible for caching a$tr{$c} typed value.
 */
struct $cc : visitable_cache<$cc> {
    /**
     * \\brief Cache constructor
     *
     * Constructs a cache object from the value it is meant to store.
     * For this class this is a `$assoc{$c}`.
     * The value is to be moved in, or be a literal.
     *
     * \\param data The value to store in the cache. Moved.
     */
    $oc$assoc{$c}&& data) noexcept;

    /**
     * \\brief Cache getter
     *
     * Returns a const pointer to the stored value in the cache.
     *
     * \\return The stored value.
     */
    const $assoc{$c}* get_value_ptr();
};
EOF
}
