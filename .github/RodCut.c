#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_INPUT_LINES 100
#define MAX_LINE_LENGTH 256

typedef struct {
    int length;
    int value;
} Piece;

void read_pieces(Piece *pieces, int *num_pieces) {
    char line[MAX_LINE_LENGTH];
    *num_pieces = 0;

    while (fgets(line, sizeof(line), stdin) != NULL) {
        if (sscanf(line, "%d, %d", &pieces[*num_pieces].length, &pieces[*num_pieces].value) != 2) {
            fprintf(stderr, "Invalid input format: %s\n", line);
            continue;
        }
        (*num_pieces)++;
    }
}

int compare_pieces(const void *a, const void *b) {
    double ratio1 = (double)((Piece *)a)->value / ((Piece *)a)->length;
    double ratio2 = (double)((Piece *)b)->value / ((Piece *)b)->length;
    return (ratio2 > ratio1) - (ratio2 < ratio1); 
}

void sort_pieces_by_ratio(Piece *pieces, int num_pieces) {
    qsort(pieces, num_pieces, sizeof(Piece), compare_pieces);
}

void calculate_cutting_plan(Piece *pieces, int num_pieces, int supplied_length, int *cut_counts, int *total_value, int *remaining_length) {
    *remaining_length = supplied_length;
    *total_value = 0;

    for (int ix = 0; ix < num_pieces && *remaining_length > 0; ix++) {
        cut_counts[ix] = *remaining_length / pieces[ix].length;
        if (cut_counts[ix] > 0) {
            *total_value += cut_counts[ix] * pieces[ix].value;
            *remaining_length -= cut_counts[ix] * pieces[ix].length;
        }
    }
}

void print_cutting_plan(Piece *pieces, int num_pieces, int *cut_counts, int remaining_length, int total_value) {
    for (int ix = 0; ix < num_pieces; ix++) {
        if (cut_counts[ix] > 0) {
            printf("%d @ %d = %d\n", cut_counts[ix], pieces[ix].length, cut_counts[ix] * pieces[ix].value);
        }
    }
    printf("Remainder: %d\n", remaining_length);
    printf("Value: %d\n", total_value);
}

int main(int argc, char *argv[]) {
    if (argc != 2) {
        fprintf(stderr, "Usage: %s <supplied rod length>\n", argv[0]);
        return 1;
    }

    int supplied_length = atoi(argv[1]);
    if (supplied_length <= 0) {
        fprintf(stderr, "Error: Supplied rod length must be a positive integer.\n");
        return 1;
    }

    Piece pieces[MAX_INPUT_LINES];
    int num_pieces;
    read_pieces(pieces, &num_pieces);

    sort_pieces_by_ratio(pieces, num_pieces);

    int cut_counts[MAX_INPUT_LINES] = {0};
    int total_value = 0, remaining_length = 0;

    calculate_cutting_plan(pieces, num_pieces, supplied_length, cut_counts, &total_value, &remaining_length);
    print_cutting_plan(pieces, num_pieces, cut_counts, remaining_length, total_value);

    return 0;
}
