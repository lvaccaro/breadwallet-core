CC=gcc
CFLAGS=-c -Wall -I./secp256k1/
SOURCES=BRAddress.c BRBIP38Key.c BRBase58.c BRCrypto.c BRMerkleBlock.c BRPeer.c BRSet.c BRWallet.c BRBIP32Sequence.c BRBIP39Mnemonic.c BRBloomFilter.c BRKey.c BRPaymentProtocol.c BRPeerManager.c BRTransaction.c

OBJECTS=$(SOURCES:.c=.o)
EXECUTABLE=main

all: $(SOURCES) $(EXECUTABLE)

$(EXECUTABLE): $(OBJECTS)
	$(CC) main.c -Wall -I./secp256k1/ $(OBJECTS) -o $@

.c.o:
	$(CC) $(CFLAGS) $< -o $@

clean:
	rm *.o