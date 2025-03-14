#include "argo.h"
#include <stdlib.h>

//this file is given in the exam

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

//main

void free_json(json j)
{
    switch (j.type)
    {
        case MAP:
            for (size_t i = 0; i < j.map.size; i++)
            {
                free(j.map.data[i].key);  // Liberar memoria de la clave
                free_json(j.map.data[i].value);  // Liberar memoria del valor
            }
            free(j.map.data);  // Liberar la memoria del mapa
            break;
        case STRING:
            free(j.string);  // Liberar la memoria de la cadena
            break;
        default:
            break;
    }
}

void	serialize(json j)
{
	switch (j.type)
	{
		case INTEGER:
			printf("%d", j.integer);
			break ;
		case STRING:
			putchar('"');
			for (int i = 0; j.string[i]; i++)
			{
				if (j.string[i] == '\\' || j.string[i] == '"')
					putchar('\\');
				putchar(j.string[i]);
			}
			putchar('"');
			break ;
		case MAP:
			putchar('{');
			for (size_t i = 0; i < j.map.size; i++)
			{
				if (i != 0)
					putchar(',');
				serialize((json){.type = STRING, .string = j.map.data[i].key});
				putchar(':');
				serialize(j.map.data[i].value);
			}
			putchar('}');
			break ;
	}
}

int	main(int argc, char **argv)
{
	if (argc != 2)
		return 1;
	char *filename = argv[1];
	FILE *stream = fopen(filename, "r");
	json	file;
	if (argo(&file, stream) != 1)
	{
		free_json(file);
		fclose(stream); //sin esto da memory leaks
		return 1;
	}
	
	serialize(file);
	free_json(file);
	fclose(stream); //sin esto da memory leaks
	printf("\n");
}


/*







int parser_map(json *dst, FILE *stream)
{
	dst->type = MAP;
	dst->map.data = NULL;
	dst->map.size = 0;
	
	int c = getc(stream); //{
	if(peek(stream) == '}') //{}
		return 1;
	
	while(1)
	{
		c = peek(stream); //{"
		if(c != '"')
			return (-1);
		
		dst->map.data =  realloc(dst->map.data, (dst->map.size + 1) * sizeof(pair));
		pair *current = &dst->map.data[dst->map.size];
		current->key = parser_str(stream); //{"hello" hasta aca aceptado
		if(current->key == NULL)
			return -1;
		
		dst->map.size++;
		
		if(expect(stream, ':') == 0)//si no ocurre {"hello" :
			return -1;
		
		//{"hello" : hasta aca aceptado
		
		if(argo(&current->value, stream) == -1)
			return -1;
		
		//{"hello" : 42 hasta aca aceptado
		
		c = peek(stream);
		
		if (c == '}')
		{
			accept(stream, c);
			break;
		}
		else if (c == ',')
		{
			accept(stream, c); //solo aceptar y siguien iterando en el while loop leyendo la siguiente "clave"
		}		
		else
		{
			unexpected(stream);
			return(-1);
		}
	}
	return 1;
}



char *parser_str(FILE *stream)
{
	char *res = calloc(4096, sizeof(char));
	if (!res)
		return NULL;
	
	int i = 0;
	int c = getc(stream); // "
	
	while (1)
	{
		c = getc(stream); // el sigueinte 'H'
		if (c == '"')
			break;
		if (c == '\\')
			c = getc(stream); //el siguiente
		if (c == EOF)
		{
			free(res);
			unexpected(stream);
			return NULL;
		}
		res[i++] = c;
	}
	return(res);
}


int parser_int(json *dst, FILE *stream)
{
	dst->type = INTEGER;
	dst->integer = -42;
	int res;
	int retorno;
	retorno = fscanf(stream, "%d", &res); //ver que pasa si no pongo &
	//printf("retorno: %d\n", retorno); //borrar
	if(retorno <= 0 || retorno == EOF)
	{
		unexpected(stream);
		return(-1);	
	}
	dst->integer = res;
	return (1);	
}

int	argo(json *dst, FILE *stream)
{
	int c = peek(stream); //peeeeeek
	
	if (c == '-' || isdigit(c)) //interger OK
	{
		//printf("check 00\n"); //borrar
		return(parser_int(dst, stream));	
	}
	
	else if (c == '"') //string OK
	{
		//printf("check 00\n"); //borrar
		dst->type = STRING;
		dst->string = parser_str(stream);
		//printf("check dst->string: %s\n",dst->string); //borrar
		if(dst->string == NULL)
		{
			//printf("check 01\n"); //borrar
			return(-1);
		}
		//printf("check 02\n"); //borrar
		return 1;	
	}
	else if (c == '{') //map
	{
		return(parser_map(dst, stream));	
	}
	else
	{
		unexpected(stream);
		return(-1);
	}
	return (-1);

}



*/