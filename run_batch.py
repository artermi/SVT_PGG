# run_batch.py
import os, subprocess, itertools
from multiprocessing import Pool

# --- CONFIGURATION ---
r0_values = [2.0, 2.5, 3.0, 3.5, 4.0, 4.5, 5.0]
alphas    = [0.1, 0.2, 0.3, 0.4, 0.5]

models = ["A", "B"]        # which models to run
taus   = [1, 10, 1000]     # only for Model B
seeds  = range(1, 11)      # 10 seeds by default

n_processes = 6
DRY_RUN = False  # True = only print commands

# --- JOB GENERATION ---
jobs = []
for r0, alpha, seed in itertools.product(r0_values, alphas, seeds):
    sigma = round(r0 * alpha, 6)
    env = os.environ.copy()
    env["PG_SEED"] = str(seed)

    for model in models:
        if model == "A":
            cmd = ["./build/public_goods_sim", str(r0), str(sigma), "A"]
            jobs.append((cmd, env))
        elif model == "B":
            for tau in taus:
                cmd = ["./build/public_goods_sim", str(r0), str(sigma), "B", str(tau)]
                jobs.append((cmd, env))

# --- EXECUTION ---
def run_job(job):
    cmd, env = job
    print("Running:", " ".join(cmd), "| PG_SEED=", env["PG_SEED"])
    if DRY_RUN:
        return 0
    return subprocess.run(cmd, env=env).returncode

if __name__ == "__main__":
    with Pool(processes=n_processes) as pool:
        rcodes = pool.map(run_job, jobs)
    bad = sum(1 for r in rcodes if r != 0)
    print(f"Completed with {bad} failures." if bad else "All jobs finished OK.")