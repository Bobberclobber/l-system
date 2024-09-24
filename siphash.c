#include <assert.h>
#include "inttypes.h"

/* Default: SipHash-2-4 */
#ifndef cROUNDS
#define cROUNDS 2
#endif
#ifndef dROUNDS
#define dROUNDS 4
#endif

#define U8TO64_LE(p)														   \
	(((uint64)((p)[0])) | ((uint64)((p)[1]) << 8) |							   \
	 ((uint64)((p)[2]) << 16) |((uint64)((p)[3]) << 24) |					   \
	 ((uint64)((p)[4]) << 32) |((uint64)((p)[5]) << 40) |					   \
	 ((uint64)((p)[6]) << 48) |((uint64)((p)[7]) << 56))

#define U32TO8_LE(p, v)														   \
	(p)[0] = (uint8)((v));													   \
	(p)[1] = (uint8)((v) >> 8);												   \
	(p)[2] = (uint8)((v) >> 16);											   \
	(p)[3] = (uint8)((v) >> 24);

#define U64TO8_LE(p, v) 													   \
	U32TO8_LE((p), (uint32)((v))); 											   \
	U32TO8_LE((p) + 4, (uint32)((v) >> 32));

#define ROTL(x, b) (uint64)(((x) << (b)) | ((x) >> (64 - (b))))

#define SIPROUND                                                               \
    do {                                                                       \
        v0 += v1;                                                              \
        v1 = ROTL(v1, 13);                                                     \
        v1 ^= v0;                                                              \
        v0 = ROTL(v0, 32);                                                     \
        v2 += v3;                                                              \
        v3 = ROTL(v3, 16);                                                     \
        v3 ^= v2;                                                              \
        v0 += v3;                                                              \
        v3 = ROTL(v3, 21);                                                     \
        v3 ^= v0;                                                              \
        v2 += v1;                                                              \
        v1 = ROTL(v1, 17);                                                     \
        v1 ^= v2;                                                              \
        v2 = ROTL(v2, 32);                                                     \
    } while (0)

int siphash(const void *in, const uint16 inlen, const void *k, uint8 *out,
			const uint16 outlen) {
	const unsigned char *ni = (const unsigned char *)in;
	const unsigned char *kk = (const unsigned char *)k;

	assert((outlen == 8) || (outlen == 16));
	uint64 v0 = 0x736f6d6570736575;
    uint64 v1 = 0x646f72616e646f6d;
    uint64 v2 = 0x6c7967656e657261;
    uint64 v3 = 0x7465646279746573;
    uint64 k0 = U8TO64_LE(kk);
    uint64 k1 = U8TO64_LE(kk + 8);
    uint64 m;
    int i;
    const unsigned char *end = ni + inlen - (inlen % sizeof(uint64));
    const int left = inlen & 7;
    uint64 b = ((uint64)inlen) << 56;
    v3 ^= k1;
    v2 ^= k0;
    v1 ^= k1;
    v0 ^= k0;

	if (outlen == 16) {
		v1 ^= 0xee;
	}

	for (; ni != end; ni += 8) {
        m = U8TO64_LE(ni);
        v3 ^= m;

        for (i = 0; i < cROUNDS; ++i) {
            SIPROUND;
		}

        v0 ^= m;
    }

	switch (left) {
    	case 7:
    	    b |= ((uint64)ni[6]) << 48;
    	    /* FALLTHRU */
    	case 6:
    	    b |= ((uint64)ni[5]) << 40;
    	    /* FALLTHRU */
    	case 5:
    	    b |= ((uint64)ni[4]) << 32;
    	    /* FALLTHRU */
    	case 4:
    	    b |= ((uint64)ni[3]) << 24;
    	    /* FALLTHRU */
    	case 3:
    	    b |= ((uint64)ni[2]) << 16;
    	    /* FALLTHRU */
    	case 2:
    	    b |= ((uint64)ni[1]) << 8;
    	    /* FALLTHRU */
    	case 1:
    	    b |= ((uint64)ni[0]);
    	    break;
    	case 0:
    	    break;
    }

	v3 ^= b;

    for (i = 0; i < cROUNDS; ++i) {
        SIPROUND;
	}

    v0 ^= b;

    if (outlen == 16) {
        v2 ^= 0xee;
	}
    else {
        v2 ^= 0xff;
	}

    for (i = 0; i < dROUNDS; ++i) {
        SIPROUND;
	}

    b = v0 ^ v1 ^ v2 ^ v3;
    U64TO8_LE(out, b);

    if (outlen == 8) {
        return 0;
	}

    v1 ^= 0xdd;

    for (i = 0; i < dROUNDS; ++i) {
        SIPROUND;
	}

    b = v0 ^ v1 ^ v2 ^ v3;
    U64TO8_LE(out + 8, b);

    return 0;
}
