# A More Precise Algorithm for tnum Multiplication

This repo contains the test program to benchmark a rewritten version
of `tnum_mul`, the function used in the Linux kernel to multiply
tristate numbers. The new algorithm addresses a challenge ("How can we
incorporate correlation in unknown bits across partial products?")
pointed out by Harishankar et al. in [their
paper](https://arxiv.org/abs/2105.05398).

The program considers all tristate pairs possible with the specified
number of bits (option: `--bits N`), and finds their concrete sets,
the exact product set, and the optimal product set. Then it finds the
product sets of the current algorithm and the new algorithm,
proceeding to compare the outputs and update the stats.

Sample output:

```
$ ./experi --bits 6
...
mine vs kernel (bits = 6): 
  better = 285059
  same   = 229058
  worse  = 17324
  myprod optimal cases  = 432406 / 531441
  linprod optimal cases = 202444 / 531441
----------------------------------------------------------
is optimal? (bits = 6): 0
```

The above output shows the new algorithm was an improvement over the
current one in 285k cases, and worse in 17k, while preserving the
precision in 229k cases. It achieved optimality in in 81% cases compared to
the 38% obtained by the current algorithm.

Notes:

- There are 531441 combinations for 6 bits because it is `3^6 * 3^6`
  (two tristate numbers of width 6, each trit having three possible
  values).
- Ill-formed tnums (value = mask = 1) are not processed.

## Overview of the Improvement

While work is underway on a detailed explanation on the new algorithm
and its soundness proof, please refer to the following to get an
initial picture:

1. The comments for `my_tnum_mul` in lib.c
2. Quote from the paper by Harishankar et al., given in this section
3. An example given in this section

From the paper by Harishankar et al.:

> While our\_mul is sound, it is not optimal. Key questions
> remain in designing a sound and optimal algorithm with O(n)
> or faster run time. (1) How can we incorporate correlation in
> unknown bits across partial products? For example, multiplying
> P = 11, Q = &#120583;1 produces the partial products T1 = 11, T2 =
> &#120583;&#120583;0. However, the two &#120583; trits in T2 are concretely either both
> 0 or both 1, resulting from the same &#120583; trit in Q. Failing to
> consider this in the addition makes the result imprecise.
> ...

Example showing the above-mentioned imprecision with the current
`tnum_mul(a, b)`, with a = &#120583;1 and b = 11.

```
    11 *
    x1
------
    11
   xx
------
  xxx1
```

The same example with the new algorithm:

```
    11 *
    x1
------
    11
   xx
------
  ????

acc_0:

    11 *
    01
------
    11
   00
------
  0011

acc_1:

    11 *
    11
------
    11
   11
------
  1001

Union:

Value = 0011 & 1001 = 0001
Mask  = (0011 ^ 1001) | 0000 | 0000 = 1010

Result = x0x1 (more precise compared to xxx1)
```

## Kernel Version

This repo contains the code for "the current algorithm" from the
kernel, which is taken from:

```
commit 0cc53520e68bea7fb80fdc6bdf8d226d1b6a98d9 (origin/master, origin/HEAD, master)
Merge: dfc0f6373094 e2f9ae91619a
Author: Linus Torvalds <torvalds@linux-foundation.org>
Date:   Wed Aug 13 20:23:32 2025 -0700

    Merge tag 'probes-fixes-v6.17-rc1' of git://git.kernel.org/pub/scm/linux/kernel/git/trace/linux-trace
```

## Building

Parts of this program are written in a custom language (ngg), whose
compiler is not released yet. This shouldn't be an issue since the
transpiled human-readable C files are also included in this repo. So a
simple `make` should work.

## Options

- `--commutative`: try both `P*Q` and `Q*P`; then pick the best based
  on the popcount of result.mask.

- `--bits N`: set the range to [0, 2^N) (for each of P.mask, P.value,
  Q.mask, and Q.value, excluding the ill-formed ones).

- `--print-sets`: print the concrete sets.

## Copyright

Copyright 2025 Nandakumar Edamana; lib.c contains snippets from the Linux
kernel. Released under the terms of the GNU General Public License version 2
only.
