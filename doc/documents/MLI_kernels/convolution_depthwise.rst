.. _cnvl_depth:

Depth-wise Convolution
~~~~~~~~~~~~~~~~~~~~~~

.. image::  ../images/image107.jpg
   :align: center
   :alt: Depth-wise Convolution Operation

..
   
This kernel implements a 2D depth-wise convolution operation applying
each filter channel to each input channel separately. As a result,
output image depth is the same as input image depth.

MLI implementation of depth-wise convolution is compatible with caffe
implementation of convolution layer with group parameter equal to
number of input channels. In comparison with TensorFlow
implementation (``tf.nn.depthwise_conv2d`` in python API), this
implementation does not support channel multiplier feature. Therefore,
the last dimension of weights tensor must be equal to 1.

For the example for CHW data layout, if the input feature map is [Ci,
Hi, Wi] and the kernel is [Ci, Hk, Wk,1], the output feature map is
[Ci, Ho, Wo] matrix where the output dimensions Ho and Wo are
calculated dynamically according to convolution parameters (such as
padding and stride) in the same way as for general convolution 2D
kernel (see :ref:`cnvl_2d`).

ReLU activation function might be applied to result of convolution. The
following types of ReLU activations are supported (for more info see
:ref:`relu`):

-  RELU_NONE

-  RELU_GEN

-  RELU_1

-  RELU_6

.. caution::
   Ensure that input and output
   tensors do not point to     
   overlapped memory regions,  
   otherwise the behavior is   
   undefined.
   
.. _function-configuration-structure-1:

Function Configuration Structure
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

Depth-wise convolution kernel shares configuration structure with
general convolution 2D kernel. For more information see :ref:`fn_conf_struct`
section of Convolution 2D function.

.. _general-api-1:

Kernel Interface
^^^^^^^^^^^^^^^^

Interface of all specializations are the same and described in
following table.

Prototype
'''''''''
   
.. code:: c                             
                                           
 mli_status mli_krn_depthwise_conv2d_      
 <layout>_<data_type>[_specialization](    
    const mli_tensor *in,                  
    const mli_tensor *weights,             
    const mli_tensor *bias,                
    const mli_conv2d_cfg *cfg,          
    mli_tensor *out); 
..

Parameters
''''''''''

.. table:: Kernel Interface Parameters
   :widths: 20,130
   
   +-----------------------+-----------------------+
   | **Parameter**         | **Description**       |
   +-----------------------+-----------------------+
   | ``in``                | [IN] Pointer to input |
   |                       | feature map tensor    |
   +-----------------------+-----------------------+
   | ``weights``           | [IN] Pointer to       |
   |                       | convolution filters   |
   |                       | weights tensor        |
   +-----------------------+-----------------------+
   | ``bias``              | [IN] Pointer to       |
   |                       | convolution filters   |
   |                       | biases tensor         |
   +-----------------------+-----------------------+
   | ``cfg``               | [IN] Pointer to       |
   |                       | convolution           |
   |                       | parameters structure  |
   +-----------------------+-----------------------+
   | ``out``               | [OUT] Pointer to      |
   |                       | output feature map    |
   |                       | tensor. Result is     |
   |                       | stored here           |
   +-----------------------+-----------------------+

.. _function-specializations-1:

Function Specializations
^^^^^^^^^^^^^^^^^^^^^^^^

There are about 70 specializations for the primitive assuming various
combinations of inputs parameters. Depth-wise convolution primitive
follows the naming convention for specializations (see :ref:`spec_fns`). 
The header file ``mli_krn_depthwise_conv2d_spec_api.h`` contains declarations 
of all specializations for the primitive.

:ref:`Non_spec_func_Dw_cnvl` does not contain specialized functions. 
No functions for HWC data layout have been implemented at the moment. 
To use depth-wise convolution in this case, ensure that you change weights
and inputs layout beforehand by permute primitive (see :ref:`permute`).

.. _Non_spec_func_Dw_cnvl:
.. table:: Non-Specialized Functions
   :widths: 20,130
   
   +-----------------------------------------------+-----------------------------------+
   | **Function**                                  | **Description**                   |
   +===============================================+===================================+
   | ``mli_krn_depthwise_conv2d_chw_fx8``          | Switching function (see           |
   |                                               | :ref:`fns`); 8bit FX tensors;     |
   |                                               | Delegates calculations to         |
   |                                               | suitable specialization or        |
   |                                               | generic function.                 |
   +-----------------------------------------------+-----------------------------------+
   | ``mli_krn_depthwise_conv2d_chw_fx16``         | Switching function (see           |
   |                                               | :ref:`fns`); 16bit FX tensors;    |
   |                                               | Delegates calculations to         |
   |                                               | suitable specialization or        |
   |                                               | generic function.                 |
   +-----------------------------------------------+-----------------------------------+
   | ``mli_krn_depthwise_conv2d_chw_fx8w16d``      | General function; FX tensors      |
   |                                               | (8bit weights and biases, 16bit   |
   |                                               | input and output)                 |
   +-----------------------------------------------+-----------------------------------+
   | ``mli_krn_depthwise_conv2d_chw_fx8_generic``  | General function; 8bit FX         |
   |                                               | tensors                           |
   +-----------------------------------------------+-----------------------------------+
   | ``mli_krn_depthwise_conv2d_chw_fx16_generic`` | General function; 16bit FX        |
   |                                               | tensors                           |
   +-----------------------------------------------+-----------------------------------+
   | ``mli_krn_depthwise_conv2d_chw_fx16_[spec]``  | Specialization function*; 8bit FX |
   |                                               | tensors                           |
   +-----------------------------------------------+-----------------------------------+
   | ``mli_krn_depthwise_conv2d_chw_fx16_[spec]``  | Specialization function*; 16bit   |
   |                                               | FX tensors                        |
   +-----------------------------------------------+-----------------------------------+

.. attention:: 
   \*For specialization         
   functions, backward          
   compatibility between        
   different releases cannot be  
   guaranteed. The general       
   functions call the available  
   specializations when possible.

.. _conditions-for-applying-the-function-1:

Conditions for Applying the Function
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

Ensure that you satisfy the following conditions before applying the
function:

-  Input, weights and bias tensors must be valid (see :ref:`mli_tns_struct`).

-  Before processing, the output tensor must contain valid pointer to a
   buffer with sufficient capacity (enough for result storing). It
   also must contain valid element parameter (``el_params.fx.frac_bits``).

-  Before processing, the output tensor does not have to contain valid
   shape, rank and element type fields. These are filled by the function.

-  Input and weights tensors data layout must be the same. Amount of
   weights channels must be 1.

-  Amount of filters (first dimension of weights tensor) must be equal
   to number of input channels.

-  Bias must be one-dimensional tensor. Its length must be equal to
   amount of filters (first dimension of weights tensor)

-  padding_top and padding_bottom parameters must be in range of [0,
   weights_height).

-  ``padding_left`` and ``padding_right`` parameters must be in range of [0,
   weights_width).

-  ``stride_width`` and ``stride_height`` parameters must not be equal to 0.

-  ``weights_width`` and ``weights_height`` must be less or equal to appropriate
   dimensions of input tensor.