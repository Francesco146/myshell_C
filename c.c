#include <unistd.h>
int main(int argc, char *argv[])
{
    char *command[15];
    command[0] = "gcc";
    command[1] = argv[1];
    command[2] = "-lm";
    command[3] = "-g";
    command[4] = "-Wall";
    command[5] = "-Werror";
    command[6] = "-Wextra";
    command[7] = "-o";
    command[8] = argv[2];
    command[9] = "-g2";
    command[10] = "-O3";
    command[11] = "-pedantic";
    command[12] = "-dM";
    command[13] = "-W";
    command[14] = NULL;
    execvp("cc", command);
    return 0;
}
