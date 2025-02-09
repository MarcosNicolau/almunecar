#ifndef TYPES_H
#define TYPES_H

// Result like return type like rust
// https://gist.github.com/f0rki/9b2c2b73d46ccdad2b39ab79b3a5517f
#define DEFINE_RESULT(T, E, NAME)                                                                                      \
    typedef struct {                                                                                                   \
        int success : 1;                                                                                               \
        union {                                                                                                        \
            T result;                                                                                                  \
            E error;                                                                                                   \
        };                                                                                                             \
    } NAME;

#define Err(NAME, E)                                                                                                   \
    (NAME) { .success = 0, .error = E }

#define Ok(NAME, R)                                                                                                    \
    (NAME) { .success = 1, .result = R }

#endif
