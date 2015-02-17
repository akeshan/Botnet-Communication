#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char** argv)
{
    char message[] = "message ";
    strncpy(message, "A", 2);
    puts(message);
}
