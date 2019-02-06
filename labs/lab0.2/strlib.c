int strlen(char *str){
    char *r;
    int len = 0;
    
    for(r = str; *r != '\0'; r++){
        len++;
    }
    
    return len;
}

char* stradd(char *origin, char *addition){
    int originalLen = strlen(origin);
    int additionLen = strlen(addition);
    int i;
    int j;
    
    char *newString = malloc(originalLen + additionLen + 1);
    
    for(i = 0; i < originalLen; i++){
        newString[i] = origin[i];
    }
    
    for(i = 0; i < additionLen; i++){
        j = originalLen + i;
        newString[j] = addition[i];
    }
    
    return newString;
}

int strfind(char *origin, char *substr){
    int i;
    int subCount = 0;
    
    for(i = 0; i < strlen(origin); i++){
        if(origin[i] == substr[subCount]){
            subCount++;
            if(subCount == strlen(substr)){
                return 1;
            }
        } else {
            subCount = 0;
        }
    }
    
    return 0;    
}