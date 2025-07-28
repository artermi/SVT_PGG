# run_batch.py
import subprocess
import itertools

r0_values = [2.0, 2.5, 3.0, 3.5, 4.0, 4.5, 5.0]
alphas = [0.1, 0.2, 0.3, 0.4, 0.5]

jobs = []
for r0, alpha in itertools.product(r0_values, alphas):
    sigma = round(r0 * alpha, 4)
    cmd = ["./build/public_goods_sim", str(r0), str(sigma)]
    jobs.append(cmd)

# Launch with parallel execution (adjust number of workers as needed)
from multiprocessing import Pool

def run_job(cmd):
    print("Running:", " ".join(cmd))
    subprocess.run(cmd)

with Pool(processes=6) as pool:  # Adjust CPU usage here
    pool.map(run_job, jobs)