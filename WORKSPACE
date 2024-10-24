workspace(name = "com_khulnasoft_khulnasofttest")

load("//:khulnasofttest_deps.bzl", "khulnasofttest_deps")
khulnasofttest_deps()

load("@bazel_tools//tools/build_defs/repo:http.bzl", "http_archive")

http_archive(
  name = "rules_python",  # 2023-07-31T20:39:27Z
  sha256 = "5654fe30c4b8fbd9fed2ab8ed771141e5da95781724fbedddb34bdcdafcd95a9",
  strip_prefix = "rules_python-f231358115c8d411a77b7e5b0a7a7dd6867679fd",
  urls = ["https://github.com/bazelbuild/rules_python/archive/f231358115c8d411a77b7e5b0a7a7dd6867679fd.zip"],
)

http_archive(
  name = "bazel_skylib",  # 2023-05-31T19:24:07Z
  sha256 = "08c0386f45821ce246bbbf77503c973246ed6ee5c3463e41efc197fa9bc3a7f4",
  strip_prefix = "bazel-skylib-288731ef9f7f688932bd50e704a91a45ec185f9b",
  urls = ["https://github.com/bazelbuild/bazel-skylib/archive/288731ef9f7f688932bd50e704a91a45ec185f9b.zip"],
)

http_archive(
  name = "platforms",  # 2023-07-28T19:44:27Z
  sha256 = "40eb313613ff00a5c03eed20aba58890046f4d38dec7344f00bb9a8867853526",
  strip_prefix = "platforms-4ad40ef271da8176d4fc0194d2089b8a76e19d7b",
  urls = ["https://github.com/bazelbuild/platforms/archive/4ad40ef271da8176d4fc0194d2089b8a76e19d7b.zip"],
)
