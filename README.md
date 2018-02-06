# Adnetcoin

[![Build Status](https://travis-ci.org/adnetcoin/adnetcoin.svg?branch=master)](https://travis-ci.org/adnetcoin/adnetcoin)

Adnetcoin (codename adnetcoin) began as a fork of Bitcoin after block height 491406 on Tue, 24 Oct 2017 01:17:35 GMT and began being mined as a separate chain at block height 491407 on Sun, 12 Nov 2017 13:34:01 GMT.

The primary goal is to maintain a variant of Bitcoin that is more decentralized by using an ASIC-resistant Proof of Work algorithm, removing barriers to entry for new miners around the world and reducing the concentration of power in the hands of massive-scale mining operations.

Adnetcoin preserves and implements Bitcoin features such as SegWit. Significant differences at launch time included:

- ASIC-resistant GPU-minable PoW algorithm (Equihash)
- Per-block difficulty adjustment algorithm
- Replay protection (SIGHASH_FORK_ID)
- Unique wallet addresses (prefixes of G and A)

Although ADNET was bootstrapped on 12 Nov to create an entirely new network, it contains the entire Bitcoin blockchain until block 491406. As a result, Adnetcoin became a full fork with all Bitcoin transaction history since 2009. Any Bitcoin wallet address which held Bitcoin in BTC block 491406 before the fork held an equal number of Adnetcoin in ADNET block 491407 after the fork.

## Links

* Website: https://adnetcoin.org
* Tech Spec: https://github.com/adnetcoin/adnetcoin/wiki/Technical-Spec
* Forum: https://forum.adnetcoin.org
* Discord: [invitation](https://discord.gg/HmVUU6S)
