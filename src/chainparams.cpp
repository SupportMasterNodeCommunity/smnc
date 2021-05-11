// Copyright (c) 2010 Satoshi Nakamoto
// Copyright (c) 2009-2014 The Bitcoin developers
// Copyright (c) 2014-2015 The Dash developers
// Copyright (c) 2015-2018 The PIVX developers
// Copyright (c) 2019-2020 The MasterWin developers
// Copyright (c) 2020-2021 The SupportMasterNodeCommunity developers
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#include "libzerocoin/Params.h"
#include "chainparams.h"
#include "random.h"
#include "util.h"
#include "utilstrencodings.h"

#include <assert.h>

#include <boost/assign/list_of.hpp>

using namespace std;
using namespace boost::assign;

struct SeedSpec6 {
    uint8_t addr[0];
    uint16_t port;
};

#include "chainparamsseeds.h"

/**
 * Main network
 */

//! Convert the pnSeeds6 array into usable address objects.
static void convertSeed6(std::vector<CAddress>& vSeedsOut, const SeedSpec6* data, unsigned int count)
{
    // It'll only connect to one or two seed nodes because once it connects,
    // it'll get a pile of addresses with newer timestamps.
    // Seed nodes are given a random 'last seen time' of between one and two
    // weeks ago.
    const int64_t nOneWeek = 7 * 24 * 60 * 60;
    for (unsigned int i = 0; i < count; i++) {
        struct in6_addr ip;
        memcpy(&ip, data[i].addr, sizeof(ip));
        CAddress addr(CService(ip, data[i].port));
        addr.nTime = GetTime() - GetRand(nOneWeek) - nOneWeek;
        vSeedsOut.push_back(addr);
    }
}

//   What makes a good checkpoint block?
// + Is surrounded by blocks with reasonable timestamps
//   (no blocks before with a timestamp after, none after with
//    timestamp before)
// + Contains no strange transactions
static Checkpoints::MapCheckpoints mapCheckpoints =
    boost::assign::map_list_of
    (0, uint256("0x00000886870b1ea12f26f3ced5f59d8d94c71958be350cacfa1467038a1d1aa6"));
static const Checkpoints::CCheckpointData data = {
    &mapCheckpoints,
    1620555589, // * UNIX timestamp of last checkpoint block
    0,		// * total number of transactions between genesis and last checkpoint
                //   (the tx=... number in the SetBestChain debug.log lines)
    2000        // * estimated number of transactions per day after checkpoint
};

static Checkpoints::MapCheckpoints mapCheckpointsTestnet =
    boost::assign::map_list_of(0, uint256("0x001"));
static const Checkpoints::CCheckpointData dataTestnet = {
    &mapCheckpointsTestnet,
    1620555590,
    0,
    250};

static Checkpoints::MapCheckpoints mapCheckpointsRegtest =
    boost::assign::map_list_of(0, uint256("0x001"));
static const Checkpoints::CCheckpointData dataRegtest = {
    &mapCheckpointsRegtest,
    1620555591,
    0,
    100};

libzerocoin::ZerocoinParams* CChainParams::Zerocoin_Params(bool useModulusV1) const
{
    assert(this);
    static CBigNum bnHexModulus = 0;
    if (!bnHexModulus)
        bnHexModulus.SetHex(zerocoinModulus);
    static libzerocoin::ZerocoinParams ZCParamsHex = libzerocoin::ZerocoinParams(bnHexModulus);
    static CBigNum bnDecModulus = 0;
    if (!bnDecModulus)
        bnDecModulus.SetDec(zerocoinModulus);
    static libzerocoin::ZerocoinParams ZCParamsDec = libzerocoin::ZerocoinParams(bnDecModulus);

    if (useModulusV1)
        return &ZCParamsHex;

    return &ZCParamsDec;
}

class CMainParams : public CChainParams
{
public:
    CMainParams()
    {
        networkID = CBaseChainParams::MAIN;
        strNetworkID = "main";
        /**
         * The message start string is designed to be unlikely to occur in normal data.
         * The characters are rarely used upper ASCII, not valid as UTF-8, and produce
         * a large 4-byte int at any alignment.
         */
        pchMessageStart[0] = 0xb2;
        pchMessageStart[1] = 0x5e;
        pchMessageStart[2] = 0xca;
        pchMessageStart[3] = 0x1d;
		vAlertPubKey = ParseHex("0473ea781030edaac07782c5040bf842547d598177b73db50d9e74590940dd1f7fc4f0be9788c14fff92d0c3cd498c15b0a549b0d2a096287a2b53f04f73429c3b");
		nDefaultPort = 44555;
        bnProofOfWorkLimit = ~uint256(0) >> 20; // SupportMasterNodeCommunity starting difficulty is 1 / 2^12
        //nSubsidyHalvingInterval = 210000;
        nMaxReorganizationDepth = 100;
        nEnforceBlockUpgradeMajority = 750;
        nRejectBlockOutdatedMajority = 950;
        nToCheckBlockUpgradeMajority = 1000;
        nMinerThreads = 0;
        nTargetTimespan = 1 * 60; // SMNC: 1 day
        nTargetSpacing = 1 * 60;  // SMNC: 1 minute
        nMaturity = 100;
        nMasternodeCountDrift = 20;
        nMaxMoneyOut = 50000000 * COIN;

        /** Height or Time Based Activations **/
        nLastPOWBlock = 200;
        //nModifierUpdateBlock = 999999999;
        nZerocoinStartHeight = 1620555589;
        nZerocoinStartTime = 1620555589;
        nBlockEnforceSerialRange = 253; //Enforce serial range starting this block
        nBlockRecalculateAccumulators = ~1; //Trigger a recalculation of accumulators
        nBlockFirstFraudulent = ~1; //First block that bad serials emerged
        nBlockLastGoodCheckpoint = ~1; //Last valid accumulator checkpoint
        nBlockEnforceInvalidUTXO = 999999999; //Start enforcing the invalid UTXO's
        nInvalidAmountFiltered = 0 * COIN; //Amount of invalid coins filtered through exchanges, that should be considered valid
        nBlockZerocoinV2 = 9999999; //!> The block that zerocoin v2 becomes active - roughly Tuesday, May 8, 2018 4:00:00 AM GMT
        nEnforceNewSporkKey = 1618857749; //!> Sporks signed after (GMT):  Wednesday, June 12, 2019 5:00:00 AM GMT must use the new spork key

        /**
         * Build the genesis block. Note that the output of the genesis coinbase cannot
         * be spent as it did not originally exist in the database.
         *
         * CBlock(hash=00000ffd590b14, ver=1, hashPrevBlock=00000000000000, hashMerkleRoot=e0028e, nTime=1390095618, nBits=1e0ffff0, nNonce=28917698, vtx=1)
         *   CTransaction(hash=e0028e, ver=1, vin.size=1, vout.size=1, nLockTime=0)
         *     CTxIn(COutPoint(000000, -1), coinbase 04ffff001d01044c5957697265642030392f4a616e2f3230313420546865204772616e64204578706572696d656e7420476f6573204c6976653a204f76657273746f636b2e636f6d204973204e6f7720416363657074696e6720426974636f696e73)
         *     CTxOut(nValue=50.00000000, scriptPubKey=0xA9037BAC7050C479B121CF)
         *   vMerkleTree: e0028e
         */
        const char* pszTimestamp = "SupportMasterNodeCommunity Created by MasterWin Developer 2021.";
        CMutableTransaction txNew;
        txNew.vin.resize(1);
        txNew.vout.resize(1);
        txNew.vin[0].scriptSig = CScript() << 486604799 << CScriptNum(4) << vector<unsigned char>((const unsigned char*)pszTimestamp, (const unsigned char*)pszTimestamp + strlen(pszTimestamp));
		txNew.vout[0].SetEmpty();
        txNew.vout[0].scriptPubKey = CScript() << ParseHex("040ce765e38edd6efbb45468bdff06b00c2177b37a0fdc9808dc046b01ce2b40b645db03928e6fe1077a9239040f24bf843893bb81b67cb4c33471abb1c50b94ee") << OP_CHECKSIG;
        genesis.vtx.push_back(txNew);
        genesis.hashPrevBlock = 0;
        genesis.hashMerkleRoot = genesis.BuildMerkleTree();
        genesis.nVersion = 1;
        genesis.nTime = 1620555589;
        genesis.nBits = 0x1e0ffff0;
        genesis.nNonce = 13315739;

        hashGenesisBlock = genesis.GetHash();
        assert(hashGenesisBlock == uint256("0x00000886870b1ea12f26f3ced5f59d8d94c71958be350cacfa1467038a1d1aa6"));
        assert(genesis.hashMerkleRoot == uint256("0xd03805b519b0b22277a4e1a3aa49b81f80b72ffd6abc96cae041a6c80dfaf89b"));

        vSeeds.push_back(CDNSSeedData("1", "[2001:4ba0:cafe:15f2::11]"));
        vSeeds.push_back(CDNSSeedData("2", "[2001:4ba0:cafe:15f2::12]"));

        base58Prefixes[PUBKEY_ADDRESS] = std::vector<unsigned char>(1, 63); // Start with 'S'
        base58Prefixes[SCRIPT_ADDRESS] = std::vector<unsigned char>(1, 39); //starts with 'G'
        base58Prefixes[SECRET_KEY] = std::vector<unsigned char>(1, 242);
        base58Prefixes[EXT_PUBLIC_KEY] = boost::assign::list_of(0x22)(0x2e)(0x35)(0x13).convert_to_container<std::vector<unsigned char> >();
        base58Prefixes[EXT_SECRET_KEY] = boost::assign::list_of(0x52)(0xf1)(0xe1)(0x1B).convert_to_container<std::vector<unsigned char> >();
        base58Prefixes[EXT_COIN_TYPE] = boost::assign::list_of(0x60)(0x70)(0x80)(0x97).convert_to_container<std::vector<unsigned char> >();

        convertSeed6(vFixedSeeds, pnSeed6_main, ARRAYLEN(pnSeed6_main));

        fMiningRequiresPeers = false;
        fAllowMinDifficultyBlocks = false;
        fDefaultConsistencyChecks = false;
        fRequireStandard = true;
        fMineBlocksOnDemand = false;
        fSkipProofOfWorkCheck = false;
        fTestnetToBeDeprecatedFieldRPC = false;
        fHeadersFirstSyncingActive = false;

        nPoolMaxTransactions = 3;
        strSporkKey = "04693eb5c0399f45beb6cd83c53349057127ed07f4cb4033e0af07f23390ebae564ef0e89e524438c6f3fa197014d1fcf7aeeee60505e33d65ff4f783c3351dc42";
        strObfuscationPoolDummyAddress = "S7JjdPvvFje4of1u8GXJRdZRQFy1eKCEBo";
        nStartMasternodePayments = 1620555862; 

        /** Zerocoin */
        zerocoinModulus = "25195908475657893494027183240048398571429282126204032027777137836043662020707595556264018525880784"
            "4069182906412495150821892985591491761845028084891200728449926873928072877767359714183472702618963750149718246911"
            "6507761337985909570009733045974880842840179742910064245869181719511874612151517265463228221686998754918242243363"
            "7259085141865462043576798423387184774447920739934236584823824281198163815010674810451660377306056201619676256133"
            "8441436038339044149526344321901146575444541784240209246165157233507787077498171257724679629263863563732899121548"
            "31438167899885040445364023527381951378636564391212010397122822120720357";
        nMaxZerocoinSpendsPerTransaction = 7; // Assume about 20kb each
        nMinZerocoinMintFee = 1 * CENT; //high fee required for zerocoin mints
        nMintRequiredConfirmations = 20; //the maximum amount of confirmations until accumulated in 19
        nRequiredAccumulation = 1;
        nDefaultSecurityLevel = 100; //full security level for accumulators
        nZerocoinHeaderVersion = 4; //Block headers must be this version once zerocoin is active
        nZerocoinRequiredStakeDepth = 200; //The required confirmations for a zsmnc to be stakable
        
    vMasternodePhasen.push_back ({
      100,
      {
        {  25000 * COIN, 95 },
        {  50000 * COIN, 0 },
        {  100000 * COIN, 0 },
        {  125000 * COIN, 0 },
        {  150000 * COIN, 0 },
        {  200000 * COIN, 0 },
        {  275000 * COIN, 0 },
        {  300000 * COIN, 0 },
        {  375000 * COIN, 0 },
        {  425000 * COIN, 0 },
        {  500000 * COIN, 0 }
      }
    });
    vMasternodePhasen.push_back ({
      20000,
      {
        {  25000 * COIN, 0 },
        {  50000 * COIN, 95 },
        {  100000 * COIN, 0 },
        {  125000 * COIN, 0 },
        {  150000 * COIN, 0 },
        {  200000 * COIN, 0 },
        {  275000 * COIN, 0 },
        {  300000 * COIN, 0 },
        {  375000 * COIN, 0 },
        {  425000 * COIN, 0 },
        {  500000 * COIN, 0 }
      }
    });
    vMasternodePhasen.push_back ({
      40000,
      {
        {  25000 * COIN, 0 },
        {  50000 * COIN, 0 },
        {  100000 * COIN, 95 },
        {  125000 * COIN, 0 },
        {  150000 * COIN, 0 },
        {  200000 * COIN, 0 },
        {  275000 * COIN, 0 },
        {  300000 * COIN, 0 },
        {  375000 * COIN, 0 },
        {  425000 * COIN, 0 },
        {  500000 * COIN, 0 }
      }
    });
    vMasternodePhasen.push_back ({
      80000,
      {
        {  25000 * COIN, 0 },
        {  50000 * COIN, 0 },
        {  100000 * COIN, 0 },
        {  125000 * COIN, 95 },
        {  150000 * COIN, 0 },
        {  200000 * COIN, 0 },
        {  275000 * COIN, 0 },
        {  300000 * COIN, 0 },
        {  375000 * COIN, 0 },
        {  425000 * COIN, 0 },
        {  500000 * COIN, 0 }
      }
    });
    vMasternodePhasen.push_back ({
      120000,
      {
        {  25000 * COIN, 0 },
        {  50000 * COIN, 0 },
        {  100000 * COIN, 0 },
        {  125000 * COIN, 0 },
        {  150000 * COIN, 95 },
        {  200000 * COIN, 0 },
        {  275000 * COIN, 0 },
        {  300000 * COIN, 0 },
        {  375000 * COIN, 0 },
        {  425000 * COIN, 0 },
        {  500000 * COIN, 0 }
      }
    });
    vMasternodePhasen.push_back ({
      150000,
      {
        {  25000 * COIN, 0 },
        {  50000 * COIN, 0 },
        {  100000 * COIN, 0 },
        {  125000 * COIN, 0 },
        {  150000 * COIN, 0 },
        {  200000 * COIN, 95 },
        {  275000 * COIN, 0 },
        {  300000 * COIN, 0 },
        {  375000 * COIN, 0 },
        {  425000 * COIN, 0 },
        {  500000 * COIN, 0 }
      }
    });
    vMasternodePhasen.push_back ({
      200000,
      {
        {  25000 * COIN, 0 },
        {  50000 * COIN, 0 },
        {  100000 * COIN, 0 },
        {  125000 * COIN, 0 },
        {  150000 * COIN, 0 },
        {  200000 * COIN, 0 },
        {  275000 * COIN, 95 },
        {  300000 * COIN, 0 },
        {  375000 * COIN, 0 },
        {  425000 * COIN, 0 },
        {  500000 * COIN, 0 }
      }
    });
    vMasternodePhasen.push_back ({
      250000,
      {
        {  25000 * COIN, 0 },
        {  50000 * COIN, 0 },
        {  100000 * COIN, 0 },
        {  125000 * COIN, 0 },
        {  150000 * COIN, 0 },
        {  200000 * COIN, 0 },
        {  275000 * COIN, 0 },
        {  300000 * COIN, 95 },
        {  375000 * COIN, 0 },
        {  425000 * COIN, 0 },
        {  500000 * COIN, 0 }
      }
    });
    vMasternodePhasen.push_back ({
      500000,
      {
        {  25000 * COIN, 0 },
        {  50000 * COIN, 0 },
        {  100000 * COIN, 0 },
        {  125000 * COIN, 0 },
        {  150000 * COIN, 0 },
        {  200000 * COIN, 0 },
        {  275000 * COIN, 0 },
        {  300000 * COIN, 0 },
        {  375000 * COIN, 95 },
        {  425000 * COIN, 0 },
        {  500000 * COIN, 0 }
      }
    });
    vMasternodePhasen.push_back ({
      600000,
      {
        {  25000 * COIN, 0 },
        {  50000 * COIN, 0 },
        {  100000 * COIN, 0 },
        {  125000 * COIN, 0 },
        {  150000 * COIN, 0 },
        {  200000 * COIN, 0 },
        {  275000 * COIN, 0 },
        {  300000 * COIN, 0 },
        {  375000 * COIN, 0 },
        {  425000 * COIN, 95 },
        {  500000 * COIN, 0 }
      }
    });
    vMasternodePhasen.push_back ({
      900000,
      {
        {  25000 * COIN, 0 },
        {  50000 * COIN, 0 },
        {  100000 * COIN, 0 },
        {  125000 * COIN, 0 },
        {  150000 * COIN, 0 },
        {  200000 * COIN, 0 },
        {  275000 * COIN, 0 },
        {  300000 * COIN, 0 },
        {  375000 * COIN, 0 },
        {  425000 * COIN, 0 },
        {  500000 * COIN, 95 }
      }
    });
  }

    const Checkpoints::CCheckpointData& Checkpoints() const
    {
        return data;
    }
};
static CMainParams mainParams;

/**
 * Testnet (v3)
 */
class CTestNetParams : public CMainParams
{
public:
    CTestNetParams()
    {
        networkID = CBaseChainParams::TESTNET;
        strNetworkID = "test";
        pchMessageStart[0] = 0x43;
        pchMessageStart[1] = 0x76;
        pchMessageStart[2] = 0x65;
        pchMessageStart[3] = 0xba;
		vAlertPubKey = ParseHex("0403b7fb5a3a014d63e1850e5a52b6230c428e9459fabd9caf2a57bb63ac99e9325281a926922b07677a7f5eeccdeb90472fe21c6849a9ec3fdc0cce1796b24515");
		nDefaultPort = 32822;
        nEnforceBlockUpgradeMajority = 51;
        nRejectBlockOutdatedMajority = 75;
        nToCheckBlockUpgradeMajority = 100;
        nMinerThreads = 0;
        nTargetTimespan = 1 * 60; // SupportMasterNodeCommunity: 1 day
        nTargetSpacing = 1 * 60;  // SupportMasterNodeCommunity: 1 minute
        nLastPOWBlock = 200;
        nMaturity = 15;
        nMasternodeCountDrift = 4;
        nModifierUpdateBlock = 51197; //approx Mon, 17 Apr 2017 04:00:00 GMT
        nMaxMoneyOut = 43199500 * COIN;
        nZerocoinStartHeight = 201576;
        nZerocoinStartTime = 1501776000;
        nBlockEnforceSerialRange = 1; //Enforce serial range starting this block
        nBlockRecalculateAccumulators = 9908000; //Trigger a recalculation of accumulators
        nBlockFirstFraudulent = 9891737; //First block that bad serials emerged
        nBlockLastGoodCheckpoint = 9891730; //Last valid accumulator checkpoint
        nBlockEnforceInvalidUTXO = 9902850; //Start enforcing the invalid UTXO's
        nInvalidAmountFiltered = 0; //Amount of invalid coins filtered through exchanges, that should be considered valid
        nBlockZerocoinV2 = 444020; //!> The block that zerocoin v2 becomes active
        nEnforceNewSporkKey = 1521604800; //!> Sporks signed after Wednesday, March 21, 2018 4:00:00 AM GMT must use the new spork key

        //! Modify the testnet genesis block so the timestamp is valid for a later start.
        genesis.nTime = 1620555594;
        genesis.nNonce = 2402015;

        hashGenesisBlock = genesis.GetHash();
        //assert(hashGenesisBlock == uint256("0x0000041e482b9b9691d98eefb48473405c0b8ec31b76df3797c74a78680ef818"));

        vFixedSeeds.clear();
        vSeeds.clear();
        vSeeds.push_back(CDNSSeedData("", ""));

        base58Prefixes[PUBKEY_ADDRESS] = std::vector<unsigned char>(1, 38); // Testnet mwx addresses start with 'g' or 'y'
        base58Prefixes[SCRIPT_ADDRESS] = std::vector<unsigned char>(1, 19);  // Testnet mwx script addresses start with '8' or '9'
        base58Prefixes[SECRET_KEY] = std::vector<unsigned char>(1, 239);     // Testnet private keys start with '9' or 'c' (Bitcoin defaults)
        // Testnet mwx BIP32 pubkeys start with 'DRKV'
        base58Prefixes[EXT_PUBLIC_KEY] = boost::assign::list_of(0x3a)(0x80)(0x61)(0xa0).convert_to_container<std::vector<unsigned char> >();
        // Testnet mwx BIP32 prvkeys start with 'DRKP'
        base58Prefixes[EXT_SECRET_KEY] = boost::assign::list_of(0x3a)(0x80)(0x58)(0x37).convert_to_container<std::vector<unsigned char> >();
        // Testnet mwx BIP44 coin type is '1' (All coin's testnet default)
        base58Prefixes[EXT_COIN_TYPE] = boost::assign::list_of(0x80)(0x00)(0x00)(0x01).convert_to_container<std::vector<unsigned char> >();

		convertSeed6(vFixedSeeds, pnSeed6_test, ARRAYLEN(pnSeed6_test));

        fMiningRequiresPeers = false;
        fAllowMinDifficultyBlocks = false;
        fDefaultConsistencyChecks = false;
        fRequireStandard = false;
        fMineBlocksOnDemand = false;
        fTestnetToBeDeprecatedFieldRPC = false;

        nPoolMaxTransactions = 2;
        strSporkKey = "045faca99360e622aa32e1910c9a370ec29e80e858e954f1904be46016ea38bd2b99ad499e42f83ada42605a982ae88b1004ca673c07de4bd49616e011eeebaa7f";
        strObfuscationPoolDummyAddress = "S7JjdPvvFje4of1u8GXJRdZRQFy1eKCEBo";
        nStartMasternodePayments = 1618857956; //Fri, 09 Jan 2015 21:05:58 GMT
        nBudget_Fee_Confirmations = 3; // Number of confirmations for the finalization fee. We have to make this very short
                                       // here because we only have a 8 block finalization window on testnet
    }
    const Checkpoints::CCheckpointData& Checkpoints() const
    {
        return dataTestnet;
    }
};
static CTestNetParams testNetParams;

/**
 * Regression test
 */
class CRegTestParams : public CTestNetParams
{
public:
    CRegTestParams()
    {
        networkID = CBaseChainParams::REGTEST;
        strNetworkID = "regtest";
        strNetworkID = "regtest";
        pchMessageStart[0] = 0x69;
        pchMessageStart[1] = 0xcf;
        pchMessageStart[2] = 0x7e;
        pchMessageStart[3] = 0xac;
        //nSubsidyHalvingInterval = 150;
        nEnforceBlockUpgradeMajority = 750;
        nRejectBlockOutdatedMajority = 950;
        nToCheckBlockUpgradeMajority = 1000;
        nMinerThreads = 1;
        nTargetTimespan = 24 * 60 * 60; // SupportMasterNodeCommunity: 1 day
        nTargetSpacing = 1 * 60;        // SupportMasterNodeCommunity: 1 minutes
        bnProofOfWorkLimit = ~uint256(0) >> 1;
        genesis.nTime = 1620555595;
        genesis.nBits = 0x207fffff;
        genesis.nNonce = 2402015;

        hashGenesisBlock = genesis.GetHash();
        nDefaultPort = 32824;
        //assert(hashGenesisBlock == uint256("0x4f023a2120d9127b21bbad01724fdb79b519f593f2a85b60d3d79160ec5f29df"));

        vFixedSeeds.clear(); //! Testnet mode doesn't have any fixed seeds.
        vSeeds.clear();      //! Testnet mode doesn't have any DNS seeds.

        fMiningRequiresPeers = false;
        fAllowMinDifficultyBlocks = false;
        fDefaultConsistencyChecks = false;
        fRequireStandard = false;
        fMineBlocksOnDemand = false;
        fTestnetToBeDeprecatedFieldRPC = false;
    }
    const Checkpoints::CCheckpointData& Checkpoints() const
    {
        return dataRegtest;
    }
};
static CRegTestParams regTestParams;

/**
 * Unit test
 */
class CUnitTestParams : public CMainParams, public CModifiableParams
{
public:
    CUnitTestParams()
    {
        networkID = CBaseChainParams::UNITTEST;
        strNetworkID = "unittest";
        nDefaultPort = 32825;
        vFixedSeeds.clear(); //! Unit test mode doesn't have any fixed seeds.
        vSeeds.clear();      //! Unit test mode doesn't have any DNS seeds.

        fMiningRequiresPeers = false;
        fDefaultConsistencyChecks = true;
        fAllowMinDifficultyBlocks = false;
        fMineBlocksOnDemand = true;
    }

    const Checkpoints::CCheckpointData& Checkpoints() const
    {
        // UnitTest share the same checkpoints as MAIN
        return data;
    }

    //! Published setters to allow changing values in unit test cases
    //virtual void setSubsidyHalvingInterval(int anSubsidyHalvingInterval) { nSubsidyHalvingInterval = anSubsidyHalvingInterval; }
    virtual void setEnforceBlockUpgradeMajority(int anEnforceBlockUpgradeMajority) { nEnforceBlockUpgradeMajority = anEnforceBlockUpgradeMajority; }
    virtual void setRejectBlockOutdatedMajority(int anRejectBlockOutdatedMajority) { nRejectBlockOutdatedMajority = anRejectBlockOutdatedMajority; }
    virtual void setToCheckBlockUpgradeMajority(int anToCheckBlockUpgradeMajority) { nToCheckBlockUpgradeMajority = anToCheckBlockUpgradeMajority; }
    virtual void setDefaultConsistencyChecks(bool afDefaultConsistencyChecks) { fDefaultConsistencyChecks = afDefaultConsistencyChecks; }
    virtual void setAllowMinDifficultyBlocks(bool afAllowMinDifficultyBlocks) { fAllowMinDifficultyBlocks = afAllowMinDifficultyBlocks; }
    virtual void setSkipProofOfWorkCheck(bool afSkipProofOfWorkCheck) { fSkipProofOfWorkCheck = afSkipProofOfWorkCheck; }
};
static CUnitTestParams unitTestParams;


static CChainParams* pCurrentParams = 0;

CModifiableParams* ModifiableParams()
{
    assert(pCurrentParams);
    assert(pCurrentParams == &unitTestParams);
    return (CModifiableParams*)&unitTestParams;
}

const CChainParams& Params()
{
    assert(pCurrentParams);
    return *pCurrentParams;
}

CChainParams& Params(CBaseChainParams::Network network)
{
    switch (network) {
    case CBaseChainParams::MAIN:
        return mainParams;
    case CBaseChainParams::TESTNET:
        return testNetParams;
    case CBaseChainParams::REGTEST:
        return regTestParams;
    case CBaseChainParams::UNITTEST:
        return unitTestParams;
    default:
        assert(false && "Unimplemented network");
        return mainParams;
    }
}

void SelectParams(CBaseChainParams::Network network)
{
    SelectBaseParams(network);
    pCurrentParams = &Params(network);
}

bool SelectParamsFromCommandLine()
{
    CBaseChainParams::Network network = NetworkIdFromCommandLine();
    if (network == CBaseChainParams::MAX_NETWORK_TYPES)
        return false;

    SelectParams(network);
    return true;
}
