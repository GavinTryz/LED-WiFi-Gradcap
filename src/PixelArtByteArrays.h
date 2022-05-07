// I understand this is probably not the best way to do this, but it's 3am and I graduate tomorrow.

#include <Arduino.h>

#define BLACK    0x0000
#define BLUE     0x001F
#define RED      0xF800
#define GREEN    0x07E0
#define CYAN     0x07FF
#define MAGENTA  0xF81F
#define YELLOW   0xFFE0 
#define WHITE    0xFFFF

const uint16_t textColors[] PROGMEM = {BLUE, CYAN, GREEN, YELLOW, RED, MAGENTA, WHITE, BLACK};

const uint16_t pegasus_img [] PROGMEM = {
	0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x18c0, 0x9380, 0x6240, 0x20c0, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 
	0x0000, 0x0000, 0x49c0, 0xede0, 0xbca0, 0x72c0, 0x3980, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x1880, 0xb4a0, 
	0xfe20, 0xcd20, 0x0000, 0x8340, 0x6280, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0xc4e0, 0xfe20, 0xfe20, 0x20c0, 
	0x0000, 0x0000, 0x0000, 0x0840, 0x0840, 0x0840, 0x0000, 0x0000, 0x0000, 0x41c0, 0xfe20, 0xcd20, 0xfe20, 0x20c0, 0x0000, 0x2900, 
	0xd560, 0xfe20, 0xf620, 0x5200, 0x0000, 0x0000, 0x20c0, 0x9380, 0xfe20, 0xb4a0, 0xfe20, 0x9380, 0x1080, 0xfe20, 0xfe20, 0xfe20, 
	0xfe20, 0x8b40, 0x0000, 0x0000, 0x6a80, 0xc4e0, 0xfe20, 0xb4a0, 0xe5a0, 0xfe20, 0xac60, 0xe5a0, 0xfe20, 0x2900, 0x5a40, 0xd520, 
	0x5a40, 0x0000, 0x9bc0, 0xd520, 0xe5a0, 0xfe20, 0xac60, 0xede0, 0xfe20, 0xac60, 0xdd60, 0xb460, 0x3140, 0xe5a0, 0xfe20, 0x0000, 
	0x8b40, 0xede0, 0xcce0, 0xdda0, 0xfe20, 0xbca0, 0xc4e0, 0xede0, 0xc4e0, 0xf5e0, 0xfe20, 0xfe20, 0xc4e0, 0x0000, 0x1880, 0x8b40, 
	0xfe20, 0xbca0, 0xc4e0, 0xdd60, 0xc4e0, 0xcce0, 0xdda0, 0xc4e0, 0xfe20, 0xb460, 0x7b00, 0x0000, 0x0000, 0x0000, 0xcd20, 0xfe20, 
	0xede0, 0xdd60, 0xd520, 0xcce0, 0xc4e0, 0xbca0, 0xf5e0, 0x8340, 0xa420, 0x0000, 0x0000, 0x0000, 0x49c0, 0xe5a0, 0xcd20, 0xd520, 
	0xd520, 0xc4e0, 0xcd20, 0xbca0, 0x5200, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x6240, 0xdd60, 0xfe20, 0xfe20, 0xfe20, 
	0x5200, 0x0840, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x3140, 0x8b80, 0xac60, 0x7b00, 0x0000, 0x0000, 
	0x0000, 0x0000, 0x0000, 0x0000
};


const uint16_t UCF_img [] PROGMEM = {
	0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x5aeb, 0x7bef, 
	0x632c, 0x632d, 0x738e, 0x6b4d, 0x632d, 0x738e, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x2104, 0x73ae, 0x2923, 0x8c0b, 
	0x0800, 0x4a26, 0x5a86, 0x738e, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x10a2, 0x9490, 0xde10, 0x0840, 0xb54f, 
	0x83a8, 0x52cc, 0x5aec, 0x4a69, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x528a, 0x942c, 0xa48b, 0x10a2, 0xcdf0, 0x8bea, 0x8c0c, 
	0x83aa, 0x4205, 0x630c, 0x0000, 0x0000, 0x0000, 0x0000, 0x6b6d, 0xb52e, 0x41c3, 0x18c3, 0xf715, 0xde32, 0x4a26, 0xb52e, 0x7304, 
	0x630d, 0x2104, 0x2104, 0x2945, 0x0000, 0x73ae, 0x6283, 0x9c4a, 0x9c6d, 0xde53, 0xc58f, 0x0000, 0x1080, 0x62e9, 0x83ed, 0x946f, 
	0x6b2a, 0x8410, 0x0000, 0x39c7, 0x632c, 0x8c4e, 0x62c9, 0xa4ae, 0x83a9, 0x0000, 0xc5d2, 0xde73, 0x6ae7, 0x7b69, 0x3141, 0x632c, 
	0x0000, 0x0000, 0x0000, 0x10a2, 0x73d0, 0x5223, 0xacab, 0x9c6d, 0xeed5, 0xc58f, 0x62c7, 0x1081, 0x52ab, 0x2945, 0x0000, 0x0000, 
	0x0000, 0x0000, 0x2965, 0x632c, 0x7b8b, 0x6b2a, 0x8bec, 0xc58f, 0x8beb, 0x5246, 0x5acb, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 
	0x0000, 0x0000, 0x2104, 0x6b8f, 0xaccc, 0x8367, 0x4a8c, 0x528a, 0x10a2, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 
	0x0000, 0x630c, 0x5202, 0x4a05, 0x2986, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x10a2, 0x7bcf, 
	0x632c, 0x73ae, 0x0020, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 
	0x0000, 0x0000, 0x0000, 0x0000
};

const uint16_t NASA_img [] PROGMEM = {
	0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 
	0x0000, 0x0000, 0x31e8, 0x21cc, 0x092c, 0x21cc, 0x39e8, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x18e3, 0x220f, 
	0x00d0, 0x0171, 0x0212, 0x0171, 0x00af, 0x220f, 0x18e3, 0x0000, 0x1082, 0x59e7, 0x0000, 0x1082, 0x01b0, 0x0130, 0x2b34, 0x2af4, 
	0x1253, 0x01b1, 0x01f2, 0x00b0, 0x0294, 0x49c7, 0xb1e7, 0x4186, 0x0000, 0x326e, 0x000e, 0x1ab4, 0x01f2, 0x0171, 0x1af4, 0x0254, 
	0x0193, 0x29f0, 0x8006, 0xe842, 0x3145, 0x0000, 0x1082, 0x1253, 0x1191, 0x1294, 0x0b16, 0x0a33, 0x114f, 0x5af2, 0x920d, 0xe0c4, 
	0xb885, 0x2210, 0x0000, 0x0000, 0x1905, 0x22f5, 0xefff, 0x7c36, 0x82d0, 0xff7d, 0x9aaf, 0xe576, 0xbbf2, 0x7af1, 0xcfbf, 0x0213, 
	0x1905, 0x0000, 0x1926, 0x218f, 0xdedb, 0xcd98, 0x822e, 0xd77e, 0x84f8, 0x8b52, 0xddf9, 0x4c58, 0xd73e, 0x6c77, 0x10e5, 0x0000, 
	0x2944, 0x21d0, 0x1170, 0x0274, 0x0275, 0x7312, 0x8b11, 0x2a32, 0x0ad5, 0x1a93, 0x0a12, 0x22d4, 0x0861, 0x0000, 0x0000, 0x2aaf, 
	0x0070, 0x11d1, 0x494c, 0x41af, 0x1af5, 0x0191, 0x01b1, 0x22d4, 0x008f, 0x326e, 0x0000, 0x0000, 0x0000, 0x0861, 0x21b0, 0x206b, 
	0x2211, 0x01f3, 0x0191, 0x0212, 0x0a73, 0x0110, 0x09f1, 0x0861, 0x0000, 0x0000, 0x0000, 0x0000, 0x3165, 0x1a10, 0x00d0, 0x0171, 
	0x0212, 0x0151, 0x006f, 0x19f0, 0x18e3, 0x0000, 0x0000, 0x0000, 0x0000, 0x0841, 0x0000, 0x0000, 0x39e9, 0x19ac, 0x010c, 0x19cc, 
	0x39e9, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 
	0x0000, 0x0000, 0x0000, 0x0000
};

const uint16_t creeper_img [] PROGMEM = {
	0xdf1a, 0x8e50, 0x0d01, 0x3565, 0x5da9, 0x9611, 0xbe77, 0xf6fe, 0xd719, 0x870e, 0x8ecf, 0x8eaf, 0x4cc9, 0x4c69, 0x9611, 0x6dcc, 
	0x1d63, 0x3585, 0x4d87, 0x7e0f, 0x9e53, 0xb5f6, 0xa653, 0x7f0d, 0x7e8d, 0x762c, 0x6d4d, 0x74ee, 0x13e2, 0x34e5, 0x8750, 0x5eaa, 
	0x3625, 0x5e6a, 0x660b, 0x33c6, 0x4d28, 0x8fee, 0x770b, 0x5e29, 0xc6b7, 0xf6fe, 0x3d86, 0x3d06, 0x4447, 0x3c46, 0x3405, 0x762d, 
	0x86af, 0x54aa, 0x4c88, 0x44c6, 0x4c67, 0x4407, 0xbe36, 0xf73e, 0x5f29, 0x3d25, 0x0160, 0x2203, 0x3a26, 0x8e10, 0xa754, 0x6d2d, 
	0x43a8, 0x0140, 0x19e3, 0x29e4, 0xbdd7, 0xffbf, 0x5708, 0x3d25, 0x1222, 0x08e0, 0x0000, 0x656a, 0x87ae, 0x5e8a, 0x33e5, 0x0000, 
	0x00c0, 0x19c2, 0x756d, 0xaf54, 0x670a, 0x4546, 0x22a3, 0x08e1, 0x0000, 0x4cc7, 0x670a, 0x4e67, 0x2bc4, 0x0801, 0x08a1, 0x11c1, 
	0x5548, 0x872d, 0x8ed0, 0x7e4e, 0x5568, 0x866f, 0xb7b5, 0x4407, 0x1260, 0x0240, 0x5429, 0xffdf, 0x8d50, 0x2324, 0x4dc6, 0x6709, 
	0xa6d3, 0x864f, 0x5d89, 0x7e0e, 0x96b2, 0x2a84, 0x00e0, 0x00e0, 0x3ae7, 0xc6b8, 0x7dae, 0x44e7, 0x6e2b, 0x86cf, 0xb6d5, 0x8df0, 
	0x44a8, 0x43a7, 0x3285, 0x0861, 0x0000, 0x0000, 0x00a0, 0x0200, 0x3d26, 0x77eb, 0xc6f7, 0xf6fe, 0xc737, 0x9e52, 0x5d2b, 0x3ae6, 
	0x1922, 0x0020, 0x0000, 0x0000, 0x0020, 0x0100, 0x3c06, 0x776b, 0x7dcf, 0x9dd3, 0xe79b, 0xb696, 0x758e, 0x29e5, 0x0000, 0x0000, 
	0x0000, 0x0000, 0x0000, 0x0000, 0x32e5, 0x772c, 0x44a7, 0x4c89, 0x86ee, 0x7ead, 0x8f4f, 0x43a7, 0x0040, 0x53e9, 0x7d4d, 0x9551, 
	0x63ec, 0x0120, 0x2b64, 0x4d68, 0x3c86, 0x4ca8, 0x66aa, 0x6eeb, 0xa7f1, 0x54a9, 0x08c1, 0x8e4f, 0xc7f6, 0xe7fb, 0xa633, 0x1201, 
	0x33a5, 0x44a8, 0x3c67, 0x54ea
};

const uint16_t github_img [] PROGMEM = {
	0x0000, 0x0000, 0x0000, 0x0000, 0x5acb, 0xa534, 0xe71c, 0xe71c, 0xa534, 0x5aeb, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 
	0x31a6, 0xdedb, 0xffff, 0xffff, 0xffff, 0xffff, 0xffff, 0xffff, 0xdefb, 0x39c7, 0x0000, 0x0000, 0x0000, 0x3186, 0xffff, 0xffff, 
	0xffff, 0xffff, 0xffff, 0xffff, 0xffff, 0xffff, 0xffff, 0xffff, 0x39c7, 0x0000, 0x0020, 0xf79e, 0xffff, 0x4228, 0x528a, 0xad75, 
	0x9492, 0x9492, 0xad75, 0x52aa, 0x4208, 0xffff, 0xf7be, 0x0841, 0x73ae, 0xffff, 0xffff, 0x18e3, 0x0000, 0x0000, 0x0000, 0x0000, 
	0x0000, 0x0000, 0x10a2, 0xffff, 0xffff, 0x7bef, 0xce79, 0xffff, 0xdedb, 0x1082, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 
	0x0861, 0xd69a, 0xffff, 0xd6ba, 0xf79e, 0xffff, 0x8410, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x7bcf, 
	0xffff, 0xf79e, 0xffdf, 0xffff, 0x8c51, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x8410, 0xffff, 0xf7be, 
	0xdefb, 0xffff, 0xef5d, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0xd69a, 0xffff, 0xe71c, 0xa534, 0xffdf, 
	0xe73c, 0xb596, 0x1082, 0x0000, 0x0000, 0x0000, 0x0000, 0x0861, 0x94b2, 0xffff, 0xffff, 0xa534, 0x2965, 0xffff, 0x632c, 0xbdf7, 
	0xffff, 0x632c, 0x0000, 0x0000, 0x5acb, 0xef7d, 0xffff, 0xffff, 0xffff, 0x2945, 0x0000, 0x94b2, 0xffff, 0x2945, 0x5acb, 0x2104, 
	0x0000, 0x0000, 0x52aa, 0xffff, 0xffff, 0xffff, 0x7bef, 0x0000, 0x0000, 0x0000, 0x94b2, 0xffdf, 0xbdd7, 0x2965, 0x0000, 0x0000, 
	0x4a49, 0xffff, 0xffff, 0x8c51, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x5acb, 0xe73c, 0x52aa, 0x0000, 0x0000, 0x4a49, 0xc618, 
	0x4a49, 0x0000, 0x0000, 0x0000
};

