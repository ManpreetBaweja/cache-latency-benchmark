import csv
import matplotlib.pyplot as plt

kb, ns = [], []
with open("results.csv", newline="", encoding="utf-16") as f:
    for row in csv.DictReader(f):
        kb.append(float(row["kb"]))
        ns.append(float(row["ns"]))

fig, ax = plt.subplots(figsize=(9, 5))
ax.plot(kb, ns, marker="o", linewidth=1.5)
ax.set_xscale("log", base=2)
ax.set_yscale("log")

# Edit these to your CPU's documented sizes (in KB) once you confirm them
markers = {"L1d (32 KB)": 32, "L2 (512 KB)": 512, "L3 (16 MB)": 16384}
for label, size in markers.items():
    ax.axvline(size, linestyle="--", color="gray", alpha=0.7)
    ax.text(size, ax.get_ylim()[1], label, rotation=90, va="top", ha="right", fontsize=8)

ax.set_xlabel("Array size (KB, log scale)")
ax.set_ylabel("Latency per access (ns, log scale)")
ax.set_title("Memory access latency vs. working set size")
ax.grid(True, which="both", alpha=0.3)
plt.tight_layout()
plt.savefig("latency_curve.png", dpi=150)
plt.show()