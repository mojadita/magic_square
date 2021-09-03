/* ms.c -- magic square.
 * Author: Luis Colorado.
 * Date: Fri 03 Sep 2021 02:08:34 PM EEST
 * Copyright: (C) 2021 Luis Colorado.  All rights reserved.
 * License: BSD.
 */

#include <getopt.h>
#include <stdio.h>
#include <stdlib.h>

#define MAX 1023
#define DFLT 11

int square[MAX][MAX];
int sum_main_diag,
    sum_anti_diag,
    sum_cols[MAX],
    sum_rows[MAX];
char DOUBLE_LINE[] = "========================";
char SINGLE_LINE[] = "------------------------";

int main(int argc, char **argv)
{
    int opt; /* option */
    int size = DFLT;

    while ((opt = getopt(argc, argv, "n:")) >= 0) {
        switch (opt) {
        case 'n': size = atoi(optarg);
            if (size < 1 /* size must be >= 1 */
                || size > MAX /* and <= MAX */
                /* || !(size & 1) */ ) /* and odd */
            {
                fprintf(stderr,
                        "N defaulting to %d, as provided "
                        "value (%s) was invalid\n",
                        DFLT, optarg);
                size = DFLT;
            }
            break;
        } /* switch */
    }

    int row = 0, /* top row */
        col = size/2, /* middle cell */
        total = size * size, /* total number of cells */
        i; /* number to put in the cell */

    for (i = 1; i <= total; i++) {
        square[row][col] = i; /* fill the square */
        if (row == col) { /* add to anti_diag */
            sum_anti_diag += i;
        }
        if (row + col + 1 == size) { /* add to main diag */
            sum_main_diag += i;
        }
        sum_cols[col] += i; /* add to col */
        sum_rows[row] += i; /* add to row */

        int nxtrow = row,  /* next row to go */
            nxtcol = col; /* next col to go */
        nxtrow--;
        if (nxtrow < 0) { /* top row */
            nxtrow = size - 1; /* rules 2 & 3 */
        }
        nxtcol++;
        if (nxtcol >= size) { /* right column */
            nxtcol = 0; /* rules 2 & 3 */
        }
        if (square[nxtrow][nxtcol]) { /* rule four */
            /* next cell is the located below the initial */
            nxtrow = row + 1;
            if (nxtrow >= size) { /* check boundary, not needed */
                nxtrow = 0;
            }
            nxtcol = col;
        }
        col = nxtcol; /* ... update */
        row = nxtrow;
    }

    /* print them all */
    int digs = snprintf(NULL, 0, "%d", total);
    int digs_sums = snprintf(NULL, 0, "%d", total*(total+1)/2/size);

    for (row = 0; row < size; row++) {
        printf("%*s +", digs_sums, "");
        for (col = 0; col < size; col++)
            printf("%.*s+", digs_sums, SINGLE_LINE);
        printf("\n%*d |", digs_sums, row);
        for (col = 0; col < size; col++) {
            printf("%*d|", digs_sums, square[row][col]);
            int val = square[row][col];
        }
        printf(" ==> %*d\n", digs, sum_rows[row]);
    }
    printf("%*s +", digs_sums, "");
    for (col = 0; col < size; col++)
        printf("%.*s+", digs_sums, DOUBLE_LINE);
    printf("\n%*d/|", digs, sum_anti_diag);
    for (col = 0; col < size; col++)
        printf("%*d|", digs, sum_cols[col]);
    printf("\\    %d\n", sum_main_diag);

    return 0; /* OK exit code */
}
