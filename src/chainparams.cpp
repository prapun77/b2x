// Copyright (c) 2010 Satoshi Nakamoto
// Copyright (c) 2009-2016 The Bitcoin Core developers
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#include "chainparams.h"
#include "consensus/merkle.h"

#include "tinyformat.h"
#include "util.h"
#include "utilstrencodings.h"

#include <assert.h>

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
    genesis.nNonce   = nNonce;
    genesis.nVersion = nVersion;
    genesis.vtx.push_back(MakeTransactionRef(std::move(txNew)));
    genesis.hashPrevBlock.SetNull();
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
    const char* pszTimestamp = "The Time 28/07/2018  His Majesty King Maha Vajiralongkorn Bodindradebayavarangkun";
    const CScript genesisOutputScript = CScript() << ParseHex("04fe06a25ca1e7a2b91008c9c42aef0fe447509fbb2b133ad2cd421ce7d9a204a1be63b5d4f7dd12ec372b03bfb3d9c1105f45cee37da0a035d875f732743dc038") << OP_CHECKSIG;
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

class CMainParams : public CChainParams {
public:
    CMainParams() {
        strNetworkID = "main";
        consensus.nSubsidyHalvingInterval = 210000;
        consensus.BIP34Height = 227931;
        consensus.BIP34Hash = uint256S("0x000000000000024b89b42a942fe0d9fea3bb44ab7bd1b19115dd6a759c0808b8");
        consensus.BIP65Height = 388381; // 000000000000000004c2b624ed5d7756c508d90fd0da2c7c679febfa6c4735f0
        consensus.BIP66Height = 363725; // 00000000000000000379eaa19dce8c9b722d46ae6a57c2f1a988119488b50931
        consensus.powLimit = uint256S("00000000ffffffffffffffffffffffffffffffffffffffffffffffffffffffff");
        consensus.posLimit = uint256S("00ffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffff");
        consensus.nPowTargetTimespan = 60; // two weeks
        consensus.nPowTargetSpacing =  15;
        consensus.nPosTargetSpacing = 60;
        consensus.nBtcPowTargetSpacing = 10 * 60;
        consensus.fPowAllowMinDifficultyBlocks = false;
        consensus.fPowNoRetargeting = false;
        consensus.nRuleChangeActivationThreshold = 108; // 95% of 2016
        consensus.nMinerConfirmationWindow = 144; // nPowTargetTimespan / nPowTargetSpacing
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
        
        consensus.vDeployments[Consensus::HARDFORK_BITCOINX].bit = 27;
        consensus.vDeployments[Consensus::HARDFORK_BITCOINX].nStartTime = 999999999999ULL;
        consensus.vDeployments[Consensus::HARDFORK_BITCOINX].nTimeout = 999999999999ULL; 

        consensus.vDeployments[Consensus::HARDFORK_POS].bit = 26;
        consensus.vDeployments[Consensus::HARDFORK_POS].nStartTime = 999999999999ULL;
        consensus.vDeployments[Consensus::HARDFORK_POS].nTimeout = 999999999999ULL; 

        // The best chain should have at least this much work.
        consensus.nMinimumChainWork = uint256S("0x0000000000000000000000000000000000000000000000000000000000000000000000");

        // By default assume that the signatures in ancestors of this block are valid.
        consensus.defaultAssumeValid = uint256S("00000000026fce835e9f517ccaa0f8ed8295716f0c4dcd06450f4f5b74f57fe7"); //501451

        consensus.hardforkHeight = 0;
        consensus.premineAddress = "18wFZB62chDhYdapBnVEXX4y9hUvWs8yRu";
        consensus.premineValue = 2000000 * COIN;

        consensus.posHeight      = 999999999;
        consensus.fidShiftHeight = 999999999;

        /**
         * The message start string is designed to be unlikely to occur in normal data.
         * The characters are rarely used upper ASCII, not valid as UTF-8, and produce
         * a large 32-bit integer with any alignment.
         */
        pchBitcoinMessageStart[0] = 0xf8;
        pchBitcoinMessageStart[1] = 0xbc;
        pchBitcoinMessageStart[2] = 0xb3;
        pchBitcoinMessageStart[3] = 0xd8;

        pchMessageStart[0] = 0xf5;
        pchMessageStart[1] = 0xb3;
        pchMessageStart[2] = 0xb6;
        pchMessageStart[3] = 0xd9;

        nDefaultPort = 8998;
        nPruneAfterHeight = 100000;

        genesis = CreateGenesisBlock(1533212823,  5588306, 0x1e0ffff0, 1, 50 * COIN);
        consensus.hashGenesisBlock = genesis.GetHash();
        if (false)
        {
            consensus.hashGenesisBlock = uint256S("0x00000f3e2c2677f55df3af6fa595b64c1aab05a54e3433d0215f4e64fdbf775c");
            LogPrintf("recalculating params for mainnet.\n");
            LogPrintf("old mainnet genesis nonce: %d\n", genesis.nNonce);
            LogPrintf("old mainnet genesis hash:  %s\n", consensus.hashGenesisBlock.ToString().c_str());
            // deliberately empty for loop finds nonce value.
            for(genesis.nNonce = 0; genesis.GetHash() > (~uint256S(0) >> 20); genesis.nNonce++){ }
            LogPrintf("new mainnet genesis merkle root: %s\n", genesis.hashMerkleRoot.ToString().c_str());
            LogPrintf("new mainnet genesis nonce: %d\n", genesis.nNonce);
            LogPrintf("new mainnet genesis hash: %s\n", genesis.GetHash().ToString().c_str());
        }

        assert(consensus.hashGenesisBlock == uint256S("0x00000f3e2c2677f55df3af6fa595b64c1aab05a54e3433d0215f4e64fdbf775c"));
        assert(genesis.hashMerkleRoot == uint256S("0x1f38dcfad187198907c360e569afcbc359c025cef3a4d6cf37b5ab4c3a552597"));

        // Note that of those with the service bits flag, most only support a subset of possible options
        vSeeds.emplace_back("node1.b2x-segwit.io", false);
        vSeeds.emplace_back("node2.b2x-segwit.io", false);
        vSeeds.emplace_back("node3.b2x-segwit.io", false);

        base58Prefixes[PUBKEY_ADDRESS] = std::vector<unsigned char>(1,23);
        base58Prefixes[SCRIPT_ADDRESS] = std::vector<unsigned char>(1,59);
        base58Prefixes[SECRET_KEY] =     std::vector<unsigned char>(1,124);
        base58Prefixes[EXT_PUBLIC_KEY] = {0x14, 0x58, 0xB2, 0x1E};
        base58Prefixes[EXT_SECRET_KEY] = {0x14, 0x58, 0xAD, 0xE4};

     //   vFixedSeeds = std::vector<SeedSpec6>(pnSeed6_main, pnSeed6_main + ARRAYLEN(pnSeed6_main));

        fDefaultConsistencyChecks = false;
        fRequireStandard = true;
        fMineBlocksOnDemand = true;

        checkpointData = (CCheckpointData) {
            {
                { 0, uint256S("0x00000f3e2c2677f55df3af6fa595b64c1aab05a54e3433d0215f4e64fdbf775c")},
                  }
        };

        chainTxData = ChainTxData{
            // Data as of block 000000000000000000d97e53664d17967bd4ee50b23abb92e54a34eb222d15ae (height 478913).
            1533212823, // * UNIX timestamp of last known number of transactions
            0,  // * total number of transactions between genesis and that timestamp
                        //   (the tx=... number in the SetBestChain debug.log lines)
            0         // * estimated number of transactions per second after that timestamp
        };
    }
};

/**
 * Testnet (v3)
 */
class CTestNetParams : public CChainParams {
public:
    CTestNetParams() {
        strNetworkID = "test";
        consensus.nSubsidyHalvingInterval = 210000;
        consensus.BIP34Height = 21111;
        consensus.BIP34Hash = uint256S("0x0000000023b3a96d3484e5abb3755c413e7d41500f8e2a5c3f0dd01299cd8ef8");
        consensus.BIP65Height = 581885; // 00000000007f6655f22f98e72ed80d8b06dc761d5da09df0fa1dc4be4f861eb6
        consensus.BIP66Height = 330776; // 000000002104c8c45e99a8853285a3b592602a3ccde2b832481da85e9e4ba182
        consensus.powLimit = uint256S("00000000ffffffffffffffffffffffffffffffffffffffffffffffffffffffff");
        consensus.posLimit = uint256S("00ffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffff");
        consensus.nPowTargetTimespan = 14 * 24 * 60 * 60; // two weeks
        consensus.nPowTargetSpacing = 2.5 * 60;
        consensus.nPosTargetSpacing = 60;
        consensus.nBtcPowTargetSpacing = 10 * 60;
        consensus.fPowAllowMinDifficultyBlocks = true;
        consensus.fPowNoRetargeting = false;
        consensus.nRuleChangeActivationThreshold = 1512; // 75% for testchains
        consensus.nMinerConfirmationWindow = 2016; // nPowTargetTimespan / nPowTargetSpacing
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

        consensus.vDeployments[Consensus::HARDFORK_BITCOINX].bit = 27;
        consensus.vDeployments[Consensus::HARDFORK_BITCOINX].nStartTime = 999999999999ULL;
        consensus.vDeployments[Consensus::HARDFORK_BITCOINX].nTimeout = 999999999999ULL; 

        consensus.vDeployments[Consensus::HARDFORK_POS].bit = 26;
        consensus.vDeployments[Consensus::HARDFORK_POS].nStartTime = 999999999999ULL;
        consensus.vDeployments[Consensus::HARDFORK_POS].nTimeout = 999999999999ULL; 

        // The best chain should have at least this much work.
        consensus.nMinimumChainWork = uint256S("0x0000000000000000000000000000000000000000000000000000000000000000");

        // By default assume that the signatures in ancestors of this block are valid.
        consensus.defaultAssumeValid = uint256S("0x0000000002e9e7b00e1f6dc5123a04aad68dd0f0968d8c7aa45f6640795c37b1"); //1135275

        consensus.hardforkHeight = 1200000;
        consensus.premineAddress = "mh7CayYx3J8ofEU5zSZPqJJV65d8ijkJpm";
        consensus.premineValue = 2000000 * COIN;

        consensus.posHeight = 1200120;
        consensus.fidShiftHeight = 1200120;

        pchBitcoinMessageStart[0] = 0x0a;
        pchBitcoinMessageStart[1] = 0x13;
        pchBitcoinMessageStart[2] = 0x08;
        pchBitcoinMessageStart[3] = 0x09;

        pchMessageStart[0] = 0xf5;
        pchMessageStart[1] = 0xb4;
        pchMessageStart[2] = 0xb2;
        pchMessageStart[3] = 0xd9;

        nDefaultPort = 18333;
        nPruneAfterHeight = 1000;

        genesis = CreateGenesisBlock(1533214813 , 865474, 0x1e0ffff0, 1, 50 * COIN);
        consensus.hashGenesisBlock = genesis.GetHash();
        assert(consensus.hashGenesisBlock == uint256S("0x0000024ba250dee43b8d3476ed6fda1045f4e3873af4b9ef6658a0f4ed7176e7"));
        assert(genesis.hashMerkleRoot == uint256S("0x1f38dcfad187198907c360e569afcbc359c025cef3a4d6cf37b5ab4c3a552597"));

        vFixedSeeds.clear();
        vSeeds.clear();
        // nodes with support for servicebits filtering should be at the top
//        vSeeds.emplace_back("node1.b2x-segwit.io", true);
  //      vSeeds.emplace_back("node2.b2x-segwit.io", true);
    //    vSeeds.emplace_back("node3.b2x-segwit.io", true);

        base58Prefixes[PUBKEY_ADDRESS] = std::vector<unsigned char>(1,121);
        base58Prefixes[SCRIPT_ADDRESS] = std::vector<unsigned char>(1,196);
        base58Prefixes[SECRET_KEY] =     std::vector<unsigned char>(1,248);
        base58Prefixes[EXT_PUBLIC_KEY] = {0x08, 0x39, 0x87, 0xCF};
        base58Prefixes[EXT_SECRET_KEY] = {0x08, 0x39, 0x83, 0x94};

        vFixedSeeds = std::vector<SeedSpec6>(pnSeed6_test, pnSeed6_test + ARRAYLEN(pnSeed6_test));

        fDefaultConsistencyChecks = false;
        fRequireStandard = false;
        fMineBlocksOnDemand = false;


        // checkpointData = (CCheckpointData) {
        //     {
        //         {546, uint256S("000000002a936ca763904c3c35fce2f3556c559c0214345d31b1bcebf76acb70")},
        //     }
        // };

        chainTxData = ChainTxData{
            // Data as of block 00000000000001c200b9790dc637d3bb141fe77d155b966ed775b17e109f7c6c (height 1156179)
            1533214813,
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
        consensus.powLimit = uint256S("7fffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffff");
        consensus.posLimit = uint256S("7fffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffff");
        consensus.nPowTargetTimespan = 14 * 24 * 60 * 60; // two weeks
        consensus.nPowTargetSpacing = 2.5 * 60;
        consensus.nPosTargetSpacing = consensus.nPowTargetSpacing * 9 + 60;
        consensus.nBtcPowTargetSpacing = 10 * 60;
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

        consensus.vDeployments[Consensus::HARDFORK_BITCOINX].bit = 27;
        consensus.vDeployments[Consensus::HARDFORK_BITCOINX].nStartTime = 0;
        consensus.vDeployments[Consensus::HARDFORK_BITCOINX].nTimeout = 999999999999ULL;
        
        consensus.vDeployments[Consensus::HARDFORK_POS].bit = 26;
        consensus.vDeployments[Consensus::HARDFORK_POS].nStartTime = 999999999999ULL;
        consensus.vDeployments[Consensus::HARDFORK_POS].nTimeout = 999999999999ULL; 

        // The best chain should have at least this much work.
        consensus.nMinimumChainWork = uint256S("0x00");

        // By default assume that the signatures in ancestors of this block are valid.
        consensus.defaultAssumeValid = uint256S("0x00");

        consensus.hardforkHeight = 10;
        consensus.fidShiftHeight = 10;
        consensus.premineAddress = "ms17iABVQf7RQB8iaxeXPBkFdQQjCv7CmV";
        consensus.premineValue = 2000000 * COIN;

        consensus.posHeight = 2000;
        
        pchBitcoinMessageStart[0] = 0xfa;
        pchBitcoinMessageStart[1] = 0xbf;
        pchBitcoinMessageStart[2] = 0xb5;
        pchBitcoinMessageStart[3] = 0xda;

        pchMessageStart[0] = 0xf5;
        pchMessageStart[1] = 0xb3;
        pchMessageStart[2] = 0xb6;
        pchMessageStart[3] = 0xd9;

        nDefaultPort = 18444;
        nPruneAfterHeight = 1000;

        genesis = CreateGenesisBlock(1533214813 , 865474, 0x1e0ffff0, 1, 50 * COIN);
        consensus.hashGenesisBlock = genesis.GetHash();
        assert(consensus.hashGenesisBlock == uint256S("0x0000024ba250dee43b8d3476ed6fda1045f4e3873af4b9ef6658a0f4ed7176e7"));
        assert(genesis.hashMerkleRoot == uint256S("0x1f38dcfad187198907c360e569afcbc359c025cef3a4d6cf37b5ab4c3a552597"));

        vFixedSeeds.clear(); //!< Regtest mode doesn't have any fixed seeds.
        vSeeds.clear();      //!< Regtest mode doesn't have any DNS seeds.

        fDefaultConsistencyChecks = true;
        fRequireStandard = false;
        fMineBlocksOnDemand = true;

        checkpointData = (CCheckpointData) {
            {
                {0, uint256S("1f38dcfad187198907c360e569afcbc359c025cef3a4d6cf37b5ab4c3a552597")},
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

static std::unique_ptr<CChainParams> globalChainParams;

const CChainParams &Params() {
    assert(globalChainParams);
    return *globalChainParams;
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
