#pragma once
#include <stdio.h>
#include <stdlib.h>


#define RAND(min, max) \ 
        ((rand() % (max - min + 1) +min))