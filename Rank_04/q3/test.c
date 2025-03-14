#include "argo.h"


int		peek(FILE *stream);
void	unexpected(FILE *stream);
int		accept(FILE *stream, char c);
int		expect(FILE *stream, char c);
int		argo(json *dst, FILE *stream);
json	parse_json(FILE *stream);
json	parse_string_json(FILE *stream);
json	parse_map(FILE *stream);
json	parse_number(FILE *stream);
char	*parse_string(FILE *stream);
void	debug_print_cur_char(FILE *stream, char const *msg);
void	set_g_error(void);

int		g_error = 0;
int		g_error_no_key = 0;


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

int	argo(json *dst, FILE *stream)
{
	if (!stream)
		return (-1);

	*dst = parse_json(stream);
	if (g_error_no_key)
	{
		return (-1);
	}
	if (g_error)
	{
		unexpected(stream);
		return (-1);
	}
	return (1);
}

json	parse_json(FILE *stream)
{
	json	nothing;

	if (peek(stream) == '"')
		return (parse_string_json(stream));
	if (peek(stream) == '{')
		return (parse_map(stream));
	if (peek(stream) == '-' || isdigit(peek(stream)))
		return (parse_number(stream));
	nothing.type = STRING;
	nothing.string = NULL;
	g_error = 1;
	return (nothing);
}

json	parse_map(FILE *stream)
{
	json	json_map;
			
	json_map.type = MAP;
	json_map.map.data = NULL;
	json_map.map.size = 0;
	if (!accept(stream, '{'))
	{
		set_g_error();
		return (json_map);
	}
	if (peek(stream) != EOF && peek(stream) != EOF)
	{
		do
		{
            pair new_pair;
			if (peek(stream) != '"')
			{
				g_error_no_key = 1;
				return (json_map);
			}
			new_pair.key = parse_string(stream);
			if (g_error)
			{
				printf("HEREH\n");
				free(new_pair.key);
				return (json_map);
			}
			if (!accept(stream, ':'))
			{
				set_g_error();
				return (json_map);
			}
			new_pair.value = parse_json(stream);
			if (g_error)
			{
				free(new_pair.key);
				free_json(new_pair.value);
				return (json_map);
			}
			json_map.map.size++;
			json_map.map.data = realloc(json_map.map.data, json_map.map.size
					* sizeof(pair));
			json_map.map.data[json_map.map.size - 1] = new_pair;
		} while (accept(stream, ','));
	}
	if (g_error || !accept(stream, '}'))
	{
		set_g_error();
		return (json_map);
	}
	return (json_map);
}

json	parse_string_json(FILE *stream)
{
	json	json_string;

	json_string.type = STRING;
	json_string.string = parse_string(stream);
	return (json_string);
}

char	*parse_string(FILE *stream)
{
	char	*res;
	char	cur_char;
	int		str_len;

	res = malloc(sizeof(char));
	res[0] = '\0';
	str_len = 0;
	if (!accept(stream, '"'))
	{
		set_g_error();
		return (res);
	}
	while (peek(stream) != '"' && peek(stream) != EOF && !g_error)
	{
		//debug_print_cur_char(stream, "PARSING STRING ");
		str_len++;
		res = realloc(res, (str_len + 1) * sizeof(char));
		res[str_len] = '\0';
		cur_char = getc(stream);
		if (cur_char == '\\')
		{
			if (peek(stream) != '\\' && peek(stream) != '\"')
			{
				set_g_error();
				return (res);
			}
			cur_char = getc(stream);
		}
		res[str_len - 1] = cur_char;
	}
	if (!accept(stream, '"'))
	{
		set_g_error();
	}
	return (res);
}

json	parse_number(FILE *stream)
{
	json	num_json;
	int		res;
	int		ret_fscanf;

	num_json.type = INTEGER;
	num_json.integer = -42;
	ret_fscanf = fscanf(stream, "%d", &res);
	if (ret_fscanf <= 0 || ret_fscanf == EOF)
	{
		set_g_error();
		return (num_json);
	}
	num_json.integer = res;
	return (num_json);
}

void	set_g_error(void)
{
	//printf("g_error_set\n");
	g_error = 1;
}

void	debug_print_cur_char(FILE *stream, char const *msg)
{
	if (msg)
	{
		printf("%s", msg);
	}
	printf("CUR_CHAR: %c\n", peek(stream));
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
	serialize(file);
	free_json(file);
	printf("\n");
}