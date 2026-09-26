/* 퀵 정렬 — 피벗 하나를 골라 그보다 작은 쪽과 큰 쪽으로 나눈 뒤,
 * 양쪽을 다시 같은 방법으로 정렬한다.
 *
 * 평균 O(n log n)이지만 피벗을 잘못 고르면 O(n^2)까지 떨어진다.
 * 그래서 피벗 고르는 방법을 quickSortPivot으로 바꿔 끼울 수 있게 했다.
 *
 * 분할은 양쪽 끝에서 가운데로 좁혀 오는 방식(Sedgewick 교재의 partition)을
 * 쓴다. 피벗과 같은 값을 만나도 멈추므로 중복이 많아도 한쪽으로 쏠리지 않는다.
 * 대신 멀리 떨어진 원소끼리 교환하기 때문에 안정 정렬은 아니다.
 */
#include "sort.h"

#include "sortctx.h"

QuickPivot quickSortPivot = PIVOT_MEDIAN3;

/* 앞 · 가운데 · 뒤 세 원소를 제자리에서 정렬해 a[lo] <= a[mid] <= a[hi]로 만든다.
 * 그러면 가운데 값이 곧 세 값의 중앙값이다. 비교는 3번. */
static void sortThree(SortCtx *c, size_t lo, size_t mid, size_t hi) {
    if (sortCompareAt(c, mid, lo) < 0) {
        sortSwap(c, mid, lo);
    }
    if (sortCompareAt(c, hi, mid) < 0) {
        sortSwap(c, hi, mid);
        if (sortCompareAt(c, mid, lo) < 0) {
            sortSwap(c, mid, lo);
        }
    }
}

/* a[lo..hi] (양 끝 포함)을 나누고 피벗이 자리 잡은 위치를 돌려준다.
 * 피벗은 a[lo]로 옮겨 두고, 분할이 끝나면 제자리로 보낸다. */
static size_t partition(SortCtx *c, size_t lo, size_t hi) {
    if (quickSortPivot == PIVOT_MEDIAN3 && hi - lo >= 2) {
        size_t mid = lo + (hi - lo) / 2;
        sortThree(c, lo, mid, hi);
        sortSwap(c, lo, mid); /* 중앙값을 피벗 자리(a[lo])로 */
    }

    size_t i = lo;
    size_t j = hi + 1;
    for (;;) {
        /* 왼쪽에서 피벗 이상인 원소를 찾는다. '<'라서 같은 값에서도 멈춘다. */
        while (sortCompareAt(c, ++i, lo) < 0) {
            if (i == hi) {
                break;
            }
        }
        /* 오른쪽에서 피벗 이하인 원소를 찾는다. */
        while (sortCompareAt(c, lo, --j) < 0) {
            if (j == lo) {
                break;
            }
        }
        if (i >= j) {
            break;
        }
        sortSwap(c, i, j);
    }
    if (j != lo) {
        sortSwap(c, lo, j); /* 피벗을 제자리로 */
    }
    return j;
}

static void quickSortRange(SortCtx *c, size_t lo, size_t hi, size_t depth) {
    sortEnterDepth(c, depth);
    if (lo >= hi) {
        return;
    }
    size_t p = partition(c, lo, hi);
    if (p > lo) {
        quickSortRange(c, lo, p - 1, depth + 1);
    }
    quickSortRange(c, p + 1, hi, depth + 1);
}

void quickSort(void *base, size_t n, size_t size, SortCompare cmp, SortStats *stats) {
    SortCtx c;
    if (!sortBegin(&c, base, n, size, cmp, stats)) {
        return;
    }
    quickSortRange(&c, 0, n - 1, 1);
    sortEnd(&c);
}
