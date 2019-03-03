int infof(const char *format, ...);
int warnf(const char *format, ...);
int errorf(const char *format, ...);
int panicf(const char *format, ...);
void textcolor(int attr, int fg);

int main () {
    
    infof("%s\n", "THIS IS INFO");
    warnf("%s\n", "THIS IS A WARNING");
    errorf("%s\n" ,"THIS IS AN ERROR");
    panicf("%s\n", "THIS IS PANIC");
   
   return 0;
}