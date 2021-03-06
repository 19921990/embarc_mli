/*
* Copyright 2019-2020, Synopsys, Inc.
* All rights reserved.
*
* This source code is licensed under the BSD-3-Clause license found in
* the LICENSE file in the root directory of this source tree.
*
*/

#include "mli_prv_lut.h"

#include "mli_config.h"
#include "mli_debug.h"
#include "mli_math.h"
#include "mli_prv_dsp.h"
#include "mli_prv_load_store.h"

#define MAX_FRAC_BITS_FX16 ((sizeof(int16_t) * 8) - 1)
#define MAX_FRAC_BITS_FX8 ((sizeof(int8_t) * 8) - 1)

/*
    *** Generated by lutfx.py ***
    arguments  = -t FX16 -o size/2 -s 180 -qi 3 -f 'sigm(x)'
    lut_size   = 180
    in_offset  = 90
    in_format  = Q4.3
    out_format = Q15
    out_error  = 0.000191 (6 lsb) (linear interpolation)
*/
static const int16_t sigmoid_lut_data_fx16[] = {
        +0,     +0,     +1,     +1,     +1,     +1,     +1,     +1,     +1,     +1,     +1,     +2,     +2,     +2,
        +2,     +3,     +3,     +4,     +4,     +5,     +5,     +6,     +7,     +8,     +9,     +10,    +11,    +12,
        +14,    +16,    +18,    +21,    +23,    +26,    +30,    +34,    +38,    +43,    +49,    +56,    +63,    +72,
        +81,    +92,    +104,   +118,   +133,   +151,   +171,   +194,   +219,   +248,   +281,   +318,   +360,   +407,
        +461,   +521,   +589,   +666,   +753,   +851,   +961,   +1084,  +1223,  +1379,  +1554,  +1750,  +1969,  +2213,
        +2486,  +2789,  +3124,  +3496,  +3906,  +4357,  +4851,  +5391,  +5978,  +6613,  +7297,  +8031,  +8813,  +9641,
        +10513, +11424, +12371, +13348, +14347, +15361, +16384, +17407, +18421, +19420, +20397, +21344, +22255, +23127,
        +23955, +24737, +25471, +26155, +26790, +27377, +27917, +28411, +28862, +29272, +29644, +29979, +30282, +30555,
        +30799, +31018, +31214, +31389, +31545, +31684, +31807, +31917, +32015, +32102, +32179, +32247, +32307, +32361,
        +32408, +32450, +32487, +32520, +32549, +32574, +32597, +32617, +32635, +32650, +32664, +32676, +32687, +32696,
        +32705, +32712, +32719, +32725, +32730, +32734, +32738, +32742, +32745, +32747, +32750, +32752, +32754, +32756,
        +32757, +32758, +32759, +32760, +32761, +32762, +32763, +32763, +32764, +32764, +32765, +32765, +32766, +32766,
        +32766, +32766, +32767, +32767, +32767, +32767, +32767, +32767, +32767, +32767, +32767, +32767};

const mli_lut sigmoid_lut_fx16 = {
        .data = sigmoid_lut_data_fx16,
        .type = MLI_EL_FX_16,
        .length = sizeof(sigmoid_lut_data_fx16) / sizeof(sigmoid_lut_data_fx16[0]),
        .frac_bits = 3,
        .offset = sizeof(sigmoid_lut_data_fx16) / (sizeof(sigmoid_lut_data_fx16[0]) * 2) /*center-aligned */
};

/*
    *** Generated by lutfx.py ***
    arguments  = -t FX16 -o size/2 -s 178 -qi 4 -f 'tanh(x)'
    lut_size   = 178
    in_offset  = 89
    in_format  = Q3.4
    out_format = Q15
    out_error  = 0.000377 (12 lsb) (linear interpolation)
*/
static const int16_t tanh_lut_data_fx16[] = {
        -32767, -32767, -32767, -32767, -32766, -32766, -32766, -32766, -32765, -32765, -32765, -32764, -32764, -32763,
        -32762, -32762, -32761, -32760, -32759, -32758, -32756, -32755, -32753, -32751, -32749, -32746, -32743, -32740,
        -32736, -32732, -32727, -32721, -32715, -32708, -32700, -32691, -32681, -32670, -32657, -32642, -32625, -32606,
        -32584, -32560, -32532, -32501, -32466, -32426, -32381, -32329, -32271, -32206, -32132, -32048, -31953, -31846,
        -31726, -31589, -31435, -31262, -31067, -30847, -30600, -30322, -30010, -29660, -29268, -28830, -28341, -27797,
        -27191, -26519, -25776, -24956, -24054, -23066, -21986, -20813, -19542, -18173, -16706, -15143, -13486, -11743,
        -9919,  -8025,  -6073,  -4075,  -2045,  +0,     +2045,  +4075,  +6073,  +8025,  +9919,  +11743, +13486, +15143,
        +16706, +18173, +19542, +20813, +21986, +23066, +24054, +24956, +25776, +26519, +27191, +27797, +28341, +28830,
        +29268, +29660, +30010, +30322, +30600, +30847, +31067, +31262, +31435, +31589, +31726, +31846, +31953, +32048,
        +32132, +32206, +32271, +32329, +32381, +32426, +32466, +32501, +32532, +32560, +32584, +32606, +32625, +32642,
        +32657, +32670, +32681, +32691, +32700, +32708, +32715, +32721, +32727, +32732, +32736, +32740, +32743, +32746,
        +32749, +32751, +32753, +32755, +32756, +32758, +32759, +32760, +32761, +32762, +32762, +32763, +32764, +32764,
        +32765, +32765, +32765, +32766, +32766, +32766, +32766, +32767, +32767, +32767};

const mli_lut tanh_lut_fx16 = {
        .data = tanh_lut_data_fx16,
        .type = MLI_EL_FX_16,
        .length = sizeof(tanh_lut_data_fx16) / sizeof(tanh_lut_data_fx16[0]),
        .frac_bits = 4,
        .offset = sizeof(tanh_lut_data_fx16) / (sizeof(tanh_lut_data_fx16[0]) * 2) /*center-aligned */
};

/*
    *** Generated by lutfx.py ***
    arguments  = -t FX16 -o size-2 -s 181 -qi 4 -f 'expneg(x)'
    lut_size   = 181
    in_offset  = 179
    in_format  = Q3.4
    out_format = Q15
    out_error  = 0.000463 (15 lsb) (linear interpolation)
*/
static const int16_t expneg_lut_data_fx16[] = {
        +0,     +0,     +1,     +1,     +1,     +1,     +1,     +1,     +1,     +1,     +1,     +1,     +1,     +1,
        +1,     +1,     +1,     +1,     +1,     +1,     +2,     +2,     +2,     +2,     +2,     +2,     +2,     +2,
        +3,     +3,     +3,     +3,     +3,     +4,     +4,     +4,     +4,     +5,     +5,     +5,     +6,     +6,
        +6,     +7,     +7,     +8,     +8,     +9,     +9,     +10,    +10,    +11,    +12,    +12,    +13,    +14,
        +15,    +16,    +17,    +18,    +19,    +21,    +22,    +23,    +25,    +26,    +28,    +30,    +32,    +34,
        +36,    +38,    +41,    +43,    +46,    +49,    +52,    +56,    +59,    +63,    +67,    +72,    +76,    +81,
        +86,    +92,    +98,    +104,   +111,   +118,   +126,   +134,   +143,   +152,   +162,   +172,   +183,   +195,
        +207,   +221,   +235,   +250,   +266,   +283,   +302,   +321,   +342,   +364,   +387,   +412,   +439,   +467,
        +498,   +530,   +564,   +600,   +639,   +680,   +724,   +771,   +820,   +873,   +930,   +990,   +1053,  +1121,
        +1194,  +1271,  +1352,  +1440,  +1533,  +1631,  +1737,  +1849,  +1968,  +2095,  +2230,  +2374,  +2527,  +2690,
        +2863,  +3048,  +3244,  +3454,  +3676,  +3914,  +4166,  +4435,  +4721,  +5025,  +5349,  +5694,  +6061,  +6452,
        +6869,  +7312,  +7783,  +8285,  +8819,  +9388,  +9994,  +10638, +11324, +12055, +12832, +13660, +14541, +15479,
        +16477, +17539, +18671, +19875, +21157, +22521, +23974, +25520, +27166, +28918, +30783, +32767, +32767};

const mli_lut expneg_lut_fx16 = {
        .data = expneg_lut_data_fx16,
        .type = MLI_EL_FX_16,
        .length = sizeof(expneg_lut_data_fx16) / sizeof(expneg_lut_data_fx16[0]),
        .frac_bits = 4,
        .offset = sizeof(expneg_lut_data_fx16) / sizeof(expneg_lut_data_fx16[0]) - 2 /*right-minus-one-aligned */
};

#ifdef __cplusplus
extern "C" {
#endif

#pragma Code(".mli_lib")

void mli_prv_activation_lut_fx16(
        const MLI_PTR(int16_t) in,
        MLI_OUT_PTR(int16_t) out,
        const mli_lut *lut,
        int in_frac_bits,
        int length) {
    MLI_ASSERT(in_frac_bits >= -1);  // -1 may be required by softmax
    MLI_ASSERT(length >= 0);
    MLI_ASSERT(lut->frac_bits >= 0);
    MLI_ASSERT(lut->length >= 0);

    int shift_in = in_frac_bits - lut->frac_bits;
    int16_t *lut_data = (int16_t *)lut->data;
    // if shift amount is too high, preshift argument itself and
    // limit shift amount to prevent overflows
    int preshift_in = MAX(shift_in - (int)MAX_FRAC_BITS_FX16, 0);
    shift_in = MIN(shift_in, (int)MAX_FRAC_BITS_FX16);

    v2q15_t offset = fx_replic_v2q15(lut->offset);
    v2q15_t lower = fx_replic_v2q15(0);

    if (shift_in > 0) {
        // input data is more precise than LUT
        v2q15_t mask = fx_replic_v2q15((1 << shift_in) - 1);
        v2q15_t upper = fx_replic_v2q15(lut->length - 2);

        if (length & 1) {
            v2q15_t x = fx_asr_v2q15_n(mli_prv_load_1_sample(in), preshift_in);
            v2q15_t lut_idx = fx_add_v2q15(fx_asr_v2q15_n(x, shift_in), offset);
            lut_idx = fx_max_v2q15(lut_idx, lower);
            lut_idx = fx_min_v2q15(lut_idx, upper);
            // perform linear interpolation
            v2q15_t frac = x & mask;
            v2q15_t res = fx_create_v2q15(lut_data[lut_idx[0]], lut_data[lut_idx[1]]);
            v2q15_t next = fx_create_v2q15(lut_data[lut_idx[0] + 1], lut_data[lut_idx[1] + 1]);
            v2q15_t diff = fx_sub_v2q15(res, next);
            mli_prv_store_1_sample(
                    out,
                    fx_sub_v2q15(res, fx_v2q15_cast_nf_asr_rnd_v2a40(fx_v2a40_mpy_v2q15(diff, frac), shift_in - 16)));
            in += 1;
            out += 1;
        }
        for (int idx = 0; idx < (length >> 1); idx++) {
            v2q15_t x = fx_asr_v2q15_n(mli_prv_load_2_samples(in), preshift_in);
            v2q15_t lut_idx = fx_add_v2q15(fx_asr_v2q15_n(x, shift_in), offset);
            lut_idx = fx_max_v2q15(lut_idx, lower);
            lut_idx = fx_min_v2q15(lut_idx, upper);
            // perform linear interpolation
            v2q15_t frac = x & mask;
            v2q15_t res = fx_create_v2q15(lut_data[lut_idx[0]], lut_data[lut_idx[1]]);
            v2q15_t next = fx_create_v2q15(lut_data[lut_idx[0] + 1], lut_data[lut_idx[1] + 1]);
            v2q15_t diff = fx_sub_v2q15(res, next);
            mli_prv_store_2_samples(
                    out,
                    fx_sub_v2q15(res, fx_v2q15_cast_nf_asr_rnd_v2a40(fx_v2a40_mpy_v2q15(diff, frac), shift_in - 16)));
            in += 2;
            out += 2;
        }
    } else {
        // input data isn't more precise than LUT
        v2q15_t upper = fx_replic_v2q15(lut->length - 1);

        if (length & 1) {
            v2q15_t x = mli_prv_load_1_sample(in);
            v2q15_t lut_idx = fx_add_v2q15(fx_asr_v2q15_n(x, shift_in), offset);
            lut_idx = fx_max_v2q15(lut_idx, lower);
            lut_idx = fx_min_v2q15(lut_idx, upper);
            // no interpolation
            mli_prv_store_1_sample(out, fx_create_v2q15(lut_data[lut_idx[0]], lut_data[lut_idx[1]]));
            in += 1;
            out += 1;
        }
        for (int idx = 0; idx < (length >> 1); idx++) {
            v2q15_t x = mli_prv_load_2_samples(in);
            v2q15_t lut_idx = fx_add_v2q15(fx_asr_v2q15_n(x, shift_in), offset);
            lut_idx = fx_max_v2q15(lut_idx, lower);
            lut_idx = fx_min_v2q15(lut_idx, upper);
            // no interpolation
            mli_prv_store_2_samples(out, fx_create_v2q15(lut_data[lut_idx[0]], lut_data[lut_idx[1]]));
            in += 2;
            out += 2;
        }
    }
}

void mli_prv_activation_lut_fx8(
        const MLI_PTR(int8_t) in,
        MLI_OUT_PTR(int8_t) out,
        const mli_lut *lut,
        int in_frac_bits,
        int length) {
    MLI_ASSERT(in_frac_bits >= -1);  // -1 may be required by softmax
    MLI_ASSERT(length >= 0);
    MLI_ASSERT(lut->frac_bits >= 0);
    MLI_ASSERT(lut->length >= 0);

    int shift_in = in_frac_bits - lut->frac_bits;
    int16_t *lut_data = (int16_t *)lut->data;
    // if shift amount is too high, preshift argument itself and
    // limit shift amount to prevent overflows
    int preshift_in = MAX(shift_in - (int)MAX_FRAC_BITS_FX16, 0);
    shift_in = MIN(shift_in, (int)MAX_FRAC_BITS_FX16);

    v2q15_t offset = fx_replic_v2q15(lut->offset);
    v2q15_t lower = fx_replic_v2q15(0);

    if (shift_in > 0) {
        // input data is more precise than LUT
        v2q15_t mask = fx_replic_v2q15((1 << shift_in) - 1);
        v2q15_t upper = fx_replic_v2q15(lut->length - 2);

        if (length & 1) {
            v2q15_t x = fx_asr_v2q15_n(mli_prv_load_1_sample(in), preshift_in);
            v2q15_t lut_idx = fx_add_v2q15(fx_asr_v2q15_n(x, shift_in), offset);
            lut_idx = fx_max_v2q15(lut_idx, lower);
            lut_idx = fx_min_v2q15(lut_idx, upper);
            // perform linear interpolation
            v2q15_t frac = x & mask;
            v2q15_t res = fx_create_v2q15(lut_data[lut_idx[0]], lut_data[lut_idx[1]]);
            v2q15_t next = fx_create_v2q15(lut_data[lut_idx[0] + 1], lut_data[lut_idx[1] + 1]);
            v2q15_t diff = fx_sub_v2q15(res, next);
            mli_prv_store_1_sample(
                    out, mli_prv_v2q7_cast_rnd_v2q15(fx_sub_v2q15(
                                 res, fx_v2q15_cast_nf_asr_rnd_v2a40(fx_v2a40_mpy_v2q15(diff, frac), shift_in - 16))));
            in += 1;
            out += 1;
        }
        for (int idx = 0; idx < (length >> 1); idx++) {
            v2q15_t x = fx_asr_v2q15_n(mli_prv_load_2_samples(in), preshift_in);
            v2q15_t lut_idx = fx_add_v2q15(fx_asr_v2q15_n(x, shift_in), offset);
            lut_idx = fx_max_v2q15(lut_idx, lower);
            lut_idx = fx_min_v2q15(lut_idx, upper);
            // perform linear interpolation
            v2q15_t frac = x & mask;
            v2q15_t res = fx_create_v2q15(lut_data[lut_idx[0]], lut_data[lut_idx[1]]);
            v2q15_t next = fx_create_v2q15(lut_data[lut_idx[0] + 1], lut_data[lut_idx[1] + 1]);
            v2q15_t diff = fx_sub_v2q15(res, next);
            mli_prv_store_2_samples(
                    out, mli_prv_v2q7_cast_rnd_v2q15(fx_sub_v2q15(
                                 res, fx_v2q15_cast_nf_asr_rnd_v2a40(fx_v2a40_mpy_v2q15(diff, frac), shift_in - 16))));
            in += 2;
            out += 2;
        }
    } else {
        // input data isn't more precise than LUT
        v2q15_t upper = fx_replic_v2q15(lut->length - 1);

        if (length & 1) {
            v2q15_t x = mli_prv_load_1_sample(in);
            v2q15_t lut_idx = fx_add_v2q15(fx_asr_v2q15_n(x, shift_in), offset);
            lut_idx = fx_max_v2q15(lut_idx, lower);
            lut_idx = fx_min_v2q15(lut_idx, upper);
            // no interpolation
            mli_prv_store_1_sample(
                    out, mli_prv_v2q7_cast_rnd_v2q15(fx_create_v2q15(lut_data[lut_idx[0]], lut_data[lut_idx[1]])));
            in += 1;
            out += 1;
        }
        for (int idx = 0; idx < (length >> 1); idx++) {
            v2q15_t x = mli_prv_load_2_samples(in);
            v2q15_t lut_idx = fx_add_v2q15(fx_asr_v2q15_n(x, shift_in), offset);
            lut_idx = fx_max_v2q15(lut_idx, lower);
            lut_idx = fx_min_v2q15(lut_idx, upper);
            // no interpolation
            mli_prv_store_2_samples(
                    out, mli_prv_v2q7_cast_rnd_v2q15(fx_create_v2q15(lut_data[lut_idx[0]], lut_data[lut_idx[1]])));
            in += 2;
            out += 2;
        }
    }
}

#pragma code()

#ifdef __cplusplus
}
#endif
