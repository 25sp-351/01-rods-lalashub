#include <stdio.h>
#include <stdlib.h>
#include <string.h>


#define MAX_INPUT_LINES 100

typedef struct {
    int length;
    int value;
} Piece;

void read_pieces(Piece *pieces, int *num_pieces) {
    char line[256];
    *num_pieces = 0;

    while (fgets(line, sizeof(line), stdin) != NULL) {
        if (sscanf(line, "%d, %d", &pieces[*num_pieces].length, &pieces[*num_pieces].value) == 2) {
            (*num_pieces)++;
        }
    }
}

void sort_pieces_by_ratio(Piece *pieces, int num_pieces) {
    for (int i = 0; i < num_pieces - 1; i++) {
        for (int j = 0; j < num_pieces - i - 1; j++) {
            double ratio1 = (double)pieces[j].value / pieces[j].length;
            double ratio2 = (double)pieces[j + 1].value / pieces[j + 1].length;
            if (ratio1 < ratio2) {
                Piece temp = pieces[j];
                pieces[j] = pieces[j + 1];
                pieces[j + 1] = temp;
            }
        }
    }
}

void print_cutting_plan(Piece *pieces, int num_pieces, int supplied_length) {
    int remaining_length = supplied_length;
    int total_value = 0;

    for (int i = 0; i < num_pieces && remaining_length > 0; i++) {
        int num_cuts = remaining_length / pieces[i].length;
        if (num_cuts > 0) {
            int value = num_cuts * pieces[i].value;
            printf("%d @ %d = %d\n", num_cuts, pieces[i].length, value);
            remaining_length -= num_cuts * pieces[i].length;
            total_value += value;
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

    print_cutting_plan(pieces, num_pieces, supplied_length);

    return 0;
}
