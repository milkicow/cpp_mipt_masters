import pandas as pd
import matplotlib.pyplot as plt

df = pd.read_csv("collisions.csv")

for col in df.columns[1:]:
    plt.plot(df["N"], df[col], label=col)

plt.xlabel("Number of strings (N)")
plt.ylabel("Collisions")
plt.legend()
plt.grid()
plt.savefig("hash_collisions")
plt.show()