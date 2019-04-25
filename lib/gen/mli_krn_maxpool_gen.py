from func import Func
from codegen import Codegen
import sys

#------------------------------------------------------------
# maxpool functions chw
#------------------------------------------------------------

#------------------------------------------------------------
# fill the basic information
#------------------------------------------------------------
func_body_template_file = "mli_krn_maxpool_func_body.txt"
file_template = "filetemplate.txt"
file_header_template = "header_filetemplate.txt"
function_group = "MaxPooling"
capital_header_file_name = "_MLI_KRN_MAXPOOL_SPEC_API_H_"
output_header_file = "..\..\include\\api\mli_krn_maxpool_spec_api.h"
output_file_fx16 = "..\..\lib\src\kernels\pooling\mli_krn_maxpool_chw_fx16.cc"
output_file_fx8 = "..\..\lib\src\kernels\pooling\mli_krn_maxpool_chw_fx8.cc"

f_list = []
f_args = [("const mli_tensor *", "in"),
          ("const mli_pool_cfg *", "cfg"),
          ("mli_tensor *", "out")]
fbase = ("krn", "maxpool", "chw", "fx16", f_args)
include_list = ["mli_krn_maxpool_chw.h"]
define_list = []
# commandline arguments can be used to generate a specific output 'fx16' | 'fx8' | 'header']
# if no arguments are given, all files are generated.
no_args = len(sys.argv) == 1

#------------------------------------------------------------
# Create a list of specialization functions for fx16
#------------------------------------------------------------

corefunc = "maxpool_chw_nopad"
stride = 1
kernel_range = range(2,11)
channel_range = [0,1,3]
f_list.extend([Func(fbase, k, k, ch, stride, stride, corefunc, "nopad") for k in kernel_range for ch in channel_range])

#stride = 1, 1xk and kx1 versions
corefunc = "maxpool_chw_nopad"
stride = 1
kernel_range = range(2,4)
channel_range = [0,1]
f_list.extend([Func(fbase, 1, k, ch, stride, stride, corefunc, "nopad") for k in kernel_range for ch in channel_range])
f_list.extend([Func(fbase, k, 1, ch, stride, stride, corefunc, "nopad") for k in kernel_range for ch in channel_range])

corefunc = "maxpool_chw_krnpad"
stride = 1
kernel_range = range(2,11)
channel_range = [0,1,3]
f_list.extend([Func(fbase, k, k, ch, stride, stride, corefunc, "krnpad") for k in kernel_range for ch in channel_range])

#stride = 1, 1xk and kx1 versions
corefunc = "maxpool_chw_krnpad"
stride = 1
kernel_range = range(2,4)
channel_range = [0,1]
f_list.extend([Func(fbase, 1, k, ch, stride, stride, corefunc, "krnpad") for k in kernel_range for ch in channel_range])
f_list.extend([Func(fbase, k, 1, ch, stride, stride, corefunc, "krnpad") for k in kernel_range for ch in channel_range])

#fix single dimension, others flex
corefunc = "maxpool_chw_krnpad"
stride = 1
f_list.extend([Func(fbase, 1, 0, 0, stride, stride, corefunc, "")]) #k_width == 1
f_list.extend([Func(fbase, 0, 1, 0, stride, stride, corefunc, "")]) #k_heigth == 1
f_list.extend([Func(fbase, 0, 0, 1, stride, stride, corefunc, "")]) #channels == 1

corefunc = "maxpool_chw_krnpad"
stride = 0
kernel_range = [2,3]
channel_range = [0,1]
f_list.extend([Func(fbase, k, k, ch, stride, stride, corefunc, "") for k in kernel_range for ch in channel_range])

#at last add the generic function that can be used in the else branch in the wrapper.
corefunc = "maxpool_chw_krnpad"
default_func = Func(fbase, 0, 0, 0, 0, 0, corefunc, generic=True)
f_list.append(default_func)

#------------------------------------------------------------
# Generate the output file
#------------------------------------------------------------
c = Codegen()
c.set_wrapper_variables({'stride_w' : "cfg->stride_width", 'stride_h' : "cfg->stride_height"})
c.set_wrapper_variables({'kernel_w' : "cfg->kernel_width", 'kernel_h' : "cfg->kernel_height"})
c.set_wrapper_variables({'channels' : "in->shape[FMAP_C_DIM_CHW]"})
c.set_wrapper_variables({'padding_top' : "cfg->padding_top"})
c.set_wrapper_variables({'padding_bot' : "cfg->padding_bottom"})
c.set_wrapper_variables({'padding_left' : "cfg->padding_left"})
c.set_wrapper_variables({'padding_right' : "cfg->padding_right"})
c.set_wrapper_hierarchy(['stride_w', 'stride_h', 'kernel_w', 'kernel_h', 'channels', 'padding'])
c.set_wrapper_if_tree(True)

if "fx16" in sys.argv or no_args:
    f = open(output_file_fx16, "wb")
    f.write(c.print_file(f_list, default_func, func_body_template_file, file_template, include_list, define_list))
    f.close()

#------------------------------------------------------------
# Create a new list of specialization functions for fx8
#------------------------------------------------------------

fbase = ("krn", "maxpool", "chw", "fx8", f_args)

f_list_fx8 = [f.copy_and_replace_base(fbase) for f in f_list]
default_func = default_func.copy_and_replace_base(fbase)

#------------------------------------------------------------
# Generate the output file
#------------------------------------------------------------
if "fx8" in sys.argv or no_args:
    f = open(output_file_fx8, "wb")
    f.write(c.print_file(f_list_fx8, default_func, func_body_template_file, file_template, include_list, define_list))
    f.close()

#------------------------------------------------------------
# Generate the output header file
#------------------------------------------------------------
if "header" in sys.argv or no_args:
    fh = open(output_header_file, "wb")
    fh.write(c.print_proto_file([f_list,f_list_fx8], function_group, capital_header_file_name, file_header_template))
    fh.close()