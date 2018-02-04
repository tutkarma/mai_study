#include <stdio.h>
#include <ctype.h>
#include "vector.h"

int main(void)
{
    TVector vec = VectorCreate();
    ReadItem(vec);
    VectorRadixSort(vec);
    VectorPrint(vec);
    VectorDestroy(&vec);
    return 0;
}

