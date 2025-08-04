# run_batch.py
import subprocess
import itertools
from multiprocessing import Pool

# --- CONFIGURATION ---
r0_values = [2.0, 2.5, 3.0, 3.5, 4.0, 4.5, 5.0]
alphas = [0.1, 0.2, 0.3, 0.4, 0.5]
model = "B"  # Change to "A" or "B"
tau = 100    # Only used if model == "B"
n_processes = 6

# --- JOB GENERATION ---
jobs = []
for r0, alpha in itertools.product(r0_values, alphas):
    sigma = round(r0 * alpha, 4)
    cmd = ["./build/public_goods_sim", str(r0), str(sigma), model]
    if model == "B":
        cmd.append(str(tau))
    jobs.append(cmd)

# --- EXECUTION ---
def run_job(cmd):
    print("Running:", " ".join(cmd))
    subprocess.run(cmd)

with Pool(processes=n_processes) as pool:
    pool.map(run_job, jobs)