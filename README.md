## Almunecar

> [!Warning]
> This library is a work in progress

[Almunecar](https://en.wikipedia.org/wiki/Almu%C3%B1%C3%A9car) is a cryptography library written in C, designed with simplicity in mind. It features a simple straightforward codebase and an intuitive API, making it accessible and easy to understand.

The primary goal of this library is not production use, but rather to provide clear and concise implementations of fundamental cryptographic protocols. It's an ideal resource for learning and exploring cryptographic concepts in a practical way.

## Libs

Glossary:

- ✅: Implemented
- 🚧: WIP
- 📝: Planned support

| Libs                                             | Desc                    | Status |
| ------------------------------------------------ | ----------------------- | ------ |
| [primitive-types](./libs/primitive-types/)       | BigUint & UINTs         | ✅     |
| [math](./libs/math/)                             | Math theorems and utils | ✅     |
| [hashes](./libs/hashes/)                         | SHA-256                 | ✅     |
| [digital-signature](./libs/digital-signature/) | RSA, ECDSA              | 🚧     |
| [elliptic-curves](./libs/elliptic-curves/)        | secp256k1, BLS12-381    | 📝     |
| [snarks](./libs/snarks/)                         | groth, plonk            | 📝     |
| [commitment schemes](./libs/pcs/)                | Merkle tree, FRI, KZG   | 📝     |

> [!NOTE]
> This is just an initial roadmap to define the scope of the lib. They are not definitive and may vary in the future as we explore and work.

## Installation

To install the library globally in your system run:

```shell
make install
```

To uninstall:

```shell
make uninstall
```

To install the CLI program in your system run:

```shell
make cli_install
almunecar --help
```

To uninstall:

```shell
make cli_uninstall
```

## Developers

To start developing, you'll need to compile the libs:

```shell
make build
```

Whenever you make a change on a lib, you'll need to re-build to see its effects.

To build and run the cli program:

```shell
make cli_build
./build/almunecar --help
```

To run tests:

```shell
make test
```

To run test of a specific lib:

```shell
make test_<LIB_NAME>
```

for example:

```shell
make test_primitive-types
```

See all available commands:

```shell
make help
```

---

### References

These are the main resources that guided our learning and implementations:

- [Lambdaworks](https://github.com/Lambdaclass/lambdaworks)
- [Constantine](https://github.com/mratsim/constantine)
- [Moon math](https://leastauthority.com/community-matters/moonmath-manual/)
- [An Introduction to Pairing-Based Cryptography](https://www.math.uwaterloo.ca/~ajmeneze/publications/pairings.pdf)
- [Why and How zk-SNARK Works](https://arxiv.org/pdf/1906.07221)
