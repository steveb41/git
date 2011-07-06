#!/bin/sh

test_description='Test cherry-pick continuation features

  + picked: rewrites foo to c
  + unrelatedpick: rewrites unrelated to reallyunrelated
  + base: rewrites foo to b
  + initial: writes foo as a, unrelated as unrelated

'

. ./test-lib.sh

pristine_detach () {
	git checkout -f "$1^0" &&
	git read-tree -u --reset HEAD &&
	git clean -d -f -f -q -x
}

test_expect_success setup '
	echo unrelated >unrelated &&
	git add unrelated &&
	test_commit initial foo a &&
	test_commit base foo b &&
	test_commit unrelatedpick unrelated reallyunrelated &&
	test_commit picked foo c &&
	git config advice.detachedhead false

'

test_expect_success 'cherry-pick cleans up sequencer directory upon success' '
	pristine_detach initial &&
	git cherry-pick initial..picked &&
	test_path_is_missing .git/sequencer
'

test_done
