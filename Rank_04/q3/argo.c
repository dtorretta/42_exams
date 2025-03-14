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

//se supone que desde aca es lo que tenemos que agregar

/*
1 → Se leyó un número entero con éxito.
0 → No se pudo leer un número (por ejemplo, si el input es abc).
EOF (-1) → Se alcanzó el final del archivo antes de leer un número.
*/
int	parse_int(json *dst, FILE *stream)
{
	int		res; //almacenará el número entero leído del archivo.
	int		retorno; //guardará el valor de retorno de fscanf, que indica si la lectura fue exitosa o falló.

    dst->type = INTEGER;
	dst->integer = -42;
	retorno = fscanf(stream, "%d", &res); //intenta leer un número entero (%d) desde stream y almacenarlo en res.
	if (retorno <= 0 || retorno == EOF) //cualuier cosa diferente a 1
	{
		unexpected(stream); //echo -n "--\n"
		return (-1);
	}
	dst->integer = res;
	return (1);
}

char *parse_str(FILE *stream) //lee una cadena de caracteres entre comillas (") desde un stream 
{
	char *res = calloc(4096, sizeof(char));
	if (!res) 
		return NULL;
		
	int i = 0;
	char c = getc(stream); //Se lee el primer carácter del stream "
	
	//printf("char 1: %c\n", c); //borrar

	while (1) //bucle infinito que leerá caracteres hasta encontrar una comilla (") o un error.
	{
		c = getc(stream);
		//printf("char 2: %c\n", c); //borrar
		if (c == '"') //Si encontramos " (cierre de la cadena), salimos del bucle.
			break ;
		if (c == '\\') //Si c es \, significa que el siguiente carácter debe interpretarse literalmente.
			c = getc(stream); //para leer el carácter después de la barra invertida (\).
			
		if (c == EOF) //significa que la cadena se cortó inesperadamente.
		{
			free(res);
			unexpected(stream);
			return NULL;
		}
		
		res[i++] = c; //Se almacena c en la posición i de res. y luego i incrementa
	}
	return (res);
}

int parse_map(json *dst, FILE *stream) //{ "clave1": valor1, "clave2": valor2, ... }
{
	dst->type = MAP;
	dst->map.size = 0; //el mapa tiene 0 elementos 
	dst->map.data = NULL; //NULL, ya que aún no hay pares clave-valor.
	
	char c = getc(stream); //se esta usando????  '{'

	// if (peek(stream) == '}') //mapa vacío {}
	// 	return 1;
	
	if (peek(stream) == '}') //mapa vacío {}
		return 1;

	while (1)
	{
		c = peek(stream); //neceisto que sea peek porque necesito que no lo consuma, o despues el parser string no va a
		if (c != '"') //Si en la clave no hay comillas, el formato es incorrecto → error (-1).
			return -1;
			
		dst->map.data = realloc(dst->map.data, (dst->map.size + 1) * sizeof(pair)); //??
		pair *current = &dst->map.data[dst->map.size]; //??
		current->key = parse_str(stream); //extrae el string que esta entre ""
		if (current->key == NULL)
			return -1;
			
		dst->map.size++; //Aumentar el tamaño del mapa
		
		//despues de {"clave1"
		
		if (expect(stream, ':') == 0) //Si no hay :, el formato es incorrecto → error (-1).
			return -1;
			
		if (argo(&current->value, stream) == -1) //si hay :, parsea el valor. si argo falla (-1) → error (-1).
			return -1;
			

		c = peek(stream); //despues de {"clave1": valor1
		
		if (c == '}') //cierre del mapa
		{
			accept(stream ,c);
			break ;
		}
		if (c == ',') //viene otro par clave-valor.
			accept(stream, ',');
		else
		{
			unexpected(stream);
			return -1;
		}
	}
	return 1;
}

int argo(json *dst, FILE *stream)
{
	int c = peek(stream); //almacena en c el primer char

	if (c == '-' || isdigit(c))  //INTERGER
		return (parse_int(dst, stream));
		
    else if (c == '{') //MAP
		return (parse_map(dst, stream));
	
	else if (c == '"') //STRING
	{
		dst->type = STRING;
		dst->string = parse_str(stream);
		if (dst->string == NULL)
			return (-1);
		return (1);
	}
	else
	{
		unexpected(stream);
		return -1;
	}
	return (1);//nunca llega aca xq antes cae en el else
}

//main

void	free_json(json j)
{
	switch (j.type)
	{
		case MAP:
			for (size_t i = 0; i < j.map.size; i++)
			{
				free(j.map.data[i].key);
				free_json(j.map.data[i].value);
			}
			free(j.map.data);
			break ;
		case STRING:
			free(j.string);
			break ;
		default:
			break ;
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
		return 1;
	}
	
	//close(stream);
	serialize(file);
	free_json(file);
	printf("\n");
}