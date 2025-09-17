# run_batch.py
import os, subprocess, itertools
from multiprocessing import Pool
from pathlib import Path

# --- CONFIGURATION ---
r0_values = [2.0, 2.5, 3.0, 3.5, 4.0, 4.5, 5.0]
alphas    = [0.1, 0.2, 0.3, 0.4, 0.5]

models = ["A","B"]         # which models to run
taus   = [1,10,100,1000]   # only for Model B
seeds  = range(6, 21)       # seeds 1..5

n_processes = 6
DRY_RUN = False  # True = only print commands

# --- PATHS ---
ROOT = Path(__file__).resolve().parent
BIN_NAME = "public_goods_sim.exe" if os.name == "nt" else "public_goods_sim"
BIN_PATH = (ROOT / "build" / BIN_NAME).resolve()

if not BIN_PATH.exists():
    raise FileNotFoundError(f"Simulator not found at: {BIN_PATH}")

# ensure top-level data/ exists
DATA_ROOT = ROOT / "data"
DATA_ROOT.mkdir(exist_ok=True)

# --- JOB GENERATION ---
jobs = []
for r0, alpha, seed in itertools.product(r0_values, alphas, seeds):
    sigma = round(r0 * alpha, 6)
    env = os.environ.copy()
    env["PG_SEED"] = str(seed)

    # per-seed folder inside data/
    seed_dir = DATA_ROOT / f"seed{seed:03d}"
    seed_dir.mkdir(parents=True, exist_ok=True)

    def make_cmd(*args):
        return [str(BIN_PATH), *(str(a) for a in args)]

    for model in models:
        if model == "A":
            cmd = make_cmd(r0, sigma, "A")
            jobs.append((cmd, env, seed_dir))
        elif model == "B":
            for tau in taus:
                cmd = make_cmd(r0, sigma, "B", tau)
                jobs.append((cmd, env, seed_dir))

# --- EXECUTION ---
def run_job(job):
    cmd, env, seed_dir = job
    print(f"Running: {' '.join(cmd)} | PG_SEED={env['PG_SEED']} | cwd={seed_dir}")
    if DRY_RUN:
        return 0
    result = subprocess.run(cmd, env=env, cwd=str(seed_dir))
    return result.returncode

if __name__ == "__main__":
    with Pool(processes=n_processes) as pool:
        rcodes = pool.map(run_job, jobs)
    bad = sum(1 for r in rcodes if r != 0)
    print(f"Completed with {bad} failures." if bad else "All jobs finished OK.")
