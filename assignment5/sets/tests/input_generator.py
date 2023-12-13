#! /usr/bin/env python3

import random
import argparse

minrange = -100000
maxrange = 100000

argparser = argparse.ArgumentParser(
    description="input and reference output generator")
argparser.add_argument("-n", "--numops", type=int, default=100,
                       help="number of set operations to generate")
argparser.add_argument("-s", "--sorted", action='store_true', default=False,
                       help="sorted numbers instead of random")
argparser.add_argument("-pi", type=int, default=33,
                       help="percentage of insert ops [0,100]")
argparser.add_argument("-pl", type=int, default=33,
                       help="percentage of lookup ops [0,100]")

args = argparser.parse_args()

f = open("output.ref", 'w')

s = set()
num = 0
for i in range(args.numops):
    c = random.randint(0,99)
    if args.sorted:
        num = num + 1
    else:
        num = random.randint(minrange, maxrange)

    if c < args.pi:
        print("+", num)
        s.add(num)
    elif c < args.pi + args.pl:
        print("?", num)
        if (num in s):
            f.write("found: ")
        else:
            f.write("not found: ")
        f.write(str(num))
        f.write("\n")
    else:
        print("-", num)
        s.discard(num)
        
print("p")
for e in sorted(s):
    f.write(str(e))
    f.write("\n")

f.close()
