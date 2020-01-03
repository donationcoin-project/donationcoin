// Copyright (c) 2013-2020 The Donationcoin developers
// Distributed under the MIT/X11 software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#include <boost/assign/list_of.hpp> // for 'map_list_of()'
#include <boost/foreach.hpp>

#include "checkpoints.h"

#include "main.h"
#include "uint256.h"

namespace Checkpoints
{
    typedef std::map<int, uint256> MapCheckpoints;

    // How many times we expect transactions after the last checkpoint to
    // be slower. This number is a compromise, as it can't be accurate for
    // every system. When reindexing from a fast disk with a slow CPU, it
    // can be up to 20, while when downloading from a slow network with a
    // fast multicore CPU, it won't be much higher than 1.
    static const double fSigcheckVerificationFactor = 5.0;

    struct CCheckpointData {
        const MapCheckpoints *mapCheckpoints;
        int64 nTimeLastCheckpoint;
        int64 nTransactionsLastCheckpoint;
        double fTransactionsPerDay;
    };

    // What makes a good checkpoint block?
    // + Is surrounded by blocks with reasonable timestamps
    //   (no blocks before with a timestamp after, none after with
    //    timestamp before)
    // + Contains no strange transactions
    static MapCheckpoints mapCheckpoints =
        boost::assign::map_list_of
        ( 0, uint256("0x56909b681667b2e089791a65f1d0953b5fe994a12a039b0057b48187e39040cc"))
        ( 8000, uint256("0x2b70375aac3cecf553e669388d88c8f04d6564073f48f126efed0931e93a7ab1"))
        ( 16000, uint256("0x95d1b81ee7c0625b5a82cbb3fecbc8f4aab9bdba4e374af2ddd5d5f8cee19c31"))
        ( 20000, uint256("0x160d48d01fb6efdbb20fe9836fc6de18ae0172ee133470f216ac802f7b48bf9a"))
        ( 24000, uint256("0x62612c24ceab478cfaa2b1a021720f45769122e57a35f55ab9e6b612e3015c1d"))
        ( 28000, uint256("0xa706d6d2bf3c4778890f16dd70574fb2b365df96b7188ab6b82b166e521f3502"))
        ( 32000, uint256("0x14ed9c6dffab57ada3e28f99c904a7927123e0f6b0f916c6e37fec1faa97e4f3"))
        ( 36000, uint256("0x5f1518b40f248bcfb02c2fdf2c4711472fbd9eba68c2be09e6b482b78319fe80"))
        ( 40000, uint256("0x9c4b7065d7d7b249a5ef08f6636f3bafade6781ee96326a30d9eaa319d4372d0"))
        ( 44000, uint256("0xa069b5c86c55aa45a8bccb28a335a5ea285679f7c3b2cf6901b78c436dfb8f50"))
        ( 48000, uint256("0xfa83a075d14c438b2124944329a4c02367f1c4b27840d895742e6c40c6c3ebc6"))
        ( 52000, uint256("0x0d0f16393f48c3bb7f19742b70ad588fecf9baf261f60b80eae2d1a2cd787402"))
        ( 56000, uint256("0x39b6f53b478dacc81860c1df7e3a8986049cc61c4454e00b596eb4c53d9afddb"))
        ( 60000, uint256("0x44ea4c5f5f1f1f0dabcff235f100a9dcafcd0eb4fd9a73cb09d90036f7723c8e"))
        ( 63500, uint256("0x596b3af95d0bd6d1419c6e61b31063a6a06dfc1d2172b2a236abd4ed93b64242"))
        (900000, uint256("0x755a5f91a1660a5a503768ab6f013b15c15accc052b3227ce5dce26db0c7ab2a"))
       (1777777, uint256("0xe07fe93405bb97630fe5b5e864516e519aa8dac3adacc1c109d903560bbbec4b"))
        ;
    static const CCheckpointData data = {
        &mapCheckpoints,
        1403632096, // * UNIX timestamp of last checkpoint block
        76935,      // * total number of transactions between genesis and last checkpoint
                    // * (the tx=... number in the SetBestChain debug.log lines)
        8000.0      // * estimated number of transactions per day after checkpoint
    };

    static MapCheckpoints mapCheckpointsTestnet = 
        boost::assign::map_list_of
        (   546, uint256("000000002a936ca763904c3c35fce2f3556c559c0214345d31b1bcebf76acb70"))
        ( 35000, uint256("2af959ab4f12111ce947479bfcef16702485f04afd95210aa90fde7d1e4a64ad"))
        ;
    static const CCheckpointData dataTestnet = {
        &mapCheckpointsTestnet,
        1369685559, // * UNIX timestamp of last testnet checkpoint block
        37581, // * total number of transactions between genesis and last checkpoint on testnet
        300 // * estimated number of transactions per day after checkpoint
    };

    const CCheckpointData &Checkpoints() {
        if (fTestNet)
            return dataTestnet;
        else
            return data;
    }

    bool CheckBlock(int nHeight, const uint256& hash)
    {
        if (fTestNet) return true; // Testnet has no checkpoints
        if (!GetBoolArg("-checkpoints", true))
            return true;

        const MapCheckpoints& checkpoints = *Checkpoints().mapCheckpoints;

        MapCheckpoints::const_iterator i = checkpoints.find(nHeight);
        if (i == checkpoints.end()) return true;
        return hash == i->second;
    }

    // Guess how far we are in the verification process at the given block index
    double GuessVerificationProgress(CBlockIndex *pindex) {
        if (pindex==NULL)
            return 0.0;

        int64 nNow = time(NULL);

        double fWorkBefore = 0.0; // Amount of work done before pindex
        double fWorkAfter = 0.0;  // Amount of work left after pindex (estimated)
        // Work is defined as: 1.0 per transaction before the last checkoint, and
        // fSigcheckVerificationFactor per transaction after.

        const CCheckpointData &data = Checkpoints();

        if (pindex->nChainTx <= data.nTransactionsLastCheckpoint) {
            double nCheapBefore = pindex->nChainTx;
            double nCheapAfter = data.nTransactionsLastCheckpoint - pindex->nChainTx;
            double nExpensiveAfter = (nNow - data.nTimeLastCheckpoint)/86400.0*data.fTransactionsPerDay;
            fWorkBefore = nCheapBefore;
            fWorkAfter = nCheapAfter + nExpensiveAfter*fSigcheckVerificationFactor;
        } else {
            double nCheapBefore = data.nTransactionsLastCheckpoint;
            double nExpensiveBefore = pindex->nChainTx - data.nTransactionsLastCheckpoint;
            double nExpensiveAfter = (nNow - pindex->nTime)/86400.0*data.fTransactionsPerDay;
            fWorkBefore = nCheapBefore + nExpensiveBefore*fSigcheckVerificationFactor;
            fWorkAfter = nExpensiveAfter*fSigcheckVerificationFactor;
        }

        return fWorkBefore / (fWorkBefore + fWorkAfter);
    }

    int GetTotalBlocksEstimate()
    {
        if (fTestNet) return 0; // Testnet has no checkpoints
        if (!GetBoolArg("-checkpoints", true))
            return 0;

        const MapCheckpoints& checkpoints = *Checkpoints().mapCheckpoints;

        return checkpoints.rbegin()->first;
    }

    CBlockIndex* GetLastCheckpoint(const std::map<uint256, CBlockIndex*>& mapBlockIndex)
    {
        if (fTestNet) return NULL; // Testnet has no checkpoints
        if (!GetBoolArg("-checkpoints", true))
            return NULL;

        const MapCheckpoints& checkpoints = *Checkpoints().mapCheckpoints;

        BOOST_REVERSE_FOREACH(const MapCheckpoints::value_type& i, checkpoints)
        {
            const uint256& hash = i.second;
            std::map<uint256, CBlockIndex*>::const_iterator t = mapBlockIndex.find(hash);
            if (t != mapBlockIndex.end())
                return t->second;
        }
        return NULL;
    }
}
