#include <stddef.h>
#include <wctype.h>

#define CASEMAP(u1, u2, l) \
    { (u1), (l) - (u1), (u2) - (u1) + 1 }
#define CASELACE(u1, u2) CASEMAP((u1), (u2), (u1) + 1)

static const struct {
    unsigned short upper;
    signed char lower;
    unsigned char len;
} casemaps[] = {CASEMAP('A', 'Z', 'a'), CASEMAP(0xc0, 0xde, 0xe0),

                CASELACE(0x0100, 0x012e),
                CASELACE(0x0132, 0x0136),
                CASELACE(0x0139, 0x0147),
                CASELACE(0x014a, 0x0176),
                CASELACE(0x0179, 0x017d),

                CASELACE(0x370, 0x372),
                CASEMAP(0x391, 0x3a1, 0x3b1),
                CASEMAP(0x3a3, 0x3ab, 0x3c3),
                CASEMAP(0x400, 0x40f, 0x450),
                CASEMAP(0x410, 0x42f, 0x430),

                CASELACE(0x460, 0x480),
                CASELACE(0x48a, 0x4be),
                CASELACE(0x4c1, 0x4cd),
                CASELACE(0x4d0, 0x50e),

                CASELACE(0x514, 0x526),
                CASEMAP(0x531, 0x556, 0x561),

                CASELACE(0x01a0, 0x01a4),
                CASELACE(0x01b3, 0x01b5),
                CASELACE(0x01cd, 0x01db),
                CASELACE(0x01de, 0x01ee),
                CASELACE(0x01f8, 0x021e),
                CASELACE(0x0222, 0x0232),
                CASELACE(0x03d8, 0x03ee),

                CASELACE(0x1e00, 0x1e94),
                CASELACE(0x1ea0, 0x1efe),

                CASEMAP(0x1f08, 0x1f0f, 0x1f00),
                CASEMAP(0x1f18, 0x1f1d, 0x1f10),
                CASEMAP(0x1f28, 0x1f2f, 0x1f20),
                CASEMAP(0x1f38, 0x1f3f, 0x1f30),
                CASEMAP(0x1f48, 0x1f4d, 0x1f40),

                CASEMAP(0x1f68, 0x1f6f, 0x1f60),
                CASEMAP(0x1f88, 0x1f8f, 0x1f80),
                CASEMAP(0x1f98, 0x1f9f, 0x1f90),
                CASEMAP(0x1fa8, 0x1faf, 0x1fa0),
                CASEMAP(0x1fb8, 0x1fb9, 0x1fb0),
                CASEMAP(0x1fba, 0x1fbb, 0x1f70),
                CASEMAP(0x1fc8, 0x1fcb, 0x1f72),
                CASEMAP(0x1fd8, 0x1fd9, 0x1fd0),
                CASEMAP(0x1fda, 0x1fdb, 0x1f76),
                CASEMAP(0x1fe8, 0x1fe9, 0x1fe0),
                CASEMAP(0x1fea, 0x1feb, 0x1f7a),
                CASEMAP(0x1ff8, 0x1ff9, 0x1f78),
                CASEMAP(0x1ffa, 0x1ffb, 0x1f7c),

                CASELACE(0x246, 0x24e),
                CASELACE(0x510, 0x512),
                CASEMAP(0x2160, 0x216f, 0x2170),
                CASEMAP(0x2c00, 0x2c2e, 0x2c30),
                CASELACE(0x2c67, 0x2c6b),
                CASELACE(0x2c80, 0x2ce2),
                CASELACE(0x2ceb, 0x2ced),

                CASELACE(0xa640, 0xa66c),
                CASELACE(0xa680, 0xa696),

                CASELACE(0xa722, 0xa72e),
                CASELACE(0xa732, 0xa76e),
                CASELACE(0xa779, 0xa77b),
                CASELACE(0xa77e, 0xa786),

                CASELACE(0xa790, 0xa792),
                CASELACE(0xa7a0, 0xa7a8),

                CASEMAP(0xff21, 0xff3a, 0xff41),
                {0, 0, 0}};

static const unsigned short pairs[][2] = {{'I', 0x0131},
                                          {'S', 0x017f},
                                          {0x0130, 'i'},
                                          {0x0178, 0x00ff},
                                          {0x0181, 0x0253},
                                          {0x0182, 0x0183},
                                          {0x0184, 0x0185},
                                          {0x0186, 0x0254},
                                          {0x0187, 0x0188},
                                          {0x0189, 0x0256},
                                          {0x018a, 0x0257},
                                          {0x018b, 0x018c},
                                          {0x018e, 0x01dd},
                                          {0x018f, 0x0259},
                                          {0x0190, 0x025b},
                                          {0x0191, 0x0192},
                                          {0x0193, 0x0260},
                                          {0x0194, 0x0263},
                                          {0x0196, 0x0269},
                                          {0x0197, 0x0268},
                                          {0x0198, 0x0199},
                                          {0x019c, 0x026f},
                                          {0x019d, 0x0272},
                                          {0x019f, 0x0275},
                                          {0x01a6, 0x0280},
                                          {0x01a7, 0x01a8},
                                          {0x01a9, 0x0283},
                                          {0x01ac, 0x01ad},
                                          {0x01ae, 0x0288},
                                          {0x01af, 0x01b0},
                                          {0x01b1, 0x028a},
                                          {0x01b2, 0x028b},
                                          {0x01b7, 0x0292},
                                          {0x01b8, 0x01b9},
                                          {0x01bc, 0x01bd},
                                          {0x01c4, 0x01c6},
                                          {0x01c4, 0x01c5},
                                          {0x01c5, 0x01c6},
                                          {0x01c7, 0x01c9},
                                          {0x01c7, 0x01c8},
                                          {0x01c8, 0x01c9},
                                          {0x01ca, 0x01cc},
                                          {0x01ca, 0x01cb},
                                          {0x01cb, 0x01cc},
                                          {0x01f1, 0x01f3},
                                          {0x01f1, 0x01f2},
                                          {0x01f2, 0x01f3},
                                          {0x01f4, 0x01f5},
                                          {0x01f6, 0x0195},
                                          {0x01f7, 0x01bf},
                                          {0x0220, 0x019e},
                                          {0x0386, 0x03ac},
                                          {0x0388, 0x03ad},
                                          {0x0389, 0x03ae},
                                          {0x038a, 0x03af},
                                          {0x038c, 0x03cc},
                                          {0x038e, 0x03cd},
                                          {0x038f, 0x03ce},
                                          {0x0399, 0x0345},
                                          {0x0399, 0x1fbe},
                                          {0x03a3, 0x03c2},
                                          {0x03f7, 0x03f8},
                                          {0x03fa, 0x03fb},
                                          {0x1e60, 0x1e9b},
                                          {0x1e9e, 0xdf},

                                          {0x1f59, 0x1f51},
                                          {0x1f5b, 0x1f53},
                                          {0x1f5d, 0x1f55},
                                          {0x1f5f, 0x1f57},
                                          {0x1fbc, 0x1fb3},
                                          {0x1fcc, 0x1fc3},
                                          {0x1fec, 0x1fe5},
                                          {0x1ffc, 0x1ff3},

                                          {0x23a, 0x2c65},
                                          {0x23b, 0x23c},
                                          {0x23d, 0x19a},
                                          {0x23e, 0x2c66},
                                          {0x241, 0x242},
                                          {0x243, 0x180},
                                          {0x244, 0x289},
                                          {0x245, 0x28c},
                                          {0x3f4, 0x3b8},
                                          {0x3f9, 0x3f2},
                                          {0x3fd, 0x37b},
                                          {0x3fe, 0x37c},
                                          {0x3ff, 0x37d},
                                          {0x4c0, 0x4cf},

                                          {0x2126, 0x3c9},
                                          {0x212a, 'k'},
                                          {0x212b, 0xe5},
                                          {0x2132, 0x214e},
                                          {0x2183, 0x2184},
                                          {0x2c60, 0x2c61},
                                          {0x2c62, 0x26b},
                                          {0x2c63, 0x1d7d},
                                          {0x2c64, 0x27d},
                                          {0x2c6d, 0x251},
                                          {0x2c6e, 0x271},
                                          {0x2c6f, 0x250},
                                          {0x2c70, 0x252},
                                          {0x2c72, 0x2c73},
                                          {0x2c75, 0x2c76},
                                          {0x2c7e, 0x23f},
                                          {0x2c7f, 0x240},
                                          {0x2cf2, 0x2cf3},

                                          {0xa77d, 0x1d79},
                                          {0xa78b, 0xa78c},
                                          {0xa78d, 0x265},
                                          {0xa7aa, 0x266},

                                          {0x10c7, 0x2d27},
                                          {0x10cd, 0x2d2d},

                                          /* bogus greek 'symbol' letters */
                                          {0x376, 0x377},
                                          {0x39c, 0xb5},
                                          {0x392, 0x3d0},
                                          {0x398, 0x3d1},
                                          {0x3a6, 0x3d5},
                                          {0x3a0, 0x3d6},
                                          {0x39a, 0x3f0},
                                          {0x3a1, 0x3f1},
                                          {0x395, 0x3f5},
                                          {0x3cf, 0x3d7},

                                          {0, 0}};

static wchar_t __towcase(wchar_t wc, int lower) {
    int i;
    int lmul = 2 * lower - 1;
    int lmask = lower - 1;
    /* no letters with case in these large ranges */
    if (!iswalpha(wc) || (unsigned)wc - 0x0600 <= 0x0fff - 0x0600 ||
        (unsigned)wc - 0x2e00 <= 0xa63f - 0x2e00 || (unsigned)wc - 0xa800 <= 0xfeff - 0xa800)
        return wc;
    /* special case because the diff between upper/lower is too big */
    if (lower && (unsigned)wc - 0x10a0 < 0x2e)
        if (wc > 0x10c5 && wc != 0x10c7 && wc != 0x10cd)
            return wc;
        else
            return wc + 0x2d00 - 0x10a0;
    if (!lower && (unsigned)wc - 0x2d00 < 0x26)
        if (wc > 0x2d25 && wc != 0x2d27 && wc != 0x2d2d)
            return wc;
        else
            return wc + 0x10a0 - 0x2d00;
    for (i = 0; casemaps[i].len; i++) {
        int base = casemaps[i].upper + (lmask & casemaps[i].lower);
        if ((unsigned)wc - base < casemaps[i].len) {
            if (casemaps[i].lower == 1)
                return wc + lower - ((wc - casemaps[i].upper) & 1);
            return wc + lmul * casemaps[i].lower;
        }
    }
    for (i = 0; pairs[i][1 - lower]; i++) {
        if (pairs[i][1 - lower] == wc)
            return pairs[i][lower];
    }
    if ((unsigned)wc - (0x10428 - 0x28 * lower) < 0x28)
        return wc - 0x28 + 0x50 * lower;
    return wc;
}

wint_t towupper(wint_t wc) {
    return __towcase(wc, 0);
}

wint_t towlower(wint_t wc) {
    return __towcase(wc, 1);
}
