// Copyright (c) 2009-2012 The Bitcoin developers
// Copyright (c) 2011-2012 Litecoin Developers
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

    //
    // What makes a good checkpoint block?
    // + Is surrounded by blocks with reasonable timestamps
    //   (no blocks before with a timestamp after, none after with
    //    timestamp before)
    // + Contains no strange transactions
    //
    static MapCheckpoints mapCheckpoints =
        boost::assign::map_list_of // Yo dawg, this is the secret. Checkpoint 0 hash == Genesis block hash.
        (         0, uint256("0xf96c0428a9d7c2f196fb4f0212a6b670abd3a55593bcbcd60d187e58f061b9fb"))
                        (         50, uint256("0xc135eb3c11cf46d8a1e3311d42b8e950b9fb9d599fb097ec7df0c5e7c3bfe556"))
                        (         500, uint256("0xb3ff656a0b48ba3edf60a808d9bba63bbc58deba8b253d0d78ed83297d2fd7f5"))
                        (         1000, uint256("0xcddf5cf6dd551cb3eccf025c39f890a43d9c295fa7a2506a427e674cfc9c70c8"))
                        (         5000, uint256("0xfd4bfbe673f18f5143f20d745b07d707e2c1487b21e8e0b9ed3215c8b356c48b"))
                        (         7500, uint256("0x972425640c236ff7a179ed3ecede6eeed8946832a6eb0c352467afa57372ac7d"))
                        (         10000, uint256("0xd1f75812953fd3c62808ea7af277a5e04380f5f165e1fc323602f02cddd957ec"))
                        (         15000, uint256("0x140a62def82035959770b92e5af71bab873bf21f4dd44fb9ad359b6a7f733187"))
                        (         19025, uint256("0x3e6c5a96c97c18d7b233036398dfa3129b49cf893b551a3a1f72ee3c02e14fce"))
                        (         25000, uint256("0xa82eec4a3703e71f7fbc6a11957ec52ba3893aeefe76130b3ad96dd33f6985ad"))
                        (         30000, uint256("0x2373b603354b94ddf626b246df1c8ff58b2bfa3bd281e6657bf655d9e83c7d24"))
                        (         35000, uint256("0xb3fa2d09b328bc9c51e38aa55d9f24f5c81c61e09b7429ee0644a73d3935b808"))
                        (         45000, uint256("0x465aba832f9a55963df840ce6887852777ef9add3a6dd52455917b9476cbe557"))
                        (         45365, uint256("0xd8c168a710552bfb49ee87f368531a7a09cbe093a229c75fb78a460f87a9270e"))
        ;

    bool CheckBlock(int nHeight, const uint256& hash)
    {
        if (fTestNet) return true; // Testnet has no checkpoints

        MapCheckpoints::const_iterator i = mapCheckpoints.find(nHeight);
        if (i == mapCheckpoints.end()) return true;
        return hash == i->second;
    }

    int GetTotalBlocksEstimate()
    {
        if (fTestNet) return 0;
        return mapCheckpoints.rbegin()->first;
    }

    CBlockIndex* GetLastCheckpoint(const std::map<uint256, CBlockIndex*>& mapBlockIndex)
    {
        if (fTestNet) return NULL;

        BOOST_REVERSE_FOREACH(const MapCheckpoints::value_type& i, mapCheckpoints)
        {
            const uint256& hash = i.second;
            std::map<uint256, CBlockIndex*>::const_iterator t = mapBlockIndex.find(hash);
            if (t != mapBlockIndex.end())
                return t->second;
        }
        return NULL;
    }
}
