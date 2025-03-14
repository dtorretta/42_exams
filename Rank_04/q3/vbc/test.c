#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <ctype.h>

typedef struct node {
    enum {
        ADD,
        MULTI,
        VAL
    }   type;
    int val;
    struct node *l;
    struct node *r;
}   node;

node    *new_node(node n)
{
    node *ret = calloc(1, sizeof(n));
    if (!ret)
        return (NULL);
    *ret = n;
    return (ret);
}

void    destroy_tree(node *n)
{
    if (!n)
        return ;
    if (n->type != VAL)
    {
        destroy_tree(n->l);
        destroy_tree(n->r);
    }
    free(n);
}

void    unexpected(char c)
{
    if (c)
        printf("Unexpected token '%c'\n", c);
    else
        printf("Unexpected end of file\n");
}

int accept(char **s, char c)
{
    if (**s == c)
    {
        (*s)++;
        return (1);
    }
    return (0);
}

int expect(char **s, char c)
{
    if (accept(s, c))
        return (1);
    unexpected(**s);
    return (0);
}

int eval_tree(node *tree)
{
    switch (tree->type)
    {
        case ADD:
            return (eval_tree(tree->l) + eval_tree(tree->r));
        case MULTI:
            return (eval_tree(tree->l) * eval_tree(tree->r));
        case VAL:
            return (tree->val);
    }
    return -1;
}





//S --> Es la cadena de caracteres con la expresión matemática a evaluar.
//i → Es un puntero a un índice de la cadena,
int	solve(char *s, int *i)
{
	int nums[500];
	int count = 0;  //  Cuenta cuántos números hemos almacenado en nums
	int nb = 0; // Almacena el número actual que se está procesando.
	char op = '+'; // Última operación leída (por defecto '+')

	while (s[*i] && s[*i] != ')') //Mientras no lleguemos al final de la cadena ni a un paréntesis de cierre ()), seguimos procesando la expresión.
	{
		if (isdigit(s[*i])) //Si el carácter actual es un dígito (0-9), lo convertimos a un número entero restando '0'.
			nb = s[*i] - '0'; //por tema de ascii
			
		else if (s[*i] == '(')
		{
			(*i)++; //avanzamos
			nb = solve(s, i); //Llamamos recursivamente a solve
		}

		if (op == '+')
			nums[count++] = nb; //en el siguiente elemento del array guardamos el nuevo numero (en la primer llamada lo gusrda en la pos 0)
			
		else if (op == '*')
			nums[count - 1] *= nb; //multiplicamos el último valor guardado en nums con nb.
		
		
		op = s[*i]; //amacenamos el nuevo operador. es un pointer de i. ahora i apunta a un numero pero dsp va a avanzar
		if (s[*i] != '\0' && s[*i] != ')') //si no es el final y NO hay parentesis, avanzamos
			(*i)++;
	}
	
	if (s[*i] == ')') 
		(*i)++;

	int k = 0;
	int res = 0;
	while (k < count)
	{
		res += nums[k]; //sumamos todos los valores
		k++;
	}
	return (res);
}





int main(int argc, char **argv)
{
	if (argc != 2)
		return (1);
	int i = 0;
	printf("%d\n", solve(argv[1], &i));
	return (0);
}