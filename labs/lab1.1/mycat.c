#include <stdio.h>
#include <fcntl.h>
//#include <time.h>

/* filecopy:  copy file ifp to file ofp */
void filecopy(int ifp, int ofp)
{
    char buf[512];
    int c;

    while ((c = read(ifp, buf, sizeof(buf))) > 0){
        write(ofp, buf, c);
    }

}

/* cat:  concatenate files, version 2 */
int main(int argc, char *argv[])
{
    //clock_t begin = clock();
    
    int fp;
    void filecopy(int ifp, int ofp);
    char *prog = argv[0];   /* program name for errors */

    if (argc == 1)  /* no args; copy standard input */
        filecopy(0, 1);
    else
        while (--argc > 0)
            if ((fp = open(*++argv, O_RDONLY, 0)) == -1) {
                fprintf(stderr, "%s: can′t open %s\n", prog, *argv);
                return 1;
            } else {
                filecopy(fp, 1);
                close(fp);
            }

    if (ferror(stdout)) {
        fprintf(stderr, "%s: error writing stdout\n", prog);
        return 2;
    }

    //clock_t end = clock();

    //printf("\nTime: %f", (double)(end - begin) / CLOCKS_PER_SEC);

    return 0;
}