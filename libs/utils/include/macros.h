
#define CONCAT_INNER(s1, s2) s1##s2
#define CONCAT(s1, s2) CONCAT_INNER(s1, s2)

// https://stackoverflow.com/questions/10379691/creating-macro-using-line-for-different-variable-names
#define ANONYMOUS_VARIABLE(str) CONCAT(str, __LINE__)
