vals = [
    0b10000,
    0b10010,
    0b11000,
    0b10100,
    0b01100,
    0b11100,
    0b11010,
    0b11001,
    0b10111,
    0b01110,
]
names = "ABCDEFGHIJKLMNOPQRSS"
rowsticks = {}
colsticks = {}
rm = {}


def rev(n):
    binary = format(n, "05b")
    reversed_binary = int(binary[::-1], 2)
    return reversed_binary


for v, a, b in zip(vals, names[::2], names[1::2]):
    i, j = ord(a) - ord("A") + 1, ord(b) - ord("A") + 1
    s = {a, b, i, j}
    rowsticks[b] = v
    rowsticks[a] = rev(v)
    colsticks[j] = v
    colsticks[i] = rev(v)
    rm[a] = s
    rm[b] = s
    rm[i] = s
    rm[j] = s


sols = set()


visited = set()


def _solve(rows, cols, rsticks, csticks):
    cachekey = (tuple(rows), tuple(cols))
    if cachekey in visited:
        return
    visited.add(cachekey)

    if not any(x is None for x in rows + cols):
        sols.add((tuple(rows), tuple(cols)))
        return

    for stick in csticks:
        v = colsticks[stick]
        for x in range(5):
            if cols[x] is not None:
                continue
            impossible = False
            for y, r in enumerate(rows):
                if r is None:
                    continue
                rv = rowsticks[r] & (1 << x) > 0
                cv = v & (1 << y) > 0
                if not (rv ^ cv):
                    impossible = True
                    break
            if impossible:
                continue

            nrs = rsticks.difference(rm[stick])
            ncs = csticks.difference(rm[stick])
            nc = list(cols)
            nc[x] = stick
            _solve(list(rows), nc, nrs, ncs)

    for stick in rsticks:
        v = rowsticks[stick]
        for y in range(5):
            if rows[y] is not None:
                continue
            impossible = False
            for x, c in enumerate(cols):
                if c is None:
                    continue
                rv = v & (1 << x) > 0
                cv = colsticks[c] & (1 << y) > 0
                if not (rv ^ cv):
                    impossible = True
                    break
            if impossible:
                continue

            nrs = rsticks.difference(rm[stick])
            ncs = csticks.difference(rm[stick])
            nr = list(rows)
            nr[y] = stick
            _solve(nr, list(cols), nrs, ncs)


def solve(sr, sc):
    rss = set(rowsticks.keys())
    css = set(colsticks.keys())
    for stick in sr + sc:
        if stick is None:
            continue
        rss = rss.difference(rm[stick])
        css = css.difference(rm[stick])

    sols.clear()
    visited.clear()
    _solve(list(sr), list(sc), rss, css)
    return sols


import sys

s_rows = [None if x == "_" else x for x in sys.argv[1]]
s_cols = [None if x == "_" else int(x) for x in sys.argv[2].split(",")]

sol = solve(s_rows, s_cols)

print(f"found {len(sol)} solutions")
for r, c in sol:
    print(",".join(str(i) for i in c), "|", "".join(r))
