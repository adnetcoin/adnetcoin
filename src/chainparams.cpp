// Copyright (c) 2010 Satoshi Nakamoto
// Copyright (c) 2009-2016 The Bitcoin Core developers
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#include "chainparams.h"
#include "consensus/merkle.h"

#include "uint256.h"
#include "arith_uint256.h"

#include "tinyformat.h"
#include "util.h"
#include "utilstrencodings.h"

// For equihash_parameters_acceptable.
#include "crypto/equihash.h"
#include "net.h"
#include "validation.h"
#define equihash_parameters_acceptable(N, K) \
    ((CBlockHeader::HEADER_SIZE + equihash_solution_size(N, K))*MAX_HEADERS_RESULTS < \
     MAX_PROTOCOL_MESSAGE_LENGTH-1000)

#include "base58.h"
#include <assert.h>
#include <boost/assign/list_of.hpp>

#include "chainparamsseeds.h"

static CBlock CreateGenesisBlock(const char* pszTimestamp, const CScript& genesisOutputScript, uint32_t nTime, uint32_t nNonce, uint32_t nBits, int32_t nVersion, const CAmount& genesisReward)
{
    CMutableTransaction txNew;
    txNew.nVersion = 1;
    txNew.vin.resize(1);
    txNew.vout.resize(1);
    txNew.vin[0].scriptSig = CScript() << 486604799 << CScriptNum(4) << std::vector<unsigned char>((const unsigned char*)pszTimestamp, (const unsigned char*)pszTimestamp + strlen(pszTimestamp));
    txNew.vout[0].nValue = genesisReward;
    txNew.vout[0].scriptPubKey = genesisOutputScript;

    CBlock genesis;
    genesis.nTime    = nTime;
    genesis.nBits    = nBits;
    genesis.nNonce   = ArithToUint256(arith_uint256(nNonce));
    genesis.nVersion = nVersion;
    genesis.vtx.push_back(MakeTransactionRef(std::move(txNew)));
    genesis.hashPrevBlock.SetNull();
    genesis.nHeight  = 0;
    genesis.hashMerkleRoot = BlockMerkleRoot(genesis);
    return genesis;
}

/**
 * Build the genesis block. Note that the output of its generation
 * transaction cannot be spent since it did not originally exist in the
 * database.
 *
 * CBlock(hash=000000000019d6, ver=1, hashPrevBlock=00000000000000, hashMerkleRoot=4a5e1e, nTime=1231006505, nBits=1d00ffff, nNonce=2083236893, vtx=1)
 *   CTransaction(hash=4a5e1e, ver=1, vin.size=1, vout.size=1, nLockTime=0)
 *     CTxIn(COutPoint(000000, -1), coinbase 04ffff001d0104455468652054696d65732030332f4a616e2f32303039204368616e63656c6c6f72206f6e206272696e6b206f66207365636f6e64206261696c6f757420666f722062616e6b73)
 *     CTxOut(nValue=50.00000000, scriptPubKey=0x5F1DF16B2B704C8A578D0B)
 *   vMerkleTree: 4a5e1e
 */
static CBlock CreateGenesisBlock(uint32_t nTime, uint32_t nNonce, uint32_t nBits, int32_t nVersion, const CAmount& genesisReward)
{
    const char* pszTimestamp = "NY Times 5/Feb/2018 Dow Jones and S.&P. Slide Again, Dropping by More Than 4%";
    const CScript genesisOutputScript = CScript() << ParseHex("04633cd76f2530c3dedd03c43e00b6d8a60f4e8097f48b8e99271c82aa53bb3b8cf93b14a732d31a52b50bdd76a031dceaee0d8c14ac71cfb94da902cab4c36541") << OP_CHECKSIG;
    return CreateGenesisBlock(pszTimestamp, genesisOutputScript, nTime, nNonce, nBits, nVersion, genesisReward);
}

void CChainParams::UpdateVersionBitsParameters(Consensus::DeploymentPos d, int64_t nStartTime, int64_t nTimeout)
{
    consensus.vDeployments[d].nStartTime = nStartTime;
    consensus.vDeployments[d].nTimeout = nTimeout;
}

/**
 * Main network
 */
/**
 * What makes a good checkpoint block?
 * + Is surrounded by blocks with reasonable timestamps
 *   (no blocks before with a timestamp after, none after with
 *    timestamp before)
 * + Contains no strange transactions
 */

const arith_uint256 maxUint = UintToArith256(uint256S("ffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffff"));


class CMainParams : public CChainParams {
public:
    CMainParams() {
        strNetworkID = "main";
        consensus.nSubsidyHalvingInterval = 100000;
        consensus.BIP34Height = 1;
        consensus.BIP34Hash = uint256S("0x00000151b2d76ba5719f789f71f53f8bcc7391fee0f2c6e1f5ab73ac69bf1813");
        consensus.BIP65Height = 2;
        consensus.BIP66Height = 3;
        consensus.ADNETHeight = 4;
        consensus.ADNETPremineWindow = 0;
        consensus.ADNETPremineEnforceWhitelist = true;
        consensus.powLimit = uint256S("0007ffffffffffffffffffffffffffffffffffffffffffffffffffffffffffff");
        consensus.powLimitStart = uint256S("0007ffffffffffffffffffffffffffffffffffffffffffffffffffffffffffff");
        consensus.powLimitLegacy = uint256S("0007ffffffffffffffffffffffffffffffffffffffffffffffffffffffffffff");
        
        //based on https://github.com/ADNETGPU/ADNETGPU/issues/78
        consensus.nPowAveragingWindow = 30;
        assert(maxUint/UintToArith256(consensus.powLimit) >= consensus.nPowAveragingWindow);
        consensus.nPowMaxAdjustDown = 32;
        consensus.nPowMaxAdjustUp = 16;
        
        consensus.nPowTargetTimespanLegacy = 24 * 60 * 60;
		consensus.nPowTargetSpacing = 2 * 60;
		consensus.fPowAllowMinDifficultyBlocks = false;
		consensus.fPowNoRetargeting = false;
		consensus.nRuleChangeActivationThreshold = 684; // 95% of 720
		consensus.nMinerConfirmationWindow = 720; // nPowTargetTimespanLegacy / nPowTargetSpacing
        consensus.vDeployments[Consensus::DEPLOYMENT_TESTDUMMY].bit = 28;
        consensus.vDeployments[Consensus::DEPLOYMENT_TESTDUMMY].nStartTime = 1199145601; // January 1, 2008
        consensus.vDeployments[Consensus::DEPLOYMENT_TESTDUMMY].nTimeout = 1230767999; // December 31, 2008

        // Deployment of BIP68, BIP112, and BIP113.
        consensus.vDeployments[Consensus::DEPLOYMENT_CSV].bit = 0;
        consensus.vDeployments[Consensus::DEPLOYMENT_CSV].nStartTime = 1462060800; // May 1st, 2016
        consensus.vDeployments[Consensus::DEPLOYMENT_CSV].nTimeout = 1493596800; // May 1st, 2017

        // Deployment of SegWit (BIP141, BIP143, and BIP147)
        consensus.vDeployments[Consensus::DEPLOYMENT_SEGWIT].bit = 1;
        consensus.vDeployments[Consensus::DEPLOYMENT_SEGWIT].nStartTime = 1479168000; // November 15th, 2016.
        consensus.vDeployments[Consensus::DEPLOYMENT_SEGWIT].nTimeout = 1510704000; // November 15th, 2017.

        // The best chain should have at least this much work.
        consensus.nMinimumChainWork = uint256S("0x0000000000000000000000000000000000000000000000000000004657dcdca7");

        // By default assume that the signatures in ancestors of this block are valid.
        consensus.defaultAssumeValid = uint256S("0x00000329d07a4b5d70712ae8b29d26eaf5c92b2e2eec61d32a4d06e928d8195f"); //104857

        /**
         * The message start string is designed to be unlikely to occur in normal data.
         * The characters are rarely used upper ASCII, not valid as UTF-8, and produce
         * a large 32-bit integer with any alignment.
         */
        pchMessageStart[0] = 0xd2;
		pchMessageStart[1] = 0x56;
		pchMessageStart[2] = 0x90;
		pchMessageStart[3] = 0xa2;
		nDefaultPort = 4222; // different port than Adnetcoin
        nPruneAfterHeight = 100000;
        const size_t N = 200, K = 9;
        BOOST_STATIC_ASSERT(equihash_parameters_acceptable(N, K));
        nEquihashN = N;
        nEquihashK = K;

        genesis = CreateGenesisBlock(1517897827, 960379, 0x1e0ffff0, 1, 100 * COIN);
        consensus.hashGenesisBlock = genesis.GetHash(consensus);
        assert(consensus.hashGenesisBlock == uint256S("0x00000dec1e86dc7205e169c5e24eeb1993e54ae989b134483bf53abe1c557850"));
        assert(genesis.hashMerkleRoot == uint256S("0x86f8a12f6f16bb153763d477bdb1868429c5e21f7c3990fbc7eb8c01479627e2"));

        vFixedSeeds.clear();
        vSeeds.clear();
        // nodes with support for servicebits filtering should be at the top

        vSeeds.emplace_back("seed.adnetcoin.cash", true);
        vSeeds.emplace_back("seed.minergategpu.org", true);

        base58Prefixes[PUBKEY_ADDRESS] = std::vector<unsigned char>(1,23);  // prefix: A
		base58Prefixes[SCRIPT_ADDRESS] = std::vector<unsigned char>(1,30);  // prefix: D
		base58Prefixes[SECRET_KEY] =     std::vector<unsigned char>(1,113);
		base58Prefixes[EXT_PUBLIC_KEY] = {0x02, 0x66, 0xB0, 0x1C};
		base58Prefixes[EXT_SECRET_KEY] = {0x02, 0x66, 0xAB, 0xE2};

        vFixedSeeds = std::vector<SeedSpec6>(pnSeed6_main, pnSeed6_main + ARRAYLEN(pnSeed6_main));

        fDefaultConsistencyChecks = false;
        fRequireStandard = true;
        fMineBlocksOnDemand = false;

        checkpointData = (CCheckpointData) {
			{
				{ 100, uint256S("0x0002c2535205de0e81f12229e6240f498cd3febcad75da050177d919c3b93656")},
				{ 200, uint256S("0x00001530413db6172d649bb24894c4db05baaa2b60acd2701e1cccc575843712")},
				{ 300, uint256S("0x000040d4f999aa515f8c01b51fae570ad220b0bcf98a13c23b0e06aea656eec4")},
				{ 400, uint256S("0x00002123a56a5e0873b0c09b1fc3510579b29d8ba18c6455d59e049b40ac22f2")},
				{ 9949, uint256S("0x000002db247ccb12d50325fabb5376308be2e591ecf2a470f01f39fb6a6e3eea")},
				{ 9958, uint256S("0x00000064f2570d7536bee71a831bae555e206ffb90bc9264f2078a399e39b4a0")},
				{ 9984, uint256S("0x0000064cf832bf9d513a0ed12c029e6fa6885a17601680537da2250c437ebba6")},
				{ 30000, uint256S("0x0000003c047a4e3f044a4f1205953a70027cdffcbc1ee397d8be03dfdfe2f370")},
				{ 60000, uint256S("0x00000414854cf014ac4e1f2b39acfbeec44e76aca0440e293095773e7d253213")},
				{ 100000, uint256S("0x000003b4bf1cbf8482a8d54a63006ffb41567bbdc58f8ebd2f63cecb464f9791")},
			}
		};


		chainTxData = ChainTxData{
			// Data as of block 000000000000000000d97e53664d17967bd4ee50b23abb92e54a34eb222d15ae (height 478913).
			1530443633, // * UNIX timestamp of last known number of transactions
			130919,  // * total number of transactions between genesis and that timestamp
						//   (the tx=... number in the SetBestChain debug.log lines)
			0.01         // * estimated number of transactions per second after that timestamp
		};

		vPreminePubkeys = {};
    }
};

/**
 * Testnet (v3)
 */
class CTestNetParams : public CChainParams {
public:
    CTestNetParams() {
        strNetworkID = "test";
        consensus.nSubsidyHalvingInterval = 100000;
        consensus.BIP34Height = -1;
        consensus.BIP34Hash = uint256S("0x0");
        consensus.BIP65Height = -1; // 00000000007f6655f22f98e72ed80d8b06dc761d5da09df0fa1dc4be4f861eb6
        consensus.BIP66Height = -1; // 000000002104c8c45e99a8853285a3b592602a3ccde2b832481da85e9e4ba182
        consensus.ADNETHeight = 1;
        consensus.ADNETPremineWindow = 0;
        consensus.ADNETPremineEnforceWhitelist = false;
        consensus.powLimit = uint256S("0007ffffffffffffffffffffffffffffffffffffffffffffffffffffffffffff");
        consensus.powLimitStart = uint256S("0007ffffffffffffffffffffffffffffffffffffffffffffffffffffffffffff");
        consensus.powLimitLegacy = uint256S("0007ffffffffffffffffffffffffffffffffffffffffffffffffffffffffffff");

        //based on https://github.com/ADNETGPU/ADNETGPU/issues/78
        consensus.nPowAveragingWindow = 30;
        assert(maxUint/UintToArith256(consensus.powLimit) >= consensus.nPowAveragingWindow);
        consensus.nPowMaxAdjustDown = 32;
        consensus.nPowMaxAdjustUp = 16;
        
        consensus.nPowTargetTimespanLegacy = 24 * 60 * 60; // two weeks
        consensus.nPowTargetSpacing = 2 * 60;
        consensus.fPowAllowMinDifficultyBlocks = true;
        consensus.fPowNoRetargeting = false;
        consensus.nRuleChangeActivationThreshold = 540; // 75% for testchains
        consensus.nMinerConfirmationWindow = 720; // nPowTargetTimespanLegacy / nPowTargetSpacing
        consensus.vDeployments[Consensus::DEPLOYMENT_TESTDUMMY].bit = 28;
        consensus.vDeployments[Consensus::DEPLOYMENT_TESTDUMMY].nStartTime = 1199145601; // January 1, 2008
        consensus.vDeployments[Consensus::DEPLOYMENT_TESTDUMMY].nTimeout = 1230767999; // December 31, 2008

        // Deployment of BIP68, BIP112, and BIP113.
        consensus.vDeployments[Consensus::DEPLOYMENT_CSV].bit = 0;
        consensus.vDeployments[Consensus::DEPLOYMENT_CSV].nStartTime = 1456790400; // March 1st, 2016
        consensus.vDeployments[Consensus::DEPLOYMENT_CSV].nTimeout = 1493596800; // May 1st, 2017

        // Deployment of SegWit (BIP141, BIP143, and BIP147)
        consensus.vDeployments[Consensus::DEPLOYMENT_SEGWIT].bit = 1;
        consensus.vDeployments[Consensus::DEPLOYMENT_SEGWIT].nStartTime = 1462060800; // May 1st 2016
        consensus.vDeployments[Consensus::DEPLOYMENT_SEGWIT].nTimeout = 1493596800; // May 1st 2017

        // The best chain should have at least this much work.
        consensus.nMinimumChainWork = uint256S("0x0");

        // By default assume that the signatures in ancestors of this block are valid.
        consensus.defaultAssumeValid = uint256S("0x0"); //1135275

        pchMessageStart[0] = 0xd2;
        pchMessageStart[1] = 0x56;
        pchMessageStart[2] = 0x5d;
        pchMessageStart[3] = 0x32;
        nDefaultPort = 14222;
        nPruneAfterHeight = 1000;
        const size_t N = 200, K = 9;  // Same as mainchain.
        BOOST_STATIC_ASSERT(equihash_parameters_acceptable(N, K));
        nEquihashN = N;
        nEquihashK = K;

        genesis = CreateGenesisBlock(1530446351, 1991623, 0x1e0ffff0, 1, 100 * COIN);
        consensus.hashGenesisBlock = genesis.GetHash(consensus);
        assert(consensus.hashGenesisBlock == uint256S("0x000001bca72f7c9110b9ed5e9e8f50a2953e980a189e8dbf18edb609aae5cdd0"));
        assert(genesis.hashMerkleRoot == uint256S("0x86f8a12f6f16bb153763d477bdb1868429c5e21f7c3990fbc7eb8c01479627e2"));

        vFixedSeeds.clear();
        vSeeds.clear();
        // nodes with support for servicebits filtering should be at the top

        vSeeds.emplace_back("test-seed.adnetcoin.cash", true);

        base58Prefixes[PUBKEY_ADDRESS] = std::vector<unsigned char>(1,111);
        base58Prefixes[SCRIPT_ADDRESS] = std::vector<unsigned char>(1,196);
        base58Prefixes[SECRET_KEY] =     std::vector<unsigned char>(1,239);
        base58Prefixes[EXT_PUBLIC_KEY] = {0x04, 0x35, 0x87, 0xCF};
        base58Prefixes[EXT_SECRET_KEY] = {0x04, 0x35, 0x83, 0x94};

        vFixedSeeds = std::vector<SeedSpec6>(pnSeed6_test, pnSeed6_test + ARRAYLEN(pnSeed6_test));

        fDefaultConsistencyChecks = false;
        fRequireStandard = false;
        fMineBlocksOnDemand = false;


        checkpointData = (CCheckpointData) {
            {
                {0, uint256S("000001bca72f7c9110b9ed5e9e8f50a2953e980a189e8dbf18edb609aae5cdd0")},
            }
        };

        chainTxData = ChainTxData{
            // Data as of block 00000000000001c200b9790dc637d3bb141fe77d155b966ed775b17e109f7c6c (height 1156179)
            0,
            0,
            0
        };
    }
};

/**
 * Regression test
 */
class CRegTestParams : public CChainParams {
public:
    CRegTestParams() {
        strNetworkID = "regtest";
        consensus.nSubsidyHalvingInterval = 150;
        consensus.BIP34Height = 100000000; // BIP34 has not activated on regtest (far in the future so block v1 are not rejected in tests)
        consensus.BIP34Hash = uint256();
        consensus.BIP65Height = 1351; // BIP65 activated on regtest (Used in rpc activation tests)
        consensus.BIP66Height = 1251; // BIP66 activated on regtest (Used in rpc activation tests)
        consensus.ADNETHeight = 3000;
        consensus.ADNETPremineWindow = 10;
        consensus.ADNETPremineEnforceWhitelist = false;
        consensus.powLimit = uint256S("7fffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffff");
        consensus.powLimitStart = uint256S("7fffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffff");
        consensus.powLimitLegacy = uint256S("7fffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffff");
        //based on https://github.com/ADNETGPU/ADNETGPU/issues/78
        consensus.nPowAveragingWindow = 30;
        consensus.nPowMaxAdjustDown = 16;
        consensus.nPowMaxAdjustUp = 32;
        consensus.nPowTargetTimespanLegacy = 14 * 24 * 60 * 60; // two weeks
        consensus.nPowTargetSpacing = 10 * 60;
        consensus.fPowAllowMinDifficultyBlocks = true;
        consensus.fPowNoRetargeting = true;
        consensus.nRuleChangeActivationThreshold = 108; // 75% for testchains
        consensus.nMinerConfirmationWindow = 144; // Faster than normal for regtest (144 instead of 2016)
        consensus.vDeployments[Consensus::DEPLOYMENT_TESTDUMMY].bit = 28;
        consensus.vDeployments[Consensus::DEPLOYMENT_TESTDUMMY].nStartTime = 0;
        consensus.vDeployments[Consensus::DEPLOYMENT_TESTDUMMY].nTimeout = 999999999999ULL;
        consensus.vDeployments[Consensus::DEPLOYMENT_CSV].bit = 0;
        consensus.vDeployments[Consensus::DEPLOYMENT_CSV].nStartTime = 0;
        consensus.vDeployments[Consensus::DEPLOYMENT_CSV].nTimeout = 999999999999ULL;
        consensus.vDeployments[Consensus::DEPLOYMENT_SEGWIT].bit = 1;
        consensus.vDeployments[Consensus::DEPLOYMENT_SEGWIT].nStartTime = 0;
        consensus.vDeployments[Consensus::DEPLOYMENT_SEGWIT].nTimeout = 999999999999ULL;

        // The best chain should have at least this much work.
        consensus.nMinimumChainWork = uint256S("0x00");

        // By default assume that the signatures in ancestors of this block are valid.
        consensus.defaultAssumeValid = uint256S("0x00");
        
        pchMessageStart[0] = 0xfa;
        pchMessageStart[1] = 0xbf;
        pchMessageStart[2] = 0xb5;
        pchMessageStart[3] = 0xda;

        nDefaultPort = 18444;
        nPruneAfterHeight = 1000;
        const size_t N = 48, K = 5;
        BOOST_STATIC_ASSERT(equihash_parameters_acceptable(N, K));
        nEquihashN = N;
        nEquihashK = K;

        genesis = CreateGenesisBlock(1296688602, 2, 0x207fffff, 1, 50 * COIN);
        consensus.hashGenesisBlock = genesis.GetHash(consensus);
        assert(consensus.hashGenesisBlock == uint256S("0x0f9188f13cb7b2c71f2a335e3a4fc328bf5beb436012afca590b1a11466e2206"));
        assert(genesis.hashMerkleRoot == uint256S("0x4a5e1e4baab89f3a32518a88c31bc87f618f76673e2cc77ab2127b7afdeda33b"));

        vFixedSeeds.clear(); //!< Regtest mode doesn't have any fixed seeds.
        vSeeds.clear();      //!< Regtest mode doesn't have any DNS seeds.

        fDefaultConsistencyChecks = true;
        fRequireStandard = false;
        fMineBlocksOnDemand = true;

        checkpointData = (CCheckpointData) {
            {
                {0, uint256S("0f9188f13cb7b2c71f2a335e3a4fc328bf5beb436012afca590b1a11466e2206")},
            }
        };

        chainTxData = ChainTxData{
            0,
            0,
            0
        };

        base58Prefixes[PUBKEY_ADDRESS] = std::vector<unsigned char>(1,111);
        base58Prefixes[SCRIPT_ADDRESS] = std::vector<unsigned char>(1,196);
        base58Prefixes[SECRET_KEY] =     std::vector<unsigned char>(1,239);
        base58Prefixes[EXT_PUBLIC_KEY] = {0x04, 0x35, 0x87, 0xCF};
        base58Prefixes[EXT_SECRET_KEY] = {0x04, 0x35, 0x83, 0x94};
    }
    
};

class AdnetcoinAddressChainParam : public CMainParams
{
public:
    AdnetcoinAddressChainParam()
    {
        base58Prefixes[PUBKEY_ADDRESS] = std::vector<unsigned char>(1,0);
        base58Prefixes[SCRIPT_ADDRESS] = std::vector<unsigned char>(1,5);
    }
};

static std::unique_ptr<CChainParams> globalChainParams;
static AdnetcoinAddressChainParam chainParamsForAddressConversion;

const CChainParams &Params()
{
    assert(globalChainParams);
    return *globalChainParams;
}

const CChainParams &AdnetcoinAddressFormatParams()
{
    return chainParamsForAddressConversion;
}

std::unique_ptr<CChainParams> CreateChainParams(const std::string& chain)
{
    if (chain == CBaseChainParams::MAIN)
        return std::unique_ptr<CChainParams>(new CMainParams());
    else if (chain == CBaseChainParams::TESTNET)
        return std::unique_ptr<CChainParams>(new CTestNetParams());
    else if (chain == CBaseChainParams::REGTEST)
        return std::unique_ptr<CChainParams>(new CRegTestParams());
    throw std::runtime_error(strprintf("%s: Unknown chain %s.", __func__, chain));
}

void SelectParams(const std::string& network)
{
    SelectBaseParams(network);
    globalChainParams = CreateChainParams(network);
}

void UpdateVersionBitsParameters(Consensus::DeploymentPos d, int64_t nStartTime, int64_t nTimeout)
{
    globalChainParams->UpdateVersionBitsParameters(d, nStartTime, nTimeout);
}


static CScript CltvMultiSigScript(const std::vector<std::string>& pubkeys, uint32_t lock_time) {
    assert(pubkeys.size() == 6);
    CScript redeem_script;
    if (lock_time > 0) {
        redeem_script << lock_time << OP_CHECKLOCKTIMEVERIFY << OP_DROP;
    }
    redeem_script << 4;
    for (const std::string& pubkey : pubkeys) {
        redeem_script << ToByteVector(ParseHex(pubkey));
    }
    redeem_script << 6 << OP_CHECKMULTISIG;
    return redeem_script;
}

bool CChainParams::IsPremineAddressScript(const CScript& scriptPubKey, uint32_t height) const {
    static const int LOCK_TIME = 3 * 365 * 24 * 3600;  // 3 years
    static const int LOCK_STAGES = 3 * 12;  // Every month for 3 years
    assert((uint32_t)consensus.ADNETHeight <= height &&
           height < (uint32_t)(consensus.ADNETHeight + consensus.ADNETPremineWindow));
    int block = height - consensus.ADNETHeight;
    int num_unlocked = consensus.ADNETPremineWindow * 40 / 100;  // 40% unlocked.
    int num_locked = consensus.ADNETPremineWindow - num_unlocked;  // 60% time-locked.
    int stage_lock_time = LOCK_TIME / LOCK_STAGES / consensus.nPowTargetSpacing;
    int stage_block_height = num_locked / LOCK_STAGES;
    const std::vector<std::string> pubkeys = vPreminePubkeys[block % vPreminePubkeys.size()];  // Round robin.
    CScript redeem_script;
    if (block < num_unlocked) {
        redeem_script = CltvMultiSigScript(pubkeys, 0);
    } else {
        int locked_block = block - num_unlocked;
        int stage = locked_block / stage_block_height;
        int lock_time = consensus.ADNETHeight + stage_lock_time * (1 + stage);
        redeem_script = CltvMultiSigScript(pubkeys, lock_time);
    }
    CScript target_scriptPubkey = GetScriptForDestination(CScriptID(redeem_script));
    return scriptPubKey == target_scriptPubkey;
}
