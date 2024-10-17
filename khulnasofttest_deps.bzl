"""Load dependencies needed to use the khulnasofttest library as a 3rd-party consumer."""

load("@bazel_tools//tools/build_defs/repo:http.bzl", "http_archive")

def khulnasofttest_deps():
    """Loads common dependencies needed to use the khulnasofttest library."""

    if not native.existing_rule("com_khulnasoftsource_code_re2"):
        http_archive(
            name = "com_khulnasoftsource_code_re2",  # 2023-03-17T11:36:51Z
            sha256 = "cb8b5312a65f2598954545a76e8bce913f35fbb3a21a5c88797a4448e9f9b9d9",
            strip_prefix = "re2-578843a516fd1da7084ae46209a75f3613b6065e",
            urls = ["https://github.com/khulnasoft-lab/re2/archive/578843a516fd1da7084ae46209a75f3613b6065e.zip"],
        )

    if not native.existing_rule("com_khulnasoft_absl"):
        http_archive(
            name = "com_khulnasoft_absl",  # 2023-09-13T14:58:42Z
            sha256 = "8a6b98d3510414ea4059e0a48c46e644be22b207a53ac8018aef7e3511f077cb",
            strip_prefix = "abseil-cpp-8c495b5bbd54adeb7d82205c26830f76e50773b3",
            urls = ["https://github.com/abseil/abseil-cpp/archive/8c495b5bbd54adeb7d82205c26830f76e50773b3.zip"],
        )
