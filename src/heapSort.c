/* 힙 정렬 — 수업에서 다루지 않은 정렬.
 *
 * 배열을 "최대 힙"으로 본다. 0번이 뿌리이고, i번의 자식은 2i+1, 2i+2번이다.
 * 최대 힙에서는 부모가 늘 자식보다 크거나 같으므로 가장 큰 값이 0번에 있다.
 *
 *   1단계 (힙 만들기) : 마지막 부모부터 거꾸로 올라가며 siftDown을 한다. O(n)
 *   2단계 (꺼내기)     : 0번(최댓값)을 배열 끝으로 보내고, 남은 앞부분을
 *                       다시 힙으로 고친다. 이것을 n-1번 반복한다. O(n log n)
 *
 * 추가 배열도 재귀도 없다. 어떤 입력이 와도 O(n log n)이 보장되지만,
 * 멀리 떨어진 원소를 맞바꾸므로 안정 정렬은 아니다.
 */
#include "sort.h"

#include "sortctx.h"

/* tmp에 들고 있는 원소를 i번 "빈자리"에서 시작해 아래로 내려 보낸다.
 * 힙의 크기는 n이다.
 *
 * 교환(이동 3번)을 거듭하는 대신 빈자리를 끌고 내려간다. 큰 자식을 한 칸
 * 위로 올리는 데 이동 1번, 마지막에 tmp를 내려놓는 데 1번이면 된다.
 * 삽입 정렬이 원소를 한 칸씩 미는 것과 같은 요령이다. */
static void siftDown(SortCtx *c, size_t i, size_t n) {
    for (;;) {
        size_t child = 2 * i + 1;
        if (child >= n) {
            break;
        }
        /* 두 자식 중 큰 쪽을 고른다. */
        if (child + 1 < n && sortCompareAt(c, child, child + 1) < 0) {
            child++;
        }
        /* 큰 자식이 tmp보다 크지 않으면 여기가 tmp의 자리다. */
        if (sortCompareTmp(c, child) <= 0) {
            break;
        }
        sortMove(c, sortElemAt(c, i), sortElemAt(c, child));
        i = child;
    }
    sortMove(c, sortElemAt(c, i), c->tmp);
}

void heapSort(void *base, size_t n, size_t size, SortCompare cmp, SortStats *stats) {
    SortCtx c;
    if (!sortBegin(&c, base, n, size, cmp, stats)) {
        return;
    }

    /* 1단계: 자식이 있는 마지막 원소(n/2 - 1)부터 0번까지 거꾸로 고친다. */
    for (size_t i = n / 2; i-- > 0;) {
        sortMove(&c, c.tmp, sortElemAt(&c, i));
        siftDown(&c, i, n);
    }

    /* 2단계: 뿌리(최댓값)를 뒤로 보내고 힙을 한 칸 줄인다. */
    for (size_t end = n - 1; end > 0; end--) {
        sortMove(&c, c.tmp, sortElemAt(&c, end));           /* 끝 원소를 들고 */
        sortMove(&c, sortElemAt(&c, end), sortElemAt(&c, 0)); /* 최댓값을 끝에 */
        siftDown(&c, 0, end);                               /* 들고 있던 것을 뿌리에서 내린다 */
    }
    sortEnd(&c);
}
