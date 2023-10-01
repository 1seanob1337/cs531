import random
import string
import itertools

def generate_unique_ips(n):
    all_ips = [f"{a}.{b}.{c}.{d}" for a in range(256) for b in range(256) for c in range(256) for d in range(10)]  # Limiting the last octet for performance
    random.shuffle(all_ips)
    return all_ips[:n]

def generate_all_names(max_length):
    chars = string.ascii_lowercase
    names = []
    for length in range(1, max_length + 1):
        for name_tuple in itertools.product(chars, repeat=length):
            names.append(''.join(name_tuple))
            if len(names) >= 100_000_000:  # If we've reached 100M names, we break
                return names
    return names

def main(n, filename="./temp"):
    ips = generate_unique_ips(n)
    all_names = generate_all_names(6)  # 6 chars should be more than enough for our constraint of 100M. Adjust as needed.
    random.shuffle(all_names)
    
    with open(filename, 'w') as f:
        for i in range(n):
            f.write(f"{ips[i]} {all_names[i]}\n")

if __name__ == "__main__":
    N = int(input("Enter the number of lines to generate (max 100M): "))
    assert N <= 100_000_000, "N should be <= 100M"
    main(N)
