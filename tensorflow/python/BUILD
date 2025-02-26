# Description:
# Python support for TensorFlow.

package(default_visibility = ["//tensorflow:internal"])

licenses(["notice"])  # Apache 2.0

exports_files(["LICENSE"])

load("//tensorflow:tensorflow.bzl", "tf_cuda_library")
load("//tensorflow:tensorflow.bzl", "tf_gen_op_wrapper_py")
load("//tensorflow:tensorflow.bzl", "py_tests")
load("//tensorflow:tensorflow.bzl", "cuda_py_tests")
load("//tensorflow:tensorflow.bzl", "tf_py_wrap_cc")
load("//tensorflow/core:platform/default/build_config.bzl", "tf_proto_library")
load("//tensorflow/core:platform/default/build_config.bzl", "tf_proto_library_py")

py_library(
    name = "python",
    srcs = [
        "__init__.py",
        "unsupported.py",
    ],
    srcs_version = "PY2AND3",
    visibility = ["//tensorflow:__pkg__"],
    deps = [
        ":client",
        ":client_testlib",
        ":framework",
        ":framework_test_lib",
        ":kernel_tests/gradient_checker",
        ":platform",
        ":platform_test",
        ":summary",
        ":training",
        "//tensorflow/contrib:contrib_py",
    ],
)

py_library(
    name = "platform",
    srcs = glob(["platform/**/*.py"]),
    srcs_version = "PY2AND3",
)

py_library(
    name = "platform_test",
    srcs = [
        "platform/default/_googletest.py",
        "platform/googletest.py",
    ],
    srcs_version = "PY2AND3",
    deps = [":platform"],
)

py_tests(
    name = "default_platform_tests",
    srcs = glob(["platform/default/*_test.py"]),
    additional_deps = [
        ":platform",
        ":platform_test",
    ],
    prefix = "default_platform",
)

py_tests(
    name = "google_platform_tests",
    srcs = glob(["platform/google/*_test.py"]),
    additional_deps = [
        ":platform",
        ":platform_test",
    ],
    prefix = "google_platform",
)

cc_library(
    name = "py_func_lib",
    srcs = ["lib/core/py_func.cc"],
    hdrs = [
        "lib/core/py_func.h",
    ],
    deps = [
        "//tensorflow/core:framework",
        "//tensorflow/core:lib",
        "//third_party/py/numpy:headers",
        "//util/python:python_headers",
    ],
)

cc_library(
    name = "py_record_reader_lib",
    srcs = [
        "lib/io/py_record_reader.cc",
    ],
    hdrs = [
        "lib/io/py_record_reader.h",
    ],
    deps = [
        "//tensorflow/core:lib",
    ],
)

cc_library(
    name = "py_record_writer_lib",
    srcs = [
        "lib/io/py_record_writer.cc",
    ],
    hdrs = [
        "lib/io/py_record_writer.h",
    ],
    deps = [
        "//tensorflow/core:lib",
    ],
)

py_test(
    name = "pywrap_status_test",
    size = "small",
    srcs = ["lib/core/pywrap_status_test.py"],
    srcs_version = "PY2AND3",
    deps = [
        ":framework_test_lib",
        ":platform_test",
        "//tensorflow/core:protos_all_py",
    ],
)

cc_library(
    name = "python_op_gen",
    srcs = [
        "framework/python_op_gen.cc",
        "framework/python_op_gen.h",
    ],
    visibility = ["//visibility:public"],
    deps = [
        "//tensorflow/core:framework",
        "//tensorflow/core:protos_cc",
    ],
    alwayslink = 1,
)

cc_library(
    name = "python_op_gen_main",
    srcs = [
        "framework/python_op_gen_main.cc",
    ],
    visibility = ["//visibility:public"],
    deps = [
        ":python_op_gen",
    ],
)

# What is needed for tf_gen_op_wrapper_py.
py_library(
    name = "framework_for_generated_wrappers",
    srcs = [
        "framework/device.py",
        "framework/dtypes.py",
        "framework/function.py",
        "framework/op_def_registry.py",
        "framework/ops.py",
        "framework/registry.py",
        "framework/tensor_shape.py",
        "framework/versions.py",
        # TODO(josh11b): Move this to the framework directory
        "ops/op_def_library.py",
        "ops/constant_op.py",
    ],
    srcs_version = "PY2AND3",
    visibility = ["//visibility:public"],
    deps = [
        ":platform",
        ":util",
        "//tensorflow/core:protos_all_py",
    ],
)

py_library(
    name = "framework",
    srcs = [
        # TODO(mrry): Move this to the framework directory.
        "client/graph_util.py",
        "framework/errors.py",
        "framework/framework_lib.py",
        "framework/importer.py",
        "framework/random_seed.py",
        "framework/tensor_util.py",
        "framework/load_library.py",
        # TODO(josh11b): Move this to the framework directory
        "ops/common_shapes.py",
    ],
    srcs_version = "PY2AND3",
    deps = [":framework_for_generated_wrappers"],
)

# subinclude("//third_party/py/cython:build_defs")

py_library(
    name = "extra_py_tests_deps",
    srcs_version = "PY2AND3",
    deps = ["//tensorflow:tensorflow_py"],
)

py_library(
    name = "framework_test_lib",
    srcs = [
        "framework/test_util.py",
    ],
    srcs_version = "PY2AND3",
    deps = [
        ":framework",
        ":platform_test",
        ":pywrap_tensorflow",
        ":session",
        ":util",
    ],
)

py_library(
    name = "client_testlib",
    srcs = [
        "platform/test.py",
    ],
    srcs_version = "PY2AND3",
    deps = [
        ":framework_test_lib",
        ":platform_test",
    ],
)

py_test(
    name = "framework_errors_test",
    srcs = ["framework/errors_test.py"],
    main = "framework/errors_test.py",
    srcs_version = "PY2AND3",
    deps = [
        ":framework_test_lib",
        ":platform_test",
        "//tensorflow:tensorflow_py",
        "//tensorflow/core:protos_all_py",
    ],
)

py_test(
    name = "contrib_test",
    srcs = ["framework/contrib_test.py"],
    main = "framework/contrib_test.py",
    srcs_version = "PY2AND3",
    deps = [
        ":framework_test_lib",
        "//tensorflow:tensorflow_py",
    ],
)

tf_gen_op_wrapper_py(
    name = "functional_ops",
    out = "ops/gen_functional_ops.py",
    hidden = [
        "SymbolicGradient",
    ],
)

py_library(
    name = "functional_ops_lib",
    srcs = ["ops/functional_ops.py"],
    srcs_version = "PY2AND3",
    deps = [
        ":functional_ops",
    ],
)

py_test(
    name = "framework_function_test",
    srcs = ["framework/function_test.py"],
    main = "framework/function_test.py",
    srcs_version = "PY2AND3",
    deps = [
        ":framework_test_lib",
        ":functional_ops_lib",
        ":platform_test",
        "//tensorflow:tensorflow_py",
        "//tensorflow/core:protos_all_py",
    ],
)

py_test(
    name = "framework_versions_test",
    srcs = ["framework/versions_test.py"],
    main = "framework/versions_test.py",
    srcs_version = "PY2AND3",
    deps = [
        ":framework_test_lib",
        ":platform_test",
        "//tensorflow:tensorflow_py",
    ],
)

py_test(
    name = "framework_importer_test",
    srcs = ["framework/importer_test.py"],
    main = "framework/importer_test.py",
    srcs_version = "PY2AND3",
    deps = [
        ":framework_test_lib",
        ":ops",
        ":platform_test",
        "//tensorflow:tensorflow_py",
    ],
)

tf_gen_op_wrapper_py(
    name = "test_ops",
    out = "framework/test_ops.py",
    deps = [":test_ops_kernels"],
)

cc_library(
    name = "test_ops_kernels",
    srcs = ["framework/test_ops.cc"],
    linkstatic = 1,
    deps = ["//tensorflow/core:framework"],
    alwayslink = 1,
)

py_test(
    name = "framework_ops_test",
    srcs = ["framework/ops_test.py"],
    main = "framework/ops_test.py",
    srcs_version = "PY2AND3",
    deps = [
        ":framework_test_lib",
        ":ops",
        ":platform_test",
        ":session",
        ":test_ops",
    ],
)

py_test(
    name = "framework_tensor_shape_test",
    srcs = ["framework/tensor_shape_test.py"],
    main = "framework/tensor_shape_test.py",
    srcs_version = "PY2AND3",
    deps = [
        ":framework_test_lib",
        ":platform_test",
        "//tensorflow/core:protos_all_py",
    ],
)

py_test(
    name = "framework_tensor_shape_div_test",
    srcs = ["framework/tensor_shape_div_test.py"],
    main = "framework/tensor_shape_div_test.py",
    srcs_version = "PY2AND3",
    deps = [
        ":framework_test_lib",
        ":platform_test",
        "//tensorflow/core:protos_all_py",
    ],
)

py_test(
    name = "framework_tensor_util_test",
    srcs = ["framework/tensor_util_test.py"],
    main = "framework/tensor_util_test.py",
    srcs_version = "PY2AND3",
    deps = [
        ":framework_test_lib",
        ":ops",
        ":platform_test",
        "//tensorflow:tensorflow_py",
    ],
)

py_test(
    name = "framework_test_util_test",
    srcs = ["framework/test_util_test.py"],
    main = "framework/test_util_test.py",
    srcs_version = "PY2AND3",
    deps = [
        ":framework_test_lib",
        ":ops",
        ":platform_test",
        "//tensorflow:tensorflow_py",
    ],
)

py_test(
    name = "framework_dtypes_test",
    srcs = ["framework/dtypes_test.py"],
    main = "framework/dtypes_test.py",
    srcs_version = "PY2AND3",
    deps = [
        ":framework_test_lib",
        ":platform_test",
        "//tensorflow:tensorflow_py",
        "//tensorflow/core:protos_all_py",
    ],
)

py_test(
    name = "op_def_library_test",
    srcs = ["ops/op_def_library_test.py"],
    main = "ops/op_def_library_test.py",
    srcs_version = "PY2AND3",
    deps = [
        ":framework_test_lib",
        ":ops",
    ],
)

tf_gen_op_wrapper_py(
    name = "array_ops",
    hidden = [
        "BroadcastGradientArgs",
        "ConcatOffset",
        "Concat",
        "Const",
        "EditDistance",
        "Pack",
        "Placeholder",
        "RefIdentity",
        "Split",
        "Slice",
        "TileGrad",  # Exported through array_grad instead of array_ops.
        "ZerosLike",  # TODO(josh11b): Use this instead of the Python version.
        "Unpack",
    ],
    require_shape_functions = True,
)

tf_gen_op_wrapper_py(
    name = "attention_ops",
    require_shape_functions = True,
)

tf_gen_op_wrapper_py(
    name = "candidate_sampling_ops",
    hidden = [
        "AllCandidateSampler",
        "ComputeAccidentalHits",
        "FixedUnigramCandidateSampler",
        "LearnedUnigramCandidateSampler",
        "LogUniformCandidateSampler",
        "ThreadUnsafeUnigramCandidateSampler",
        "UniformCandidateSampler",
    ],
    require_shape_functions = True,
)

tf_gen_op_wrapper_py(
    name = "control_flow_ops",
    hidden = [
        "Switch",
        "Merge",
        "RefMerge",
        "Exit",
        "RefExit",
    ],
    require_shape_functions = True,
    deps = [
        "//tensorflow/core:control_flow_ops_op_lib",
        "//tensorflow/core:no_op_op_lib",
    ],
)

tf_gen_op_wrapper_py(
    name = "data_flow_ops",
    hidden = [
        "FIFOQueue",
        "HashTable",
        "InitializeTable",
        "LookupTableFind",
        "LookupTableSize",
        "Mutex",
        "MutexAcquire",
        "MutexRelease",
        "PaddingFIFOQueue",
        "QueueClose",
        "QueueDequeue",
        "QueueDequeueMany",
        "QueueEnqueue",
        "QueueEnqueueMany",
        "QueueSize",
        "RandomShuffleQueue",
        "Stack",
        "StackPop",
        "StackPush",
        "StackClose",
        "TensorArray",
        "TensorArrayClose",
        "TensorArrayGrad",
        "TensorArrayRead",
        "TensorArrayPack",
        "TensorArrayUnpack",
        "TensorArrayWrite",
    ],
    require_shape_functions = True,
)

tf_gen_op_wrapper_py(
    name = "image_ops",
    hidden = [
        "ResizeBilinearGrad",
        "ResizeNearestNeighborGrad",
        "AdjustContrastv2",
        "ScaleImageGrad",
    ],
    require_shape_functions = True,
)

tf_gen_op_wrapper_py(
    name = "io_ops",
    hidden = [
        "FixedLengthRecordReader",
        "IdentityReader",
        "ReaderClose",
        "ReaderEnqueueWork",
        "ReaderNumRecordsProduced",
        "ReaderNumWorkUnitsCompleted",
        "ReaderRead",
        "ReaderReset",
        "ReaderRestoreState",
        "ReaderSerializeState",
        "ReaderWorkQueueLength",
        "Restore",
        "RestoreSlice",
        "Save",
        "SaveSlices",
        "ShardedFilename",
        "ShardedFilespec",
        "TextLineReader",
        "TFRecordReader",
        "WholeFileReader",
    ],
    require_shape_functions = True,
)

tf_gen_op_wrapper_py(
    name = "linalg_ops",
    require_shape_functions = True,
)

tf_gen_op_wrapper_py(
    name = "logging_ops",
    hidden = [
        "Assert",
        "Print",
    ],
    require_shape_functions = True,
)

tf_gen_op_wrapper_py(
    name = "math_ops",
    hidden = [
        "Abs",
        "All",
        "Any",
        "BatchMatMul",
        "Complex",
        "Max",
        "Mean",
        "Min",
        "Pow",
        "Prod",
        "Range",
        "SparseMatMul",
        "Sum",
        "MatMul",
        "Sigmoid",
        "Tanh",
        "Lgamma",
        "Erf",
        "Erfc",
    ],
    require_shape_functions = True,
)

tf_gen_op_wrapper_py(
    name = "nn_ops",
    hidden = [
        "AvgPoolGrad",  # "*Grad" accessible through nn_grad instead of nn_ops.
        "BatchNormWithGlobalNormalizationGrad",
        "SoftmaxCrossEntropyWithLogits",
        "LRNGrad",
        "MaxPoolGrad",
        "MaxPoolGradWithArgmax",
        "ReluGrad",
        "Relu6Grad",
        "EluGrad",
        "SoftplusGrad",
        "SoftsignGrad",
        "TopK",
        "TopKV2",
        "BiasAdd",
        "Relu6",
        "AvgPool",
        "MaxPool",
    ],
    require_shape_functions = True,
)

tf_gen_op_wrapper_py(
    name = "parsing_ops",
    hidden = [
        "ParseExample",
        "ParseSingleSequenceExample",
    ],
    require_shape_functions = True,
)

tf_gen_op_wrapper_py(
    name = "random_ops",
    hidden = [
        "RandomUniform",
        "RandomUniformInt",
        "RandomShuffle",
        "RandomStandardNormal",
        "TruncatedNormal",
    ],
    require_shape_functions = True,
)

tf_gen_op_wrapper_py(
    name = "script_ops",
    hidden = [
        "PyFunc",
    ],
    require_shape_functions = True,
)

tf_gen_op_wrapper_py(
    name = "state_ops",
    hidden = [
        "Variable",
        "TemporaryVariable",
        "DestroyTemporaryVariable",
    ],
    require_shape_functions = True,
)

tf_gen_op_wrapper_py(
    name = "sparse_ops",
    hidden = [
        "DeserializeManySparse",
        "SerializeManySparse",
        "SerializeSparse",
        "SparseConcat",
        "SparseSplit",
        "SparseSelectLastK",
        "SparseReorder",
        "SparseToDense",
    ],
    require_shape_functions = True,
)

tf_gen_op_wrapper_py(
    name = "string_ops",
    require_shape_functions = True,
)

tf_gen_op_wrapper_py(
    name = "summary_ops",
    hidden = [
        "HistogramAccumulatorSummary",
        "HistogramSummary",
        "ImageSummary",
        "MergeSummary",
        "ScalarSummary",
    ],
    require_shape_functions = True,
)

tf_gen_op_wrapper_py(
    name = "user_ops",
    hidden = [
        "Fact",
    ],
    require_shape_functions = False,
)

tf_gen_op_wrapper_py(
    name = "training_ops",
    out = "training/gen_training_ops.py",
    require_shape_functions = True,
)

py_library(
    name = "ops",
    srcs = [
        "ops/array_grad.py",
        "ops/array_ops.py",
        "ops/attention_ops.py",
        "ops/candidate_sampling_ops.py",
        "ops/clip_ops.py",
        "ops/control_flow_grad.py",
        "ops/control_flow_ops.py",
        "ops/data_flow_grad.py",
        "ops/data_flow_ops.py",
        "ops/embedding_ops.py",
        "ops/gen_array_ops.py",
        "ops/gen_attention_ops.py",
        "ops/gen_control_flow_ops.py",
        "ops/gen_data_flow_ops.py",
        "ops/gen_image_ops.py",
        "ops/gen_io_ops.py",
        "ops/gen_linalg_ops.py",
        "ops/gen_logging_ops.py",
        "ops/gen_math_ops.py",
        "ops/gen_nn_ops.py",
        "ops/gen_random_ops.py",
        "ops/gen_state_ops.py",
        "ops/gen_string_ops.py",
        "ops/gen_summary_ops.py",
        "ops/gradients.py",
        "ops/histogram_ops.py",
        "ops/image_grad.py",
        "ops/image_ops.py",
        "ops/init_ops.py",
        "ops/io_ops.py",
        "ops/learn.py",
        "ops/linalg_grad.py",
        "ops/linalg_ops.py",
        "ops/logging_ops.py",
        "ops/math_grad.py",
        "ops/math_ops.py",
        "ops/nn.py",
        "ops/nn_grad.py",
        "ops/nn_ops.py",
        "ops/numerics.py",
        "ops/parsing_ops.py",
        "ops/random_ops.py",
        "ops/rnn.py",
        "ops/rnn_cell.py",
        "ops/script_ops.py",
        "ops/seq2seq.py",
        "ops/sparse_grad.py",
        "ops/sparse_ops.py",
        "ops/standard_ops.py",
        "ops/state_grad.py",
        "ops/state_ops.py",
        "ops/string_ops.py",
        "ops/summary_ops.py",
        "ops/template.py",
        "ops/variable_scope.py",
        "ops/variables.py",
        "user_ops/user_ops.py",
    ],
    srcs_version = "PY2AND3",
    deps = [
        ":array_ops",
        ":candidate_sampling_ops",
        ":control_flow_ops",
        ":data_flow_ops",
        ":framework",
        ":functional_ops_lib",
        ":io_ops",
        ":linalg_ops",
        ":logging_ops",
        ":math_ops",
        ":nn_ops",
        ":parsing_ops",
        ":random_ops",
        ":script_ops",
        ":sparse_ops",
        ":string_ops",
        ":summary_ops",
        ":user_ops",
    ],
)

py_library(
    name = "training",
    srcs = glob(
        ["training/**/*.py"],
        exclude = ["**/*test*"],
    ),
    srcs_version = "PY2AND3",
    deps = [
        ":client",
        ":framework",
        ":lib",
        ":ops",
        ":protos_all_py",
        ":pywrap_tensorflow",
        ":training_ops",
    ],
)

py_library(
    name = "client",
    srcs = glob(
        ["client/**/*.py"],
        exclude = ["**/*test*"],
    ),
    srcs_version = "PY2AND3",
    deps = [
        ":framework",
        ":ops",
        ":session",
        ":training_ops",
    ],
)

py_library(
    name = "util",
    srcs = glob(["util/**/*.py"]),
    srcs_version = "PY2AND3",
    deps = ["//google/protobuf:protobuf_python"],
)

tf_proto_library_py(
    name = "protos_all",
    srcs = glob(
        ["**/*.proto"],
        exclude = [
            "util/protobuf/compare_test.proto",
            "training/saver.proto",
        ],
    ),
    deps = [":public_protos_py"],
)

tf_proto_library(
    name = "public_protos",
    srcs = ["training/saver.proto"],
    visibility = ["//visibility:public"],
)

tf_proto_library_py(
    name = "compare_test_proto",
    testonly = 1,
    srcs = ["util/protobuf/compare_test.proto"],
)

py_test(
    name = "protobuf_compare_test",
    srcs = ["util/protobuf/compare_test.py"],
    main = "util/protobuf/compare_test.py",
    srcs_version = "PY2AND3",
    deps = [
        ":compare_test_proto_py",
        ":platform_test",
        ":util",
    ],
)

py_test(
    name = "events_writer_test",
    size = "small",
    srcs = [
        "client/events_writer_test.py",
    ],
    srcs_version = "PY2AND3",
    deps = [
        ":framework_test_lib",
        ":lib",
        ":platform_test",
    ],
)

tf_cuda_library(
    name = "tf_session_helper",
    srcs = ["client/tf_session_helper.cc"],
    hdrs = ["client/tf_session_helper.h"],
    deps = [
        ":construction_fails_op",
        ":test_ops_kernels",
        "//tensorflow/core",
        "//tensorflow/core:direct_session",
        "//tensorflow/core:kernels",
        "//tensorflow/core:lib",
        "//tensorflow/core:protos_cc",
        "//third_party/py/numpy:headers",
        "//util/python:python_headers",
    ],
)

tf_py_wrap_cc(
    name = "pywrap_tensorflow",
    srcs = ["tensorflow.i"],
    swig_includes = [
        "client/events_writer.i",
        "client/tf_session.i",
        "framework/python_op_gen.i",
        "lib/core/py_func.i",
        "lib/core/status.i",
        "lib/core/status_helper.i",
        "lib/core/strings.i",
        "lib/io/py_record_reader.i",
        "lib/io/py_record_writer.i",
        "platform/base.i",
        "platform/numpy.i",
        "util/port.i",
    ],
    deps = [
        ":py_func_lib",
        ":py_record_reader_lib",
        ":py_record_writer_lib",
        ":python_op_gen",
        ":tf_session_helper",
        "//util/python:python_headers",
    ],
)

py_library(
    name = "lib",
    srcs = glob(["lib/**/*.py"]),
    srcs_version = "PY2AND3",
    deps = [
        ":pywrap_tensorflow",
    ],
)

py_library(
    name = "session",
    srcs = ["client/session.py"],
    srcs_version = "PY2AND3",
    deps = [
        ":framework",
        ":ops",
        ":pywrap_tensorflow",
    ],
)

# Just used by tests.
tf_cuda_library(
    name = "construction_fails_op",
    testonly = 1,
    srcs = ["client/test_construction_fails_op.cc"],
    deps = [
        "//tensorflow/core",
        "//tensorflow/core:lib",
        "//tensorflow/core:protos_cc",
    ],
    alwayslink = 1,
)

py_test(
    name = "session_test",
    srcs = ["client/session_test.py"],
    srcs_version = "PY2AND3",
    deps = [
        ":framework",
        ":framework_test_lib",
        ":session",
    ],
)

py_test(
    name = "graph_util_test",
    srcs = ["client/graph_util_test.py"],
    srcs_version = "PY2AND3",
    deps = [
        ":framework",
        ":framework_test_lib",
        "//tensorflow:tensorflow_py",
    ],
)

py_library(
    name = "kernel_tests/gradient_checker",
    srcs = ["kernel_tests/gradient_checker.py"],
    srcs_version = "PY2AND3",
)

cpu_only_kernel_test_list = glob([
    "kernel_tests/attention_ops_test.py",
    "kernel_tests/barrier_ops_test.py",
    "kernel_tests/bcast_ops_test.py",
    "kernel_tests/candidate_sampler_ops_test.py",
    "kernel_tests/cholesky_op_test.py",
    "kernel_tests/clip_ops_test.py",
    "kernel_tests/decode_csv_op_test.py",
    "kernel_tests/decode_raw_op_test.py",
    "kernel_tests/determinant_op_test.py",
    "kernel_tests/diag_op_test.py",
    "kernel_tests/edit_distance_op_test.py",
    "kernel_tests/fifo_queue_test.py",
    "kernel_tests/identity_op_py_test.py",
    "kernel_tests/in_topk_op_test.py",
    "kernel_tests/io_ops_test.py",
    "kernel_tests/listdiff_op_test.py",
    "kernel_tests/logging_ops_test.py",
    "kernel_tests/lookup_table_op_test.py",
    "kernel_tests/lrn_op_py_test.py",
    "kernel_tests/matrix_inverse_op_test.py",
    "kernel_tests/matrix_solve_op_test.py",
    "kernel_tests/matrix_triangular_solve_op_test.py",
    "kernel_tests/matrix_solve_ls_op_test.py",
    "kernel_tests/mutex_ops_test.py",
    "kernel_tests/parsing_ops_test.py",
    "kernel_tests/queue_ops_test.py",
    "kernel_tests/random_shuffle_queue_test.py",
    "kernel_tests/save_restore_ops_test.py",
    "kernel_tests/segment_reduction_ops_test.py",
    "kernel_tests/self_adjoint_eig_op_test.py",
    "kernel_tests/sparse_concat_op_test.py",
    "kernel_tests/sparse_matmul_op_test.py",
    "kernel_tests/sparse_split_op_test.py",
    "kernel_tests/sparse_reorder_op_test.py",
    "kernel_tests/sparse_to_dense_op_test.py",
    "kernel_tests/sparsemask_op_test.py",
    "kernel_tests/summary_ops_test.py",
    "kernel_tests/template_test.py",
    "kernel_tests/topk_op_test.py",
    "kernel_tests/unique_op_test.py",
    "kernel_tests/variable_scope_test.py",
    "kernel_tests/variables_test.py",
    "kernel_tests/where_op_test.py",
])

py_tests(
    name = "cpu_only_kernel_tests",
    srcs = cpu_only_kernel_test_list,
)

py_tests(
    name = "reader_ops_test",
    srcs = ["kernel_tests/reader_ops_test.py"],
    additional_deps = [
        ":lib",
    ],
)

cuda_py_tests(
    name = "op_tests",
    srcs = glob(
        ["ops/*_test.py"],
        exclude = [
            "ops/image_ops_test.py",
            "ops/op_def_library_test.py",
        ],
    ),
)

cuda_py_tests(
    name = "kernel_tests",
    srcs = glob(
        ["kernel_tests/*_test.py"],
        exclude = [
            "**/reader_ops_test.py",
            # Sharded below
            "**/cwise_ops_test.py",
            "**/conv_ops_test.py",
            "**/linalg_grad_test.py",
            "**/pooling_ops_test.py",
        ] + cpu_only_kernel_test_list,
    ),
)

cuda_py_tests(
    name = "kernel_tests_with_sharding",
    srcs = [
        "kernel_tests/conv_ops_test.py",
        "kernel_tests/cwise_ops_test.py",
        "kernel_tests/linalg_grad_test.py",
        "kernel_tests/pooling_ops_test.py",
    ],
    shard_count = 2,
)

cuda_py_tests(
    name = "image_ops_test",
    srcs = [
        "ops/image_ops_test.py",
    ],
    data = [
        "//tensorflow/core:image_testdata",
    ],
    shard_count = 5,
)

cuda_py_tests(
    name = "training_tests",
    srcs = glob(
        ["training/*_test.py"],
        exclude = ["training/input_test.py"],
    ),
    additional_deps = [
        ":training",
    ],
)

py_tests(
    name = "training_tests",
    srcs = glob(
        ["training/input_test.py"],
    ),
    additional_deps = [
        ":training",
    ],
)

py_library(
    name = "summary",
    srcs = glob(
        ["summary/**/*.py"],
        exclude = ["**/*test*"],
    ),
    srcs_version = "PY2AND3",
    deps = [
        ":client",
        ":framework",
        ":protos_all_py",
        ":pywrap_tensorflow",
        ":util",
    ],
)

py_tests(
    name = "summary_tests",
    srcs = glob(["summary/**/*_test.py"]),
    additional_deps = [
        ":summary",
        ":training",
    ],
)

py_library(
    name = "docs",
    srcs = [
        "framework/docs.py",
    ],
    srcs_version = "PY2AND3",
    deps = [
        ":platform",
    ],
)

py_binary(
    name = "gen_docs_combined",
    srcs = [
        "framework/gen_docs_combined.py",
    ],
    main = "framework/gen_docs_combined.py",
    srcs_version = "PY2AND3",
    deps = [
        ":docs",
        ":platform",
        "//tensorflow:tensorflow_py",
    ],
)

filegroup(
    name = "all_files",
    srcs = glob(
        ["**/*"],
        exclude = [
            "**/METADATA",
            "**/OWNERS",
        ],
    ),
    visibility = ["//tensorflow:__subpackages__"],
)
