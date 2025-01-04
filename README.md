## Almunecar

> [!Warning]
> This library is a work in progress

Cryptography library in c that aims to have a very simple codebase easy to understand with an intuitive API. The goal of this library is not to be used at prod environments but provide implementations for the most basic crypto protocols in the simplest way for educational purposes.

### Libs

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
> This is just an initial roadmap to define the scope of the lib. They are not definitive and may vary in the future and as we explore.

---

### References

These are the main resources that guided our learning and implementations:

- [Lambdaworks](https://github.com/Lambdaclass/lambdaworks)
- [Constantine](https://github.com/mratsim/constantine)
- [Moon math](https://leastauthority.com/community-matters/moonmath-manual/)
- [An Introduction to Pairing-Based Cryptography](https://www.math.uwaterloo.ca/~ajmeneze/publications/pairings.pdf)
- [Why and How zk-SNARK Works](https://arxiv.org/pdf/1906.07221)
- [Lambda sparkling water bootcamp](https://github.com/lambdaclass/sparkling_water_bootcamp/)
