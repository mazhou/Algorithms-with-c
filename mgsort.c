/**
 * 归并排序，是一种运用分支排序算法。
 * 同快速排序一样，它依赖于元素之间的比较来排序。
 * 但是，归并排序需要额外的存储空间来完成排序
 *
 * 原理：
 * 将数据分为两个堆，递归分，直到只剩下一个。
 * 然后合并成一个有序的集合
 *
 * 归并排序和其他排序最大的不同在于他的归并过程。将两个有序的数据集合合并成一个有序的数据集
 * 而合并两个有序的数据集的过程是最高效的，因为只需要一次遍历即可。
 * 根据以上，再加上归并排序算法是按照可预期的方式来划分数据的
 * 这使得归并排序在任何情况下都可以达到快排的平均性能。
 *
 * 然而它需要额外的存储空间来运行,
 * 因为合并过程不能在无序数据集本身中进行，
 * 所以必须要有两倍于无序数据集的空间来进行算法。
 * 对小批量一般使用快排，对海量数据一般用归并排序
 */

/**
 * 归并排序的关键在于如何将两个有序集合合并成一个有序集合，
 * 这部分工作由merge完成。
 * 它将 data 中 i 到 j 之间的数据集与 j + 1 到 k 之间的数据集合并成一个 i 到 k 的有序数据集
 * 
 * 最初， ipos 和 jpos 指向每个有序集的头部，只要数据集中还有元素存在，合并过程就将继续，如果没有元素可以进行下面的操作
 * 如果一个集合没有要合并的元素，那么将另外一个集合中要合并的元素全部放到合并的集合中
 * 否则，首先比较两个集合中的首元素，判断哪个元素将要放到合并的集合中。然后将它放置进去，
 * 接着根据元素来自的集合移动 ipos 和 jpos 的位置，以此类推
 *
 */
#include <stdlib.h>
#include <string.h>
#include "sort.h"

/**
 * 初次调用时，i 设为 0 ， k 设置为 size - 1，
 * 分割data, j 处于元素中间的位置
 * 然后调用mgsort来处理左分区(i 到 j)。左分区继续递归。直到传入mgsort的分区只包含一个元素。
 * i 将小于 k, 一次调用终止。
 * 右边一样，只是处理 j + 1 到 k.
 */
static int merge(void *data, int size, int esize, int i, int k, int (*compare)(const void *key1, const void *key2))
{
    char *a = data,
         *m;
    int ipos,
        jpos,
        mpos;

    ipos = i;
    jpos = j + 1;
    mpos = 0;

    if ((m = (char *)malloc(esize * ((k - i) + 1))) == NULL) {
        return -1;
    }

    // 继续，无论分割中有没有元素
    while (ipos <= j || jpos <= k) {
        if (ipos > j) {
            // 左分割没有更多的元素去合并
            while (jpos <= k) {
                memcpy(&m[mpos * esize], &a[jpos * esize], esize);
                jpos++;
                mpos++;
            }
            continue;
        }else if (jpos > k) {
            // 又分割没有更多的元素就去合并
            while (ipos <= j) {
                memcpy(&m[mpos * esize], &a[jpos * esize], esize);
                ipos++;
                mpos++;
            }
            continue;
        }

        // 追加下一个有序元素到合并的元素
        if (compare(&a[ipos * esize], &a[jpos * esize]) < 0) {
            memcpy(&m[mpos * esize], &a[ipos * esize], esize);
            ipos++;
            mpos++;
        }else {
            memcpy(&m[mpos * esize], &a[jpos * esize], esize);
            jpos++;
            mpos++;
        }
    }
    // 准备返回已合并的数据
    memcpy(&a[i * esize], m, esize * ((k - i) + 1));

    free(m);
    return 0;
    

}

int mgsort(void *data, int size, int esize, int i, int k, int (*compare)(const void *key1, const void *key2))
{
    int j;
    
    // 当没有分割可以被创建的时候停止递归
    if (i < k) {
        // 确定哪里分割元素
        j = (int)(((i + k - 1)) / 2);

        // 递归地排序两个分割
        if (mgsort(data, size, esize, i, j, compare) < 0) {
            return -1;
        }

        if (mgsort(data, size, esize, j + 1, k, compare) < 0) {
            return -1;
        }

        if (mgsort(data, esize, i, j, k, compare) < 0) {
            return -1;
        }
    }
    return 0;
}
