#!/usr/bin/env python
#
# Copyright 2008, Khulnasoft Inc.
# All rights reserved.
#
# Redistribution and use in source and binary forms, with or without
# modification, are permitted provided that the following conditions are
# met:
#
#     * Redistributions of source code must retain the above copyright
# notice, this list of conditions and the following disclaimer.
#     * Redistributions in binary form must reproduce the above
# copyright notice, this list of conditions and the following disclaimer
# in the documentation and/or other materials provided with the
# distribution.
#     * Neither the name of Khulnasoft Inc. nor the names of its
# contributors may be used to endorse or promote products derived from
# this software without specific prior written permission.
#
# THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
# "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
# LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
# A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
# OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
# SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
# LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
# DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
# THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
# (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
# OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

"""Verifies that Khulnasoft Test correctly determines whether to use colors."""

import os
from khulnasofttest.test import ktest_test_utils

IS_WINDOWS = os.name == 'nt'

COLOR_ENV_VAR = 'KTEST_COLOR'
COLOR_FLAG = 'ktest_color'
COMMAND = ktest_test_utils.GetTestExecutablePath('khulnasofttest-color-test_')


def SetEnvVar(env_var, value):
  """Sets the env variable to 'value'; unsets it when 'value' is None."""

  if value is not None:
    os.environ[env_var] = value
  elif env_var in os.environ:
    del os.environ[env_var]


def UsesColor(term, color_env_var, color_flag):
  """Runs khulnasofttest-color-test_ and returns its exit code."""

  SetEnvVar('TERM', term)
  SetEnvVar(COLOR_ENV_VAR, color_env_var)

  if color_flag is None:
    args = []
  else:
    args = ['--%s=%s' % (COLOR_FLAG, color_flag)]
  p = ktest_test_utils.Subprocess([COMMAND] + args)
  return not p.exited or p.exit_code


class GTestColorTest(ktest_test_utils.TestCase):

  def testNoEnvVarNoFlag(self):
    """Tests the case when there's neither KTEST_COLOR nor --ktest_color."""

    if not IS_WINDOWS:
      self.assertTrue(not UsesColor('dumb', None, None))
      self.assertTrue(not UsesColor('emacs', None, None))
      self.assertTrue(not UsesColor('xterm-mono', None, None))
      self.assertTrue(not UsesColor('unknown', None, None))
      self.assertTrue(not UsesColor(None, None, None))
    self.assertTrue(UsesColor('linux', None, None))
    self.assertTrue(UsesColor('cygwin', None, None))
    self.assertTrue(UsesColor('xterm', None, None))
    self.assertTrue(UsesColor('xterm-color', None, None))
    self.assertTrue(UsesColor('xterm-kitty', None, None))
    self.assertTrue(UsesColor('xterm-256color', None, None))

  def testFlagOnly(self):
    """Tests the case when there's --ktest_color but not KTEST_COLOR."""

    self.assertTrue(not UsesColor('dumb', None, 'no'))
    self.assertTrue(not UsesColor('xterm-color', None, 'no'))
    if not IS_WINDOWS:
      self.assertTrue(not UsesColor('emacs', None, 'auto'))
    self.assertTrue(UsesColor('xterm', None, 'auto'))
    self.assertTrue(UsesColor('dumb', None, 'yes'))
    self.assertTrue(UsesColor('xterm', None, 'yes'))

  def testEnvVarOnly(self):
    """Tests the case when there's KTEST_COLOR but not --ktest_color."""

    self.assertTrue(not UsesColor('dumb', 'no', None))
    self.assertTrue(not UsesColor('xterm-color', 'no', None))
    if not IS_WINDOWS:
      self.assertTrue(not UsesColor('dumb', 'auto', None))
    self.assertTrue(UsesColor('xterm-color', 'auto', None))
    self.assertTrue(UsesColor('dumb', 'yes', None))
    self.assertTrue(UsesColor('xterm-color', 'yes', None))

  def testEnvVarAndFlag(self):
    """Tests the case when there are both KTEST_COLOR and --ktest_color."""

    self.assertTrue(not UsesColor('xterm-color', 'no', 'no'))
    self.assertTrue(UsesColor('dumb', 'no', 'yes'))
    self.assertTrue(UsesColor('xterm-color', 'no', 'auto'))

  def testAliasesOfYesAndNo(self):
    """Tests using aliases in specifying --ktest_color."""

    self.assertTrue(UsesColor('dumb', None, 'true'))
    self.assertTrue(UsesColor('dumb', None, 'YES'))
    self.assertTrue(UsesColor('dumb', None, 'T'))
    self.assertTrue(UsesColor('dumb', None, '1'))

    self.assertTrue(not UsesColor('xterm', None, 'f'))
    self.assertTrue(not UsesColor('xterm', None, 'false'))
    self.assertTrue(not UsesColor('xterm', None, '0'))
    self.assertTrue(not UsesColor('xterm', None, 'unknown'))


if __name__ == '__main__':
  ktest_test_utils.Main()
