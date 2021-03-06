/*
* Copyright 2019-2020, Synopsys, Inc.
* All rights reserved.
*
* This source code is licensed under the BSD-3-Clause license found in
* the LICENSE file in the root directory of this source tree.
*
*/

#ifndef _MLI_PRIVATE_TYPES_API_H_
#define _MLI_PRIVATE_TYPES_API_H_

#include "mli_config.h"
#include "mli_types.h"

/**
 * Private tensor struct typically used for pooling/conv2d/depthwise_conv2d
 * inputs and outputs.
 */
template <typename T>
struct tensor_private_t {
    T __restrict ptr;
    int32_t width;
    int32_t height;
    int32_t ch;
    int32_t col_mem_stride;
    int32_t row_mem_stride;
    int32_t ch_mem_stride;
};

/**
 * Private weights tensor struct typically used for conv2d/depthwise_conv2d.
 */
template <typename T>
struct conv2d_weights_tensor_private_t {
    T __restrict ptr;
    int32_t kernel_width;
    int32_t kernel_height;
    int32_t in_ch;
    int32_t out_ch;
    int32_t col_mem_stride;
    int32_t row_mem_stride;
    int32_t in_ch_mem_stride;
    int32_t out_ch_mem_stride;
};

#ifdef __cplusplus
extern "C" {
#endif

typedef struct {
    uint32_t row_beg;
    uint32_t row_end;
    uint32_t clmn_beg;
    uint32_t clmn_end;
} rect_t;

/**
 * Lookup table config definition 
 */
typedef struct {
    const void* data;
    mli_element_type type;
    int length;
    int frac_bits;
    int offset;
} mli_lut;

// Value range for applying ReLU 
typedef struct {
    int16_t min;
    int16_t max;
} mli_minmax_t;

typedef int32_t   mli_acc32_t;
typedef accum40_t mli_acc40_t;

typedef signed char v2i8_t __attribute__((__vector_size__(2)));

typedef enum {
    FX_MATH = 0,
    S8ASYM_MATH
} mli_math_type;


/**
 * @brief Quantization specific parameter to perform correct calculations in s8asym quantization scheme.
 */
struct s8asym_quant_specific_params {
    int16_t in_offset;
    int16_t out_offset;
    int16_t weights_offset;

    const int32_t *weight_scales;
    int32_t in_to_out_scales_ratio;
    
    int32_t out_mul;
    int out_shift;
};

/**
 * @brief Quantization specific parameter to perform correct calculations in MLI_FX quantization scheme.
 */
struct fx_quant_specific_params {
    int bias_shift;
    int out_shift;
};

typedef union _conv_math_params {
    struct fx_quant_specific_params fx;

    struct s8asym_quant_specific_params i8asym;
} conv_math_params;

#ifdef __cplusplus
}
#endif
#endif                          //_MLI_HELPERS_API_H_
