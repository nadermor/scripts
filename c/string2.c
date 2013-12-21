#include <stdio.h>
#include <string.h>

/* string library: useful functions
 * char* strcpy(s,ct)     -> copy ct into s, including ``\0''; return s
 * char* strncpy(s,ct,n)  -> copy n charcater of ct into s, return s
 * char* strncat(s,ct)    -> concatenate ct to end of s; return s
 * char* strncat(s,ct,n)  -> concatenate n character of ct to end of s, terminate with ``\0''; return s
 * int strcmp(cs,ct)      -> compare cs and ct; return 0 if cs=ct, <0 if cs0 if cs>ct 
 * char* strchr(cs,c)     -> return pointer to first occurence of c in cs or NULL if not encountered
 * size_t strlen(cs)      -> return length of cs
 *
 * (s and t are char*, cs and ct are const char*, c is an char converted to type int, and n is an int.) 
 */

void main() {
    char line[100], line2[100], *sub_text;

    char str[] = "Hello, I am a string."; // const char*
    printf("Original str: %s\n", str);
    
    strcpy(line, str);
    printf("strcpy(line, str): %s\n", line);
    // Note its strNcpy, not strcpy
    strncpy(line2, str, 10);
    printf("strncpy(line, str, 10): %s\n", line2);

    strcat(line, " what are you?");
    printf("strcat(line, newStr): %s\n", line);

    printf("Length of line: %d\n", strlen(line));

    // first occurrence, note that it can be used into iteration
    if ((sub_text = strchr(line, 'W')) != NULL)
        printf("String starting with \"W\" -> %s\n", sub_text);
    else
        printf("No string starting with \"W\"\n");
    if ((sub_text = strchr(line, 'w')) != NULL)
        printf("String starting with \"w\" -> %s\n", sub_text);
    else
        printf("No string starting with \"w\"\n");


    if ((sub_text = strchr(line, 'u')) != NULL)
        printf("String starting with \"u\" -> %s\n", sub_text);
    else
        printf("No string starting with \"u\"\n");
}
