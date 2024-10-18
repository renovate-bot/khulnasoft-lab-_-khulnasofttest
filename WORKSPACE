workspace(name = "com_khulnasoft_khulnasofttest")

load("//:khulnasofttest_deps.bzl", "khulnasofttest_deps")
khulnasofttest_deps()

load("@bazel_tools//tools/build_defs/repo:http.bzl", "http_archive")

http_archive(
  name = "rules_python",  # 2023-07-31T20:39:27Z
  sha256 = "8c9c9a4774f37bf240fecfe3c3deffb3db627d4e2067c68e52525b1ec8f9fdd6",
  strip_prefix = "rules_python-0c0492dfaea7eee9132c501439872e8d8e62bd4a",
  urls = ["https://github.com/bazelbuild/rules_python/archive/0c0492dfaea7eee9132c501439872e8d8e62bd4a.zip"],
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
