#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <math.h>

#if (defined(i386) || defined(__i386__) || defined(__i386) \
     || defined(__i486__) || defined(__i586__) || defined(__i686__) \
     || defined(__X86__) || defined(_X86_) || defined(__THW_INTEL__) \
     || defined(__I86__) || defined(__INTEL__) || defined(__IA32__) \
     || defined(_M_IX86) || defined(_I86_)) && defined(BOOST_WINDOWS)
# define BOOST_CONTEXT_CALLDECL __cdecl
#else
# define BOOST_CONTEXT_CALLDECL
#endif

typedef void* fcontext_t;
typedef struct {
    fcontext_t  fctx;
    void* data;
} transfer_t;

extern
transfer_t BOOST_CONTEXT_CALLDECL jump_fcontext(fcontext_t const to, void* vp);
extern
fcontext_t BOOST_CONTEXT_CALLDECL make_fcontext(void* sp, size_t size, void (*fn)(transfer_t));

#define default_stacksize (1024*1024)

#define BOOST_CHECK(x) do {                                         \
    if(!(x)) {                                                      \
        fprintf(stderr, "%d: error %s\n", __LINE__, #x);            \
    }                                                               \
    else {                                                          \
        fprintf(stdout, "%d: success %s\n", __LINE__, #x);          \
    }                                                               \
} while(0)

#define BOOST_CHECK_EQUAL(x,y) do {                                 \
    if((x) != (y)) {                                                \
        fprintf(stderr, "%d: error %s == %s\n", __LINE__, #x, #y);  \
    }                                                               \
    else {                                                          \
        fprintf(stdout, "%d: success %s == %s\n", __LINE__, #x, #y);\
    }                                                               \
} while(0)

#define TEST(f) do {                                                \
    printf("===================== %s =====================\n", #f); \
    f();                                                            \
} while(0)


int value1 = 0;
double value3 = 0.;
void * value4 = 0;

void f1( transfer_t t) {
    ++value1;
    jump_fcontext( t.fctx, t.data);
}

void f3( transfer_t t_) {
    ++value1;
    transfer_t t = jump_fcontext( t_.fctx, 0);
    ++value1;
    jump_fcontext( t.fctx, t.data);
}

void f4( transfer_t t) {
    int i = 7;
    jump_fcontext( t.fctx, & i);
}

void f5( transfer_t t) {
    jump_fcontext( t.fctx, t.data);
}

#if 0
void f6( transfer_t t_) {
    std::pair< int, int > data = * ( std::pair< int, int > * ) t_.data;
    int res = data.first + data.second;
    transfer_t t = jump_fcontext( t_.fctx, & res);
    data = * ( std::pair< int, int > *) t.data;
    res = data.first + data.second;
    jump_fcontext( t.fctx, & res);
}
#endif

void f8( transfer_t t) {
    double d = * ( double *) t.data;
    d += 3.45;
    value3 = d;
    jump_fcontext( t.fctx, 0);
}

void f10( transfer_t t) {
    value1 = 3;
    jump_fcontext( t.fctx, 0);
}

void f9( transfer_t t) {
    printf("f1: entered\n");
    void* sp = malloc(default_stacksize);
    fcontext_t ctx = make_fcontext((char*)sp + default_stacksize, default_stacksize, f10);
    jump_fcontext( ctx, 0);
    jump_fcontext( t.fctx, 0);
}

transfer_t f11( transfer_t t_) {
    value4 = t_.data;
    transfer_t t = { t_.fctx, t_.data };
    return t;
}

void f12( transfer_t t_) {
    transfer_t t = jump_fcontext( t_.fctx, t_.data);
    value1 = * ( int *) t.data;
    jump_fcontext( t.fctx, t.data);
}

void f13( transfer_t t) {
    {
        double n1 = 0;
        double n2 = 0;
        sscanf("3.14 7.13", "%lf %lf", & n1, & n2);
        BOOST_CHECK( n1 == 3.14);
        BOOST_CHECK( n2 == 7.13);
    }
    {
        int n1=0;
        int n2=0;
        sscanf("1 23", "%d %d", & n1, & n2);
        BOOST_CHECK( n1 == 1);
        BOOST_CHECK( n2 == 23);
    }
    {
        int n1=0;
        int n2=0;
        sscanf("1 jjj 23", "%d %*[j] %d", & n1, & n2);
        BOOST_CHECK( n1 == 1);
        BOOST_CHECK( n2 == 23);
    }
    jump_fcontext( t.fctx, 0);
}

void f14( transfer_t t) {
    {
        const char *fmt = "sqrt(2) = %.2f";
        char buf[19];
        snprintf( buf, sizeof( buf), fmt, sqrt( 2) );
        BOOST_CHECK( 0 < sizeof( buf) );
        BOOST_CHECK( strcmp("sqrt(2) = 1.41", buf) == 0 );
    }
    {
        uint64_t n = 0xbcdef1234567890;
        const char *fmt = "0x%016llX";
        char buf[100];
        snprintf( buf, sizeof( buf), fmt, n);
        BOOST_CHECK( strcmp("0x0BCDEF1234567890", buf) == 0);
    }
    jump_fcontext( t.fctx, 0);
}

void test_setup() {
    void* sp = malloc(default_stacksize);
    fcontext_t ctx = make_fcontext( (char *)sp + default_stacksize, default_stacksize, f1);
    BOOST_CHECK( ctx);
    free(sp);
}

void test_start() {
    value1 = 0;
    void* sp = malloc(default_stacksize);
    fcontext_t ctx = make_fcontext((char*)sp + default_stacksize, default_stacksize, f1);
    BOOST_CHECK( ctx);

    BOOST_CHECK_EQUAL( 0, value1);
    jump_fcontext( ctx, 0);
    BOOST_CHECK_EQUAL( 1, value1);
    free(sp);
}

void test_jump() {
    value1 = 0;
    void* sp = malloc(default_stacksize);
    fcontext_t ctx = make_fcontext((char*)sp + default_stacksize, default_stacksize, f3);
    BOOST_CHECK( ctx);
    BOOST_CHECK_EQUAL( 0, value1);
    transfer_t t = jump_fcontext( ctx, 0);
    BOOST_CHECK_EQUAL( 1, value1);
    jump_fcontext( t.fctx, 0);
    BOOST_CHECK_EQUAL( 2, value1);
    free(sp);
}

void test_result() {
    void* sp = malloc(default_stacksize);
    fcontext_t ctx = make_fcontext((char*)sp + default_stacksize, default_stacksize, f4);
    BOOST_CHECK( ctx);
    transfer_t t = jump_fcontext( ctx, 0);
    int result = * ( int *) t.data;
    BOOST_CHECK_EQUAL( 7, result);
    free(sp);
}

void test_arg() {
    int i = 7;
    void* sp = malloc(default_stacksize);
    fcontext_t ctx = make_fcontext((char*)sp + default_stacksize, default_stacksize, f5);
    BOOST_CHECK( ctx);
    transfer_t t = jump_fcontext( ctx, & i);
    int result = * ( int *) t.data;
    BOOST_CHECK_EQUAL( i, result);
    free(sp);
}

#if 0
void test_transfer() {
    std::pair< int, int > data = std::make_pair( 3, 7);
    void* sp = malloc(default_stacksize);
    fcontext_t ctx = make_fcontext((char*)sp + default_stacksize, default_stacksize, f6);
    BOOST_CHECK( ctx);
    transfer_t t = jump_fcontext( ctx, & data);
    int result = * ( int *) t.data;
    BOOST_CHECK_EQUAL( 10, result);
    data = std::make_pair( 7, 7);
    t = jump_fcontext( t.fctx, & data);
    result = * ( int *) t.data;
    BOOST_CHECK_EQUAL( 14, result);
    free(sp);
}
#endif

#if 0
void test_exception() {
    std::string what("hello world");
    void* sp = malloc(default_stacksize);
    fcontext_t ctx = make_fcontext((char*)sp + default_stacksize, default_stacksize, f7);
    BOOST_CHECK( ctx);
    jump_fcontext( ctx, & what);
    BOOST_CHECK_EQUAL( std::string( what), value2);
    free(sp);
}
#endif

void test_fp() {
    double d = 7.13;
    void* sp = malloc(default_stacksize);
    fcontext_t ctx = make_fcontext((char*)sp + default_stacksize, default_stacksize, f8);
    BOOST_CHECK( ctx);
    jump_fcontext( ctx, & d);
    BOOST_CHECK_EQUAL( 10.58, value3);
    free(sp);
}

void test_stacked() {
    value1 = 0;
    void* sp = malloc(default_stacksize);
    fcontext_t ctx = make_fcontext((char*)sp + default_stacksize, default_stacksize, f9);
    jump_fcontext( ctx, 0);
    BOOST_CHECK_EQUAL( 3, value1);
    free(sp);
}

void test_sscanf() {
    void* sp = malloc(default_stacksize);
    fcontext_t ctx = make_fcontext((char*)sp + default_stacksize, default_stacksize, f13);
    BOOST_CHECK( ctx);
    jump_fcontext( ctx, 0);
    free(sp);
}

void test_snprintf() {
    void* sp = malloc(default_stacksize);
    fcontext_t ctx = make_fcontext((char*)sp + default_stacksize, default_stacksize, f14);
    jump_fcontext( ctx, 0);
    free(sp);
}

int main() {
    TEST(test_setup);
    TEST(test_start);
    TEST(test_jump);
    TEST(test_result);
    TEST(test_arg);
    //TEST(test_transfer);
    //TEST(test_exception);
    TEST(test_fp);
    TEST(test_stacked);
    //TEST(test_ontop);
    TEST(test_sscanf);
    TEST(test_snprintf);

    return 0;
}
