#include "Algorithm.h"

#ifndef C_PLATFORM_STM32
#define PI  3.1415926
#endif

#ifndef ABS
    #define ABS(X)          ((X)>=0?(X):(-X))
#endif // ABS

#define I16_MAX         (int32_t)32767

#ifndef U16_MAX
    #define U16_MAX         (int32_t)65535
#endif

#include "FastMath.h"

#ifdef USE_FAST_SIN_COS
uint16_t fast_sin_value_table[129] = {
         0,       402,       804,      1206,      1607,      2009,      2410,      2811,      3211,      3611,
      4011,      4409,      4808,      5205,      5602,      5997,      6392,      6786,      7179,      7571,
      7961,      8351,      8739,      9126,      9512,      9896,     10278,     10659,     11039,     11416,
     11793,     12167,     12539,     12910,     13278,     13645,     14010,     14372,     14732,     15090,
     15446,     15800,     16151,     16499,     16846,     17189,     17530,     17869,     18204,     18537,
     18868,     19195,     19519,     19841,     20159,     20475,     20787,     21097,     21403,     21706,
     22005,     22301,     22594,     22884,     23170,     23453,     23732,     24007,     24279,     24547,
     24812,     25073,     25330,     25583,     25832,     26077,     26319,     26557,     26790,     27020,
     27245,     27466,     27684,     27897,     28106,     28310,     28511,     28707,     28898,     29086,
     29269,     29447,     29621,     29791,     29956,     30117,     30273,     30425,     30572,     30714,
     30852,     30985,     31114,     31237,     31357,     31471,     31581,     31685,     31785,     31881,
     31971,     32057,     32138,     32214,     32285,     32351,     32413,     32469,     32521,     32568,
     32610,     32647,     32679,     32706,     32728,     32745,     32758,     32765,     32768
};

uint16_t fast_sin_slope_table[128] = {
       402,       402,       402,       401,       402,       401,       401,       400,       400,       400,
       398,       399,       397,       397,       395,       395,       394,       393,       392,       390,
       390,       388,       387,       386,       384,       382,       381,       380,       377,       377,
       374,       372,       371,       368,       367,       365,       362,       360,       358,       356,
       354,       351,       348,       347,       343,       341,       339,       335,       333,       331,
       327,       324,       322,       318,       316,       312,       310,       306,       303,       299,
       296,       293,       290,       286,       283,       279,       275,       272,       268,       265,
       261,       257,       253,       249,       245,       242,       238,       233,       230,       225,
       221,       218,       213,       209,       204,       201,       196,       191,       188,       183,
       178,       174,       170,       165,       161,       156,       152,       147,       142,       138,
       133,       129,       123,       120,       114,       110,       104,       100,        96,        90,
        86,        81,        76,        71,        66,        62,        56,        52,        47,        42,
        37,        32,        27,        22,        17,        13,         7,         3
};

static uint32_t FastSinCore(uint32_t theta);

#endif /*USE_FAST_SIN_COS*/

#ifdef USE_FAST_TAN_COT
uint16_t fast_tan_value_table[65] = {
         0,       402,       804,      1206,      1609,      2013,      2417,      2821,      3227,      3633,
      4041,      4450,      4860,      5272,      5685,      6100,      6517,      6937,      7358,      7781,
      8207,      8636,      9068,      9502,      9940,     10380,     10825,     11272,     11724,     12180,
     12640,     13104,     13572,     14046,     14525,     15008,     15498,     15993,     16494,     17001,
     17514,     18035,     18562,     19097,     19640,     20191,     20750,     21317,     21894,     22481,
     23077,     23684,     24302,     24931,     25572,     26225,     26892,     27571,     28265,     28974,
     29699,     30439,     31197,     31973,     32767
};

uint16_t fast_tan_slope_table[64] = {
       402,       402,       402,       403,       404,       404,       404,       406,       406,       408,
       409,       410,       412,       413,       415,       417,       420,       421,       423,       426,
       429,       432,       434,       438,       440,       445,       447,       452,       456,       460,
       464,       468,       474,       479,       483,       490,       495,       501,       507,       513,
       521,       527,       535,       543,       551,       559,       567,       577,       587,       596,
       607,       618,       629,       641,       653,       667,       679,       694,       709,       725,
       740,       758,       776,       794
};

static uint32_t FastTanCore(uint32_t theta);

#endif /*USE_FAST_TAN_COT*/


#ifdef USE_FAST_ATAN
uint16_t fast_arctan_value_table[257] = {
         0,        40,        81,       122,       162,       203,       244,       285,       325,       366,
       407,       447,       488,       529,       569,       610,       651,       691,       732,       772,
       813,       853,       894,       934,       974,      1015,      1055,      1096,      1136,      1176,
      1216,      1256,      1297,      1337,      1377,      1417,      1457,      1497,      1537,      1576,
      1616,      1656,      1696,      1735,      1775,      1814,      1854,      1893,      1933,      1972,
      2011,      2051,      2090,      2129,      2168,      2207,      2246,      2285,      2323,      2362,
      2401,      2439,      2478,      2516,      2555,      2593,      2631,      2669,      2708,      2746,
      2784,      2821,      2859,      2897,      2935,      2972,      3010,      3047,      3084,      3122,
      3159,      3196,      3233,      3270,      3307,      3343,      3380,      3416,      3453,      3489,
      3526,      3562,      3598,      3634,      3670,      3706,      3742,      3777,      3813,      3848,
      3884,      3919,      3954,      3989,      4024,      4059,      4094,      4129,      4164,      4198,
      4233,      4267,      4301,      4335,      4369,      4403,      4437,      4471,      4505,      4538,
      4572,      4605,      4638,      4671,      4704,      4737,      4770,      4803,      4836,      4868,
      4901,      4933,      4965,      4997,      5029,      5061,      5093,      5125,      5156,      5188,
      5219,      5251,      5282,      5313,      5344,      5375,      5406,      5436,      5467,      5497,
      5528,      5558,      5588,      5618,      5648,      5678,      5708,      5738,      5767,      5797,
      5826,      5855,      5884,      5913,      5942,      5971,      6000,      6028,      6057,      6085,
      6114,      6142,      6170,      6198,      6226,      6254,      6282,      6309,      6337,      6364,
      6391,      6419,      6446,      6473,      6500,      6527,      6553,      6580,      6606,      6633,
      6659,      6685,      6711,      6737,      6763,      6789,      6815,      6841,      6866,      6892,
      6917,      6942,      6967,      6992,      7017,      7042,      7067,      7092,      7116,      7141,
      7165,      7190,      7214,      7238,      7262,      7286,      7310,      7333,      7357,      7381,
      7404,      7428,      7451,      7474,      7497,      7520,      7543,      7566,      7589,      7611,
      7634,      7657,      7679,      7701,      7724,      7746,      7768,      7790,      7812,      7833,
      7855,      7877,      7898,      7920,      7941,      7963,      7984,      8005,      8026,      8047,
      8068,      8089,      8109,      8130,      8151,      8171,      8192
};

uint8_t fast_arctan_slope_table[256] = {
        40,        41,        41,        40,        41,        41,        41,        40,        41,        41,
        40,        41,        41,        40,        41,        41,        40,        41,        40,        41,
        40,        41,        40,        40,        41,        40,        41,        40,        40,        40,
        40,        41,        40,        40,        40,        40,        40,        40,        39,        40,
        40,        40,        39,        40,        39,        40,        39,        40,        39,        39,
        40,        39,        39,        39,        39,        39,        39,        38,        39,        39,
        38,        39,        38,        39,        38,        38,        38,        39,        38,        38,
        37,        38,        38,        38,        37,        38,        37,        37,        38,        37,
        37,        37,        37,        37,        36,        37,        36,        37,        36,        37,
        36,        36,        36,        36,        36,        36,        35,        36,        35,        36,
        35,        35,        35,        35,        35,        35,        35,        35,        34,        35,
        34,        34,        34,        34,        34,        34,        34,        34,        33,        34,
        33,        33,        33,        33,        33,        33,        33,        33,        32,        33,
        32,        32,        32,        32,        32,        32,        32,        31,        32,        31,
        32,        31,        31,        31,        31,        31,        30,        31,        30,        31,
        30,        30,        30,        30,        30,        30,        30,        29,        30,        29,
        29,        29,        29,        29,        29,        29,        28,        29,        28,        29,
        28,        28,        28,        28,        28,        28,        27,        28,        27,        27,
        28,        27,        27,        27,        27,        26,        27,        26,        27,        26,
        26,        26,        26,        26,        26,        26,        26,        25,        26,        25,
        25,        25,        25,        25,        25,        25,        25,        24,        25,        24,
        25,        24,        24,        24,        24,        24,        23,        24,        24,        23,
        24,        23,        23,        23,        23,        23,        23,        23,        22,        23,
        23,        22,        22,        23,        22,        22,        22,        22,        21,        22,
        22,        21,        22,        21,        22,        21,        21,        21,        21,        21,
        21,        20,        21,        21,        20,        21
};


static uint32_t FastAtanCore(uint32_t k);

#endif /*USE_FAST_ATAN*/

#ifdef USE_FAST_ASIN_ACOS

uint16_t fast_arcsin_normal_value_table[241] = {
         0,        40,        81,       122,       162,       203,       244,       285,       326,       366,
       407,       448,       489,       529,       570,       611,       652,       693,       733,       774,
       815,       856,       897,       938,       979,      1020,      1061,      1102,      1143,      1184,
      1225,      1266,      1307,      1348,      1389,      1430,      1471,      1512,      1554,      1595,
      1636,      1677,      1719,      1760,      1801,      1843,      1884,      1925,      1967,      2008,
      2050,      2091,      2133,      2175,      2216,      2258,      2300,      2342,      2383,      2425,
      2467,      2509,      2551,      2593,      2635,      2677,      2719,      2761,      2804,      2846,
      2888,      2931,      2973,      3016,      3058,      3101,      3143,      3186,      3229,      3272,
      3315,      3357,      3400,      3443,      3487,      3530,      3573,      3616,      3660,      3703,
      3747,      3790,      3834,      3877,      3921,      3965,      4009,      4053,      4097,      4141,
      4185,      4230,      4274,      4318,      4363,      4408,      4452,      4497,      4542,      4587,
      4632,      4677,      4723,      4768,      4813,      4859,      4905,      4950,      4996,      5042,
      5088,      5134,      5181,      5227,      5274,      5320,      5367,      5414,      5461,      5508,
      5555,      5603,      5650,      5698,      5745,      5793,      5841,      5889,      5938,      5986,
      6035,      6084,      6132,      6181,      6231,      6280,      6330,      6379,      6429,      6479,
      6529,      6580,      6630,      6681,      6732,      6783,      6834,      6886,      6937,      6989,
      7041,      7094,      7146,      7199,      7252,      7305,      7358,      7412,      7466,      7520,
      7574,      7629,      7684,      7739,      7795,      7850,      7906,      7962,      8019,      8076,
      8133,      8190,      8248,      8306,      8365,      8423,      8483,      8542,      8602,      8662,
      8723,      8784,      8845,      8907,      8969,      9032,      9095,      9158,      9223,      9287,
      9352,      9418,      9484,      9550,      9617,      9685,      9753,      9822,      9892,      9962,
     10033,     10105,     10177,     10250,     10324,     10399,     10474,     10550,     10628,     10706,
     10785,     10865,     10946,     11029,     11112,     11197,     11283,     11371,     11460,     11550,
     11642,     11736,     11831,     11928,     12028,     12129,     12233,     12340,     12449,     12561,
     12676
};

uint8_t fast_arcsin_normal_slope_table[240] = {
        40,        41,        41,        40,        41,        41,        41,        41,        40,        41,
        41,        41,        40,        41,        41,        41,        41,        40,        41,        41,
        41,        41,        41,        41,        41,        41,        41,        41,        41,        41,
        41,        41,        41,        41,        41,        41,        41,        42,        41,        41,
        41,        42,        41,        41,        42,        41,        41,        42,        41,        42,
        41,        42,        42,        41,        42,        42,        42,        41,        42,        42,
        42,        42,        42,        42,        42,        42,        42,        43,        42,        42,
        43,        42,        43,        42,        43,        42,        43,        43,        43,        43,
        42,        43,        43,        44,        43,        43,        43,        44,        43,        44,
        43,        44,        43,        44,        44,        44,        44,        44,        44,        44,
        45,        44,        44,        45,        45,        44,        45,        45,        45,        45,
        45,        46,        45,        45,        46,        46,        45,        46,        46,        46,
        46,        47,        46,        47,        46,        47,        47,        47,        47,        47,
        48,        47,        48,        47,        48,        48,        48,        49,        48,        49,
        49,        48,        49,        50,        49,        50,        49,        50,        50,        50,
        51,        50,        51,        51,        51,        51,        52,        51,        52,        52,
        53,        52,        53,        53,        53,        53,        54,        54,        54,        54,
        55,        55,        55,        56,        55,        56,        56,        57,        57,        57,
        57,        58,        58,        59,        58,        60,        59,        60,        60,        61,
        61,        61,        62,        62,        63,        63,        63,        65,        64,        65,
        66,        66,        66,        67,        68,        68,        69,        70,        70,        71,
        72,        72,        73,        74,        75,        75,        76,        78,        78,        79,
        80,        81,        83,        83,        85,        86,        88,        89,        90,        92,
        94,        95,        97,       100,       101,       104,       107,       109,       112,       115,
};

uint16_t fast_arcsin_odd_value_table[257] = {
     12676,     12684,     12691,     12698,     12706,     12713,     12720,     12728,     12735,     12743,
     12750,     12758,     12765,     12773,     12780,     12788,     12795,     12803,     12810,     12818,
     12826,     12833,     12841,     12848,     12856,     12864,     12872,     12879,     12887,     12895,
     12902,     12910,     12918,     12926,     12934,     12942,     12949,     12957,     12965,     12973,
     12981,     12989,     12997,     13005,     13013,     13021,     13029,     13037,     13045,     13053,
     13061,     13070,     13078,     13086,     13094,     13102,     13111,     13119,     13127,     13135,
     13144,     13152,     13160,     13169,     13177,     13186,     13194,     13203,     13211,     13220,
     13228,     13237,     13245,     13254,     13263,     13271,     13280,     13289,     13297,     13306,
     13315,     13324,     13332,     13341,     13350,     13359,     13368,     13377,     13386,     13395,
     13404,     13413,     13422,     13431,     13440,     13449,     13459,     13468,     13477,     13486,
     13496,     13505,     13514,     13524,     13533,     13543,     13552,     13562,     13571,     13581,
     13590,     13600,     13610,     13619,     13629,     13639,     13649,     13658,     13668,     13678,
     13688,     13698,     13708,     13718,     13728,     13738,     13749,     13759,     13769,     13779,
     13790,     13800,     13810,     13821,     13831,     13842,     13853,     13863,     13874,     13884,
     13895,     13906,     13917,     13928,     13939,     13950,     13961,     13972,     13983,     13994,
     14005,     14017,     14028,     14039,     14051,     14062,     14074,     14086,     14097,     14109,
     14121,     14133,     14145,     14157,     14169,     14181,     14193,     14205,     14218,     14230,
     14242,     14255,     14267,     14280,     14293,     14306,     14319,     14332,     14345,     14358,
     14371,     14384,     14398,     14411,     14425,     14439,     14452,     14466,     14480,     14494,
     14509,     14523,     14537,     14552,     14566,     14581,     14596,     14611,     14626,     14641,
     14657,     14672,     14688,     14704,     14720,     14736,     14752,     14769,     14785,     14802,
     14819,     14836,     14853,     14871,     14889,     14906,     14925,     14943,     14962,     14980,
     15000,     15019,     15039,     15059,     15079,     15099,     15120,     15142,     15163,     15185,
     15208,     15231,     15254,     15278,     15302,     15327,     15352,     15378,     15405,     15433,
     15461,     15491,     15521,     15552,     15585,     15619,     15655,     15692,     15731,     15774,
     15819,     15868,     15923,     15984,     16058,     16153,     16384
};

uint8_t fast_arcsin_odd_slope_table[256] = {
         8,         7,         7,         8,         7,         7,         8,         7,         8,         7,
         8,         7,         8,         7,         8,         7,         8,         7,         8,         8,
         7,         8,         7,         8,         8,         8,         7,         8,         8,         7,
         8,         8,         8,         8,         8,         7,         8,         8,         8,         8,
         8,         8,         8,         8,         8,         8,         8,         8,         8,         8,
         9,         8,         8,         8,         8,         9,         8,         8,         8,         9,
         8,         8,         9,         8,         9,         8,         9,         8,         9,         8,
         9,         8,         9,         9,         8,         9,         9,         8,         9,         9,
         9,         8,         9,         9,         9,         9,         9,         9,         9,         9,
         9,         9,         9,         9,         9,        10,         9,         9,         9,        10,
         9,         9,        10,         9,        10,         9,        10,         9,        10,         9,
        10,        10,         9,        10,        10,        10,         9,        10,        10,        10,
        10,        10,        10,        10,        10,        11,        10,        10,        10,        11,
        10,        10,        11,        10,        11,        11,        10,        11,        10,        11,
        11,        11,        11,        11,        11,        11,        11,        11,        11,        11,
        12,        11,        11,        12,        11,        12,        12,        11,        12,        12,
        12,        12,        12,        12,        12,        12,        12,        13,        12,        12,
        13,        12,        13,        13,        13,        13,        13,        13,        13,        13,
        13,        14,        13,        14,        14,        13,        14,        14,        14,        15,
        14,        14,        15,        14,        15,        15,        15,        15,        15,        16,
        15,        16,        16,        16,        16,        16,        17,        16,        17,        17,
        17,        17,        18,        18,        17,        19,        18,        19,        18,        20,
        19,        20,        20,        20,        20,        21,        22,        21,        22,        23,
        23,        23,        24,        24,        25,        25,        26,        27,        28,        28,
        30,        30,        31,        33,        34,        36,        37,        39,        43,        45,
        49,        55,        61,        74,        95,       231
};

static uint32_t FastAsinOddCore(uint32_t k);
static uint32_t FastAsinNormalCore(uint32_t k);

#endif /*USE_FAST_ASIN_ACOS*/






#define STEP_SHIFT (7)
#define STEP_MASK (127)
#define STEP_LEN (128)

//#define RANGE(value, start, end) (																			\
//	((value) >= (start))?																					\
//	(																										\
//		(  ( (value) - (start) ) % ( (end) - (start) )  ) + (start)											\
//	):																										\
//	(																										\
//		(  (end) - (start) - ( (end) - (value) ) % ( (end) - (start) )  ) % ( (end) - (value) ) + (start)	\
//	)																										\
//)




#ifdef USE_FAST_ATAN

uint32_t FastAtanCore(uint32_t k)
{
	uint32_t index = k >> STEP_SHIFT;
	uint32_t theta = ((fast_arctan_slope_table[index & 0xFF] * (k & STEP_MASK)) >> STEP_SHIFT) + fast_arctan_value_table[index];
	return theta;
}

inline float FastAtanF32(float k)
{
	return (float)FastAtanQ31((int32_t)(k * 32768.0)) * PI / 32768.0;
}

inline int32_t FastAtanF32OutputQ31(float k)
{
	return FastAtanQ31((int32_t)(k * 32768.0));
}

inline float FastAtanQ31OutputF32(int32_t k)
{
	return (float)FastAtanQ31((int32_t)(k)) * PI / 32768.0;;
}



int32_t FastAtanQ31(int32_t k)
{
	uint8_t phase = 0;
	int32_t theta = 0;
	if (k < 0)
		phase |= 0x01;
	if (abs(k) > 32768)
		phase |= 0x02;
	
	switch (phase)
	{
	case 0x00:
		theta = FastAtanCore(k);
		break;
	case 0x01:
		theta = -FastAtanCore(-k);
		break;
	case 0x02:
		theta = 16384 - FastAtanCore(0x40000000/k);
		break;
	case 0x03:
		theta = FastAtanCore(0x40000000 / (-k)) - 16384;
		break;
	default:
		break;
	}

	return theta;
}


inline float FastAtan2F32(float sin, float cos)
{
	return (float) FastAtan2Q31(sin * 32768.0, cos * 32768.0) * PI / 32768.0;
}

inline int32_t FastAtan2F32OutputQ31(float sin, float cos)
{
	return FastAtan2Q31(sin * 32768.0, cos * 32768.0);
}

inline float FastAtan2Q31OutputF32(int32_t sin, int32_t cos)
{
	return (float) FastAtan2Q31(sin, cos) * PI / 32768.0;
}

/**
 * @brief A fixed-point inverse tangent function implement, consider the sign of both two input parameters.
 *        Use look up table algorithm with linear interpolation.
 *        Max error compare with atan2 (in C std library) is +/-0.001749 degree.
 *        Time consumption: 1.73us min, 3.5us max in a 72MHz Cortex-M3 CPU
 *         (when input value is very large, it takes the program some time to
 *        adjust them into right range).
 * @param x_in: x value. If parameter is out of range -32768~32767, it can be automatically adjusted into the right range.
 * @param y_in: y value. If parameter is out of range -32768~32767, it can be automatically adjusted into the right range.
 * @return atan2(y,x) range from -32768~32767 (as -pi ~ +pi)
 * @author shiqian.dong
 */
int32_t FastAtan2Q31(int32_t sin, int32_t cos)
{
	uint32_t a, b, k;
	int32_t theta;
	uint8_t phase = 0;
	if (sin < 0)
		phase |= 0x01;
	if (cos < 0)
		phase |= 0x02;

	sin = abs(sin);
	cos = abs(cos);
	if (sin > cos)
		phase |= 0x04;




	a = (sin > cos) ? (cos) : (sin);
	b = (sin > cos) ? (sin) : (cos);

	k = (a << 15) / b;

	theta = FastAtanCore(k);
	//return theta;

	switch (phase)
	{
	case 0x00:
		theta = theta;
		break;


	case 0x01:
		theta = - theta;
		break;

	case 0x02:
		theta = 32768 - theta;
		break;

	case 0x03:
		theta = theta - 32768;
		break;

	case 0x04:
		theta = 16384 - theta;
		break;

	case 0x05:
		theta = -16384 + theta;
		break;

	case 0x06:
		theta = 16384 + theta;
		break;

	case 0x07:
		theta = -16384 - theta;
		break;
	}

	return theta;
}

#endif /*USE_FAST_ATAN*/





#ifdef USE_FAST_ASIN_ACOS
static uint32_t FastAsinNormalCore(uint32_t k)
{
	uint32_t index = k >> STEP_SHIFT;
	uint32_t theta = ((fast_arcsin_normal_slope_table[index % 240] * (k & STEP_MASK)) >> STEP_SHIFT) + fast_arcsin_normal_value_table[index];
	return theta;
}


static uint32_t FastAsinOddCore(uint32_t k)
{
	uint32_t index = k >> 3;
	uint32_t theta = ((fast_arcsin_odd_slope_table[index & 0xFF] * (k & 7)) >> 3) + fast_arcsin_odd_value_table[index];
	return theta;
}


/**
  * @brief A fast arcsin function implementation
  * @param x: input value, range from -65536 to +65536, as -1.0f to +1.0f.
  * @return arcsin(x), range from -32768 to +32768, as -pi/2 to +pi/2
  * @throw if x out of the range -65536 ~ +65536, it returns 32768.
  */
//int32_t FastAsinF

int32_t FastAsinQ31(int32_t k)
{
	int32_t theta = 0;
	if (k >= 32768){
		theta = 16384;
		return theta;
	}
	else if (k <= -32768){
		theta = -16384;
		return theta;
	}


	if (abs(k) >= 30720){
		if (k > 0){
			theta = FastAsinOddCore(k-30720);
		}
		else{
			theta = -FastAsinOddCore(-k-30720);
		}
		return theta;
	}
	else
	{
		if (k >= 0){
			theta = FastAsinNormalCore(k);
			return theta;
		}
		else{
			theta = -FastAsinNormalCore(-k);
			return theta;
		}
	}
}

/**
* @brief A fast arccos function implementation
* @param k: input value, range from -65536 to +65536, as -1.0f to +1.0f.
* @return arccos(x), range from -32768 to +32768, as -pi/2 to +pi/2
* @throw if x out of the range -65536 ~ +65536, it returns 0x00008000.
*/
 inline int32_t FastAcosQ31(int32_t k)
{
	int32_t theta = 0;
	if (k >= 32768)
		return 0;
	else if (k <= -32768)
		return 32768;
	theta = FastAsinQ31(k);
	theta = 16384 - theta;
	theta = RANGE(theta, 0, 32768);
	return theta;
}

inline float FastAsinF32(float k)
{
	return (float)FastAsinQ31((int32_t)(k * 32768.0f)) / 32768.0f * PI;
}

inline int32_t FastAsinF32OutputQ31(float k)
{
	return FastAsinQ31((int32_t)(k * 32768.0f));
}

inline float FastAsinQ31OutputF32(int32_t k)
{
	return (float)FastAsinQ31(k) / 32768.0f * PI;
}

inline float FastAcosF32(float k)
{
	return (float)FastAcosQ31((int32_t)(k * 32768.0f)) /32768.0f * PI;
}

inline int32_t FastAcosF32OutputQ31(float k)
{
	return FastAcosQ31((int32_t)(k * 32768.0f));
}

inline float FastAcosQ31OutputF32(int32_t k)
{
	return (float)FastAcosQ31(k) / 32768.0f * PI;
}

#endif /*USE_FAST_ASIN_ACOS*/

#ifdef USE_FAST_SIN_COS

static uint32_t FastSinCore(uint32_t theta)
{
	uint32_t index = theta >> STEP_SHIFT;
	uint32_t k = ((fast_sin_slope_table[index & 0xFF] * (theta & STEP_MASK)) >> STEP_SHIFT) + fast_sin_value_table[index];
	return k;
}


int32_t FastSinQ31(int32_t theta)
{
	theta = RANGE(theta, 0, 65536);
	uint8_t phase = theta / 16384;
	int32_t k = 0;
	switch (phase)
	{
	case 0:
		k = FastSinCore(theta);
		break;
	case 1:
		k = FastSinCore(32768 - theta);
		break;
	case 2:
		k = -FastSinCore(theta - 32768);
		break;
	case 3:
		k = -FastSinCore(65536 - theta);
		break;
	default:
		break;
	}
	return k;
}

inline int32_t FastCosQ31(int32_t theta)
{
	return FastSinQ31(16384 - theta);
}

inline float FastSinF32(float theta)
{	
	return (float)FastSinQ31((int32_t)(theta / PI * 32768.0f)) / 32768.0f;	
}

inline float FastCosF32(float theta)
{
	return (float)FastCosQ31((int32_t)(theta / PI * 32768.0f)) / 32768.0f;
}

inline int32_t FastSinF32OutputQ31(float theta)
{	
	return FastSinQ31((int32_t)(theta / PI * 32768.0f));	
}

inline float FastSinQ31OutputF32(int32_t theta)
{	
	return (float)FastSinQ31(theta) / 32768.0f;	
}

inline int32_t FastCosF32OutputQ31(float theta)
{	
	return FastCosQ31((int32_t)(theta / PI * 32768.0f));	
}

inline float FastCosQ31OutputF32(int32_t theta)
{	
	return (float)FastCosQ31(theta) / 32768.0f;	
}



#endif /*USE_FAST_SIN_COS*/


#ifdef USE_FAST_TAN_COT

static uint32_t FastTanCore(uint32_t theta)
{
	uint32_t index = theta >> STEP_SHIFT;
	uint32_t k = ((fast_tan_slope_table[index & 0x3F] * (theta & STEP_MASK)) >> STEP_SHIFT) + fast_tan_value_table[index];
	return k;
}

int32_t FastTanQ31(int32_t theta)
{
	theta = RANGE(theta, 0, 32768);
	uint8_t phase = theta / 8192;
	int32_t k = 0;
	if (theta % 16384 == 0)
		return 0;
	switch (phase)
	{
	case 0:
		k = FastTanCore(theta);
		break;
	case 1:
		k = 0x40000000 / FastTanCore(16384 - theta);
		break;
	case 2:
		k = - ((int32_t)0x40000000 / (int32_t)FastTanCore(theta - 16384));
		//k = 0;
		break;
	case 3:
		k = - (int32_t)FastTanCore(32768 - theta);
		//k = 0;
		break;
	default:
		break;
	}
	return k;
	
}

inline float FastTanF32(float theta)
{
	return (float)FastTanQ31((int32_t)(theta / PI * 32768.0f)) / 32768.0f;
}

inline int32_t FastCotQ31(int32_t theta)
{
	return -FastTanQ31(theta - 16384);
}

inline float FastCotF32(float theta)
{
	return (float)FastCotQ31((int32_t)(theta / PI * 32768.0f)) / 32768.0f;
}




#endif /*USE_FAST_TAN_COT*/




const int32_t isqrtTable_I32[29] = {
		46339 , 41447 , 37836 , 35029 , 32767 , 30893 , 29307 , 27943 ,
		26754 , 25704 , 24769 , 23929 , 23169 , 22477 , 21844 , 21262 ,
		20723 , 20224 , 19759 , 19324 , 18918 , 18535 , 18175 , 17836 ,
		17514 , 17210 , 16920 , 16645 , 16383 };

const int32_t isqrtSlopeTable_I32[28] = {
		-39137 , -28890 , -22453 , -18099 , -14991 , -12682 , -10911 , -9517 ,
		-8396 , -7480 , -6719 , -6078 , -5534 , -5066 , -4661 , -4306 ,
		-3995 , -3719 , -3474 , -3255 , -3057 , -2879 , -2718 , -2571 ,
		-2437 , -2314 , -2201 , -2097 };

#define ISQRT_MINIINPUT 16384
#define ISQRT_MAXINPUT 131068
/**
  * @brief A fixed-point inverse square root function implement.
  *        Use look up table algorithm with linear interpolation.
  *        It is designed to use in cycle limitation in SVPWM of a 3-phase inverter.
  *        Time consumption: 0.85us in a 72MHz Cortex-M3 CPU
  *
  * @param x_in: x value. Range from 16384~98310(as 0.5f~4.0f). If out of range, it returns the limit bound.
  * @return square root of x. If input value out of range, returns zero.
  * @author shiqian.dong
  */
int32_t FastIsqrtQ31(int32_t x)
{
    static int32_t offset, bais, index, result;

    if(x < ISQRT_MINIINPUT)
    {
        return 46339; 	//isqrt(0.5)
    }
    if(x > ISQRT_MAXINPUT)
    {
        return 16384;   //isqrt(4)
    }
    x -= ISQRT_MINIINPUT;
    index = x >> 12;

    bais = isqrtTable_I32[index];
    offset = x - index * 4096;// x - (index << 10);
    result = bais + ((isqrtSlopeTable_I32[index] * offset) >> 15);

    return (result);
}



float FastInvSqrtF32(float x)
{
	long i;
	float x2, y;
	const float threehalfs = 1.5F;

	x2 = x * 0.5F;
	y  = x;
	i  = * ( long * ) &y;                       // evil floating point bit level hacking
	i  = 0x5f3759df - ( i >> 1 );               // what the fuck? 
	y  = * ( float * ) &i;
	y  = y * ( threehalfs - ( x2 * y * y ) );   // 1st iteration
//	y  = y * ( threehalfs - ( x2 * y * y ) );   // 2nd iteration, this can be removed

	return y;
}

float FastSqrtF32(float x)
{
	return 1.0f / FastInvSqrtF32(x);
}

uint16_t Byte_Contact(uint8_t h, uint8_t l)
{
	return ( ((uint16_t)h << 8 ) + (uint16_t)l );
}

void Fill_u16_To_u8(uint16_t x, uint8_t* h, uint8_t* l)
{
	*h = (uint8_t)(x >> 8);
	*l = (uint8_t)(x >> 0);
}
