//
//  test.c
//
//  Created by Aaron Voisine on 8/14/15.
//  Copyright (c) 2015 breadwallet LLC
//
//  Permission is hereby granted, free of charge, to any person obtaining a copy
//  of this software and associated documentation files (the "Software"), to deal
//  in the Software without restriction, including without limitation the rights
//  to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
//  copies of the Software, and to permit persons to whom the Software is
//  furnished to do so, subject to the following conditions:
//
//  The above copyright notice and this permission notice shall be included in
//  all copies or substantial portions of the Software.
//
//  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
//  IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
//  FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
//  AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
//  LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
//  OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
//  THE SOFTWARE.

#include "BRCrypto.h"
#include "BRBloomFilter.h"
#include "BRMerkleBlock.h"
#include "BRWallet.h"
#include "BRKey.h"
#include "BRBIP38Key.h"
#include "BRAddress.h"
#include "BRBase58.h"
#include "BRBIP39Mnemonic.h"
#include "BRBIP39WordsEn.h"
#include "BRPeer.h"
#include "BRPeerManager.h"
#include "BRPaymentProtocol.h"
#include "BRInt.h"
#include "BRArray.h"
#include "BRSet.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <inttypes.h>
#include <errno.h>
#include <time.h>
#include <unistd.h>
#include <arpa/inet.h>


static void walletBalanceChanged(void *info, uint64_t balance)
{
    printf("balance changed %"PRIu64"\n", balance);
}

static void walletTxAdded(void *info, BRTransaction *tx)
{
    printf("tx added: %s\n", u256_hex_encode(tx->txHash));
}

static void walletTxUpdated(void *info, const UInt256 txHashes[], size_t txCount, uint32_t blockHeight,
                            uint32_t timestamp)
{
    for (size_t i = 0; i < txCount; i++) printf("tx updated: %s\n", u256_hex_encode(txHashes[i]));
}

static void walletTxDeleted(void *info, UInt256 txHash, int notifyUser, int recommendRescan)
{
    printf("tx deleted: %s\n", u256_hex_encode(txHash));
}

void syncStarted(void *info)
{
    printf("sync started\n");
}

void syncStopped(void *info, int error)
{
    printf("sync stopped: %s\n", strerror(error));
}

void txStatusUpdate(void *info)
{
    printf("transaction status updated\n");
}

int main(int argc, const char *argv[])
{
    
    int err = 0;
    UInt512 seed = UINT512_ZERO;
    BRMasterPubKey mpk = BR_MASTER_PUBKEY_NONE;
    BRWallet *wallet;
    BRPeerManager *manager;

    //BRBIP39DeriveKey(seed.u8, "video tiger report bid suspect taxi mail argue naive layer metal surface", NULL);
    BRBIP39DeriveKey(seed.u8, "axis husband project any sea patch drip tip spirit tide bring belt", NULL);
    mpk = BRBIP32MasterPubKey(&seed, sizeof(seed));

    wallet = BRWalletNew(NULL, 0, mpk);
    BRWalletSetCallbacks(wallet, wallet, walletBalanceChanged, walletTxAdded, walletTxUpdated, walletTxDeleted);
    printf("wallet created with first receive address: %s\n", BRWalletReceiveAddress(wallet).s);

    manager = BRPeerManagerNew(wallet, BIP39_CREATION_TIME, NULL, 0, NULL, 0);
    BRPeerManagerSetCallbacks(manager, manager, syncStarted, syncStopped, txStatusUpdate, NULL, NULL, NULL, NULL);

    BRPeerManagerConnect(manager);
    while (err == 0 && BRPeerManagerPeerCount(manager) > 0) err = sleep(1);
    if (err != 0) printf("sleep got a signal\n");

    BRPeerManagerDisconnect(manager);
    BRPeerManagerFree(manager);
    BRWalletFree(wallet);
    sleep(5);

}
