load("@bazel_tools//tools/build_defs/repo:http.bzl", "http_archive")

http_archive(
    name = "rules_foreign_cc",
    sha256 = "33a5690733c5cc2ede39cb62ebf89e751f2448e27f20c8b2fbbc7d136b166804",
    strip_prefix = "rules_foreign_cc-0.5.1",
    url = "https://github.com/bazelbuild/rules_foreign_cc/archive/0.5.1.tar.gz",
)

load("@rules_foreign_cc//foreign_cc:repositories.bzl", "rules_foreign_cc_dependencies")

# This sets up some common toolchains for building targets. For more details, please see
# https://bazelbuild.github.io/rules_foreign_cc/0.5.1/flatten.html#rules_foreign_cc_dependencies
rules_foreign_cc_dependencies()

http_archive(
    name = "pybind11",
    build_file_content = 
"""
cc_library(
    name = "pybind11",
    hdrs = glob(["include/**/*.h"]),
    includes = ["include"],
    visibility = ["//visibility:public"],
)
""",
    url = "https://github.com/pybind/pybind11/archive/refs/tags/v2.7.1.tar.gz",
    sha256 = "616d1c42e4cf14fa27b2a4ff759d7d7b33006fdc5ad8fd603bb2c22622f27020",
    strip_prefix = "pybind11-2.7.1"
)

new_local_repository(
    name = "python",
    build_file_content =
"""
cc_library(
    name = "lib",
    hdrs = glob(["include/python3.6/**/*.h"]),
    srcs = glob(["lib/x86_64-linux-gnu/libpython*3.6*so"]),
    includes = ["include/python3.6"],

    visibility = ["//visibility:public"],
)
""",
    path = "/usr",
) 

http_archive(
    name = "mppp",
    build_file_content =
"""
load("@rules_foreign_cc//foreign_cc:defs.bzl", "cmake")

filegroup(
    name = "srcs",
    srcs = glob(["**"]),
    visibility = ["//visibility:public"],
)

cmake(
    name = "lib",
    alwayslink = True,
    lib_source = ":srcs",
    out_include_dir = "include",
    out_shared_libs = ["libmp++.so.10"],
    visibility = ["//visibility:public"],
)
""",
    url = "https://github.com/bluescarni/mppp/archive/refs/tags/v0.24.tar.gz",
    sha256 = "c84cbe38545b7f3f20688791e0a7ce4020830ed84ab6a109ab13a208745be9dc",
    strip_prefix = "mppp-0.24",
)

http_archive(
    name = "rules_python",
    url = "https://github.com/bazelbuild/rules_python/releases/download/0.3.0/rules_python-0.3.0.tar.gz",
    sha256 = "934c9ceb552e84577b0faf1e5a2f0450314985b4d8712b2b70717dc679fdc01b",
)

load("@rules_python//python:pip.bzl", "pip_install")

pip_install(
   name = "py_deps",
   requirements = "//:requirements.txt",
)
