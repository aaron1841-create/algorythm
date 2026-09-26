"""힙 정렬의 1단계(힙 만들기)만 따로 비교 횟수를 센다.

    python3 tools/heap_build_count.py

src/heapSort.c의 siftDown과 같은 방식으로 짰다. "힙 만들기는 O(n)"이라는
설명이 맞는지, n을 키우며 비교 횟수 / n 이 일정한지 본다.
"""

import random


def build_heap_compares(a):
    n = len(a)
    compares = 0
    for i in range(n // 2 - 1, -1, -1):
        tmp = a[i]
        while True:
            child = 2 * i + 1
            if child >= n:
                break
            if child + 1 < n:
                compares += 1
                if a[child] < a[child + 1]:
                    child += 1
            compares += 1
            if a[child] <= tmp:
                break
            a[i] = a[child]
            i = child
        a[i] = tmp
    return compares


def main():
    print("n, 힙 만들기 비교, 비교/n")
    for n in (1000, 2000, 4000, 8000, 16000, 32000):
        random.seed(1)
        c = build_heap_compares([random.random() for _ in range(n)])
        print(f"{n}, {c}, {c / n:.2f}")


if __name__ == "__main__":
    main()
