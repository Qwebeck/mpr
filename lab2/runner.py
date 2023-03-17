import os

# mpiexec -np 1 ./root.py 1e10
# 3.1415699588
# Elapsed time: 185.927560206

# [ares][plgforobohd@ac0755 lab2]$ mpiexec -np 1 ./root.py 1e6
# 3.141036
# Elapsed time: 0.025546171


small = 1e6
medium = 1e7
large = 1e10


def run_scaling_experiment(scaling_type):
    for np in range(1, 13, 1):
        for problem_size in [small, medium, large]:
            results_file = f'{scaling_type}_{np}_{problem_size}'
            if scaling_type == 'strong':
                problem_size = problem_size / np
            os.system(
                f'mpiexec -np {np} ./root.py {problem_size} {results_file}')


if __name__ == '__main__':
    run_scaling_experiment('strong')
    run_scaling_experiment('weak')
