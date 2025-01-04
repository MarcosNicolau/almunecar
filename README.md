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

| Libs                                       | Desc                                   | Status |
| ------------------------------------------ | -------------------------------------- | ------ |
| [primitive-types](./libs/primitive-types/) | UINT & U256                            | ✅     |
| [math](./libs/math/)                       | Utils for polynomials and modular math | 🚧     |
| [hashes](./libs/hashes/)                   | SHA2-256                               | 🚧     |
| [rsa](./libs/rsa/)                         | RSA implementation                     | 📝     |
| [elliptic-curves](./libs/eliptic-curves/)  | ECDSA & BLS12-381                      | 📝     |
| [snarks](./libs/snarks/)                   | groth & plonk                          | 📝     |
| [pcs](./libs/pcs/)                         | KZG                                    | 📝     |

> [!NOTE]
> This is just an initial roadmap to define the scope of the lib. They are not definitive and may vary in the future as we explore and work.

---

### References

These are the main resources that guided our learning and implementations:

- [Lambdaworks](https://github.com/Lambdaclass/lambdaworks)
- [Constantine](https://github.com/mratsim/constantine)
- [Moon math](https://leastauthority.com/community-matters/moonmath-manual/)
- [An Introduction to Pairing-Based Cryptography](https://www.math.uwaterloo.ca/~ajmeneze/publications/pairings.pdf)
- [Why and How zk-SNARK Works](https://arxiv.org/pdf/1906.07221)
- [Lambda sparkling water bootcamp](https://github.com/lambdaclass/sparkling_water_bootcamp/)
