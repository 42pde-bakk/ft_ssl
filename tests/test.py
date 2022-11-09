#!/usr/bin/env python3

import sys
import subprocess
from typing import Tuple
from colored import fg, attr
import json
import os


def run_process(arg: str) -> Tuple[int, str, str]:
	p = subprocess.Popen(f'{arg}', shell=True, stdout=subprocess.PIPE, stderr=subprocess.PIPE)
	p.wait()
	stdout = p.stdout.read().decode('ascii')
	stderr = p.stderr.read().decode('ascii')
	return p.returncode, stdout, stderr


def hard_code(testcase: dict) -> None:
	my_ret, my_stdout, my_stderr = run_process(f'{testcase["command"]}')

	assert(testcase['expected_statuscode'] == my_ret), f'Statuscode:\n\tgot:\t{my_ret}, expected: {testcase["expected_statuscode"]}'
	if 'expected_stdout' in testcase.keys():
		assert(testcase['expected_stdout'] == my_stdout), f'STDOUT:\n\tgot:\t\'{my_stdout.encode("unicode_escape")}\',\n\texpected:\'{testcase["expected_stdout"].encode("unicode_escape")}\''
	if 'expected_stderr' in testcase.keys():
		assert(testcase['expected_stderr'] == my_stderr), f'STDERR:n\n\ttgot:\t\'{my_stderr.encode("unicode_escape")}\',\n\texpected:\'{testcase["expected_stderr"].encode("unicode_escape")}\''


def run_testcases(test_cases: list, test_name: str) -> int:
	return_status = 0
	print(f'Testing {test_name}')
	for test_case in test_cases:
		test_outcome = f"{fg('green')}OK{attr(0)}"
		try:
			hard_code(test_case)
		except AssertionError as e:
			test_outcome = f"{fg('red')}KO{attr(0)}"
			print(f'exception: {e}')
			return_status = 1
		print(f'[{test_outcome}] on "{test_case["command"]}"')
		if return_status:
			break
	return return_status


def main():
	ret = 0
	if not os.path.isfile('ft_ssl'):
		if not os.path.isfile('Makefile'):
			print('Please make sure you are in the root folder of ft_ssl (not the tests directory)!', file=sys.stderr)
		else:
			print('Please make sure you have compiled succesfully!', file=sys.stderr)
		exit(1)

	with open('/tmp/file', 'w') as f:
		f.write("And above all,\n")
	with open('/tmp/website', 'w') as f:
		f.write("https://www.42.fr/\n")

	with open('tests/tests.json', 'r') as f:
		test_file = json.load(f)

	for test_name in ['MD5', 'SHA256', 'SHA224', 'BASE64', 'DES-ECB', 'DES-CBC', 'Invalid']:
		ret |= run_testcases(test_file[test_name], test_name)

	exit(ret)


main()
