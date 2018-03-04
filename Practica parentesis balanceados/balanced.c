#include <stdio.h>
#define MAX_EXPRESSION 100
int isParenthesisBalanced(char *);


int main()
{
    char expr[MAX_EXPRESSION];


    printf("Enter expression: ");
    scanf("%s", &expr);


    if(isParenthesisBalanced(expr) == 0)
        printf("Balanced parenthesis. \n");
    else
        printf("Unbalanced\n");
    return 0;

}


//returns 0 if the parenthesis are balanced
int isParenthesisBalanced(char *expr)
{
    int bf = 0;
    int i;
    i = 0;
    while(expr[i] != '\0')
    {
        if(expr[i] == '(')
            bf++;
        else if(expr[i] == ')')
        {
            bf--;
            if(bf < 0)
                break;
        }
        i++;
    }
    return bf;
}
