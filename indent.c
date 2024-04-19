#include "indent.h"
#include <stdio.h>
#include <stdlib.h>

void print_indent(int indent) {
    for(int i = 0; i < indent; i++) {
        printf("\t");
    }
}