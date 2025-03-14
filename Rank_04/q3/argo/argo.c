#include "argo.h"
#include <stdlib.h>

/*
// Si el input es "{}"
char c = peek(stream);  // c == '{'
char d = peek(stream);  // d sigue siendo '{' (no avanzó)
*/
//Mira el SIGUIENTE carácter del archivo sin mover el puntero de lectura.
int	peek(FILE *stream)
{
	int	c = getc(stream); // Lee el siguiente carácter
	ungetc(c, stream); // Lo devuelve al stream
	return c; // Retorna el valor del carácter leído
}

void	unexpected(FILE *stream)
{
	if (peek(stream) != EOF)
		printf("unexpected token '%c'\n", peek(stream));
	else
		printf("unexpected end of input\n");
}

/*
Chequea si el siguiente carácter del archivo es c.
Si lo es, lo consume (avanza el puntero del archivo) y retorna 1 (verdadero).
Si no lo es, no hace nada y retorna 0 (falso).
*/
/*
Si el input es "{}"
accept(stream, '{');  // Avanza y consume '{'
accept(stream, '}');  // Avanza y consume '}'
accept(stream, '}');  // Retorna 0 porque ya no hay '}'
*/
int	accept(FILE *stream, char c)
{
	if (peek(stream) == c) // Lee el siguiente carácter
	{
		(void)getc(stream);  // Consumirlo
		return 1;
	}
	return 0;
}

int	expect(FILE *stream, char c)
{
	if (accept(stream, c))
		return 1;
	unexpected(stream);
	return 0;
}

int parse_int(json *dst, FILE *stream)
{
	int res;
	int retorno;
	
	dst->type = INTEGER;
	dst->integer = -42;
	retorno = fscanf(stream, "%d", &res);
	if(retorno <=  0 || retorno == EOF)
	{
		unexpected(stream);
		return -1;	
	}
	dst->integer = res;
	return 1;
}

char *parse_str(FILE *stream)
{
	char *res = calloc(4096, sizeof(char));
	if (!res)
		return NULL;
	
	int i = 0;
	int c = getc(stream); //"
	while(1)
	{
		c = getc(stream); //"H
		if (c == '"')
		{
			accept(stream, c);
			break;
		}
		if (c == '\\')
		{
			c = getc(stream); //sigue
		}
		if(c == EOF)
		{
			free(res);
			unexpected(stream);
			return NULL;
		}
		res[i++] = c;
	}
	return (res);	
}

int parse_map(json *dst, FILE *stream)
{
	dst->type = MAP;
	dst->map.data = NULL;
	dst->map.size = 0;
	
	int c = getc(stream); //{
	if(peek(stream) == '}') //{}
		return 1;
	
	while(1)
	{
		c = peek(stream); 
		if(c != '"') //si o si {"
			return -1;
		
		dst->map.data = realloc(dst->map.data, (dst->map.size + 1) * sizeof(pair));
		pair *current = &dst->map.data[dst->map.size]; //probar sin el &
		current->key = parse_str(stream);
		if(current->key == NULL)
			return -1;
		dst->map.size++;
		
		//{"hello"		
		if(expect(stream, ':') == 0) //{"hello" : si o si
			return -1;
		if(argo(&current->value, stream) != 1) //probar sin el &
			return -1;
		
		//{"hello" : 42 
		c = peek(stream);
		
		if(c == '}')
		{
			accept(stream, c);
			break;
		}
		else if(c == ',')
		{
			accept(stream, c);
		}
		else
		{
			unexpected(stream);
			return -1;
		}	
	}
	return 1;
}

int	argo(json *dst, FILE *stream)
{
	int c = peek(stream);
	if(c == '-' || isdigit(c))
		return(parse_int(dst, stream));
	else if(c == '{')
		return(parse_map(dst, stream));
	else if(c == '"')
	{
		dst->type = STRING;
		dst->string = parse_str(stream);
		if(dst->string == NULL)
		{
			//free(dst->string); //ver
			return -1;
		}
		return 1;
	}
	else
	{
		unexpected(stream);
		return -1;
	}
	return 1;
}
