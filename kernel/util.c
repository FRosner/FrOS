void memory_copy(char *source, char *dest, int nbytes) {
    int i;
    for (i = 0; i < nbytes; i++) {
        *(dest + i) = *(source + i);
    }
}

// https://stackoverflow.com/questions/9994742/want-to-convert-integer-to-string-without-itoa-function
char *int_to_string(int v, char *buff, int radix_base) {
    static char table[] = "0123456789abcdefghijklmnopqrstuvwxyz";
    char *p = buff;
    unsigned int n = (v < 0 && radix_base == 10) ? -v : (unsigned int) v;
    while (n >= radix_base) {
        *p++ = table[n % radix_base];
        n /= radix_base;
    }
    *p++ = table[n];
    if (v < 0 && radix_base == 10) *p++ = '-';
    *p = '\0';
    return buff;
}
