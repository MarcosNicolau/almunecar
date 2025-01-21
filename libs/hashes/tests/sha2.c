#include <hashes/sha256.h>
#include <string.h>
#include <utils/test.h>

#include <hashes/sha256.h>
#include <string.h>
#include <utils/test.h>

void test_sha256_empty() {
    sha256 hash = sha256_new();
    sha256_update(&hash, (uint8_t *)"", 0);
    u256 digest = sha256_finalize(&hash);
    char *str = u256_to_dec_string(digest);
    char *expected_result = "1029873362495540970295352123225813227897999006481980349933793970011156"
                            "65086549";
    assert_that(strcmp(str, expected_result) == 0);
}

void test_sha256_single_char() {
    sha256 hash = sha256_new();
    uint8_t bytes[1] = "a";
    sha256_update(&hash, bytes, 1);
    u256 digest = sha256_finalize(&hash);
    char *str = u256_to_dec_string(digest);
    char *expected_result = "9163488015244361753484262128703993804158108125491405800297860105017955"
                            "6493499";
    assert_that(strcmp(str, expected_result) == 0);
}

void test_sha256_short_string() {
    sha256 hash = sha256_new();
    uint8_t bytes[3] = "abc";
    sha256_update(&hash, bytes, 3);
    u256 digest = sha256_finalize(&hash);
    char *str = u256_to_dec_string(digest);
    char *expected_result = "8434236848709080036652383492814226366010488369501651437746298582971681"
                            "7089965"; // SHA-256("abc") result
    assert_that(strcmp(str, expected_result) == 0);
}

void test_sha256_long_string() {
    sha256 hash = sha256_new();
    uint8_t bytes[] = "The quick brown fox jumps over the lazy dog";
    sha256_update(&hash, bytes, strlen((char *)bytes));
    u256 digest = sha256_finalize(&hash);
    char *str = u256_to_dec_string(digest);
    char *expected_result = "9754582991727437845042049306863340363436609792361092711364013968352019"
                            "4405778";
    assert_that(strcmp(str, expected_result) == 0);
}

void test_sha256_binary_data() {
    sha256 hash = sha256_new();
    uint8_t bytes[] = {0x00, 0x01, 0x02, 0x03, 0xFF};
    sha256_update(&hash, bytes, sizeof(bytes));
    u256 digest = sha256_finalize(&hash);
    char *str = u256_to_dec_string(digest);
    char *expected_result = "1155050113059766760852447818468967315849566989347365055840175606142539"
                            "40679982"; // SHA-256 of binary data
    assert_that(strcmp(str, expected_result) == 0);
}

void test_sha256_repeated_updates() {
    sha256 hash = sha256_new();
    uint8_t part1[] = "Hello, ";
    uint8_t part2[] = "world!";
    sha256_update(&hash, part1, strlen((char *)part1));
    sha256_update(&hash, part2, strlen((char *)part2));
    u256 digest = sha256_finalize(&hash);
    char *str = u256_to_dec_string(digest);
    char *expected_result = "2233181402739248830710573607548020574234866647396933363417373207145921"
                            "5699411";
    assert_that(strcmp(str, expected_result) == 0);
}

void test_sha256_mid_input() {
    sha256 hash = sha256_new();
    int size = 62;
    uint8_t bytes[size];
    for (int i = 0; i < size - 1; i++)
        bytes[i] = 'a';
    bytes[size - 1] = '\0';
    sha256_update(&hash, bytes, strlen((char *)bytes));
    u256 digest = sha256_finalize(&hash);
    char *str = u256_to_dec_string(digest);
    char *expected_result = "24350659281745930032268338123534478735493466811181"
                            "724436128984886588430415152";
    assert_that(strcmp(str, expected_result) == 0);
}

void test_sha256_long_input() {
    sha256 hash = sha256_new();
    int size = 1001;
    uint8_t bytes[size];
    for (int i = 0; i < size - 1; i++)
        bytes[i] = 'a';
    bytes[size - 1] = '\0';

    sha256_update(&hash, bytes, strlen((char *)bytes));
    u256 digest = sha256_finalize(&hash);
    char *str = u256_to_dec_string(digest);
    char *expected_result = "29820712876050628553104236154147713728727538950694"
                            "247640693841099527019527843";
    assert_that(strcmp(str, expected_result) == 0);
}

int main() {
    BEGIN_TEST();
    test(test_sha256_empty);
    test(test_sha256_single_char);
    test(test_sha256_short_string);
    test(test_sha256_long_string);
    test(test_sha256_binary_data);
    test(test_sha256_repeated_updates);
    test(test_sha256_mid_input);
    test(test_sha256_long_input);
    END_TEST();

    return 0;
}
