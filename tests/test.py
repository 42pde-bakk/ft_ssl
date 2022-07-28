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
    assert(testcase['expected_stdout'] == my_stdout), f'STDOUT:\n\tgot \'{my_stdout.encode("unicode_escape")}\',\n\texpected: \'{testcase["expected_stdout"].encode("unicode_escape")}\''
    assert(testcase['expected_stderr'] == my_stderr), f'STDERR:n\n\ttgot \'{my_stderr.encode("unicode_escape")}\',\n\texpected: \'{testcase["expected_stderr"].encode("unicode_escape")}\''


def test_md5():
    with open('tests/tests.json', 'r') as f:
        test_cases = json.load(f)
    return_status = 0
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
    if not os.path.isfile('ft_ssl'):
        if not os.path.isfile('Makefile'):
            print('Please make sure you are in the root folder of ft_ssl (not the tests directory)!', file=sys.stderr)
        else:
            print('Please make sure you have compiled succesfully!', file=sys.stderr)
        exit(1)

    with open('/tmp/file', 'w') as f:
        f.write("And above all,\n")

    ret = 0
    ret |= test_md5()
    exit(ret)


main()
