#include<stdio.h>

//single line comment
/* 
* * *  * * /Multi 
line 
comment
*/

int main()
{
    int sum=401+5, div = 400/10;
    float f = 10.08f;
    int hex = 0xabcd;
    int bin = 0b10101;
    int oct = 076;
    hex += 10;
    int arr[4] = { 1 , 2 , 3 , 4 };
    char ch = 'a';
    for(int i=10; i!=10; i--);
    for(int i=0; i<=10; i++);
    for(int i=0; i<<10; i++);
    
    for(int i=0; i<=10; i++);
    while (1)
    {
        if(!!0) break;
    }
    printf("The sum of 4 and 5 is %d\n" , sum);
   
    /*
    multi line*/
    //single line
    printf("hello wolrd");
    return 0;
}