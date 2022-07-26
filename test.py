import sys
import subprocess
from typing import Tuple


def run_process(arg: str) -> Tuple[int, str, str]:
    p = subprocess.Popen(f'{arg}', shell=True, stdout=subprocess.PIPE, stderr=subprocess.PIPE)
    p.wait()
    stdout = p.stdout.read().decode('ascii')
    stderr = p.stderr.read().decode('ascii')
    return p.returncode, stdout, stderr


def check_hashes(my_stdout: str, md5sum_stdout: str) -> bool:
    my_stdout_split, md5sum_stdout_split = my_stdout.split(), md5sum_stdout.split()
    if len(my_stdout_split) != len(md5sum_stdout_split):
        print(f'not same length')
        return False
    for my_line, md5sum_line in zip(my_stdout_split, md5sum_stdout_split):
        if my_line.split() != md5sum_line.split():
            print(f'{my_line} != {md5sum_line}')
            return False
    return True


def test_md5():
    test_cases = [
        'README.md',
        # 'haha hihi'
    ]
    return_status = 0
    for test_case in test_cases:
        test_outcome = 'OK'
        my_ret, my_stdout, my_stderr = run_process(f'./ft_ssl md5 {test_case}')
        og_ret, og_stdout, og_stderr = run_process(f'md5sum {test_case}')
        # print(f'ret = {my_ret}')
        # print(f'stdout ="{my_stdout}"')
        # print(f'stderr ="{my_stderr}"')
        if my_ret != og_ret or not check_hashes(my_stdout, og_stdout):
            test_outcome = 'KO'
            return_status = 1
        print(f'[{test_outcome}] on "./ft_ssl md5 {test_case}"')

    return return_status


def main():
    ret = 0
    ret |= test_md5()
    exit(ret)


main()
