// Copyright (c) 2009-2010 Satoshi Nakamoto
// Copyright (c) 2009-2016 The Bitcoin Core developers
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef ADNETCOIN_POW_H
#define ADNETCOIN_POW_H

#include "arith_uint256.h"
#include "consensus/params.h"

#include <stdint.h>

class CBlockHeader;
class CBlockIndex;
class CChainParams;
class uint256;

unsigned int GetNextWorkRequired(const CBlockIndex* pindexLast, const CBlockHeader *pblock, const Consensus::Params&);
unsigned int CalculateNextWorkRequired(arith_uint256 bnAvg, int64_t nLastBlockTime, int64_t nFirstBlockTime, const Consensus::Params& params);

unsigned int AdnetcoinGetNextWorkRequired(const CBlockIndex* pindexLast, const CBlockHeader *pblock, const Consensus::Params& params);
unsigned int AdnetcoinCalculateNextWorkRequired(const CBlockIndex* pindexLast, int64_t nFirstBlockTime, const Consensus::Params& params);

/** Check whether the Equihash solution in a block header is valid */
bool CheckEquihashSolution(const CBlockHeader *pblock, const CChainParams&);

/** Check whether a block hash satisfies the proof-of-work requirement specified by nBits */
bool CheckProofOfWork(uint256 hash, unsigned int nBits, bool postfork, const Consensus::Params&);

#endif // ADNETCOIN_POW_H
