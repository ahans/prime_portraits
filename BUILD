load("@rules_python//python:defs.bzl", "py_binary")
load("@py_deps//:requirements.bzl", "requirement")

cc_binary(
    name = "hello",
    srcs = ["hello.cc"],
)

cc_binary(
    name = "prime_search.so",
    srcs = ["prime_search.cpp"],
    deps = [
        "@pybind11//:pybind11",
        "@python//:lib",
        "@mppp//:lib",
    ],
    copts = [
        "-std=c++17", "-O3", "-Wall", "-Wextra",
    ],
    linkopts = [
        "-lgmp",
    ],
    linkstatic = True,
    linkshared = True,
)

py_binary(
    name = "find_portrait",
    srcs = [
        "find_portrait.py",
    ],
    deps = [
        requirement("Pillow"),
        requirement("sklearn"),
    ],
    data = [
        "data/starrynight.jpg",
        ":prime_search.so",
    ],
)
