
/*****************************************************************************************************************
	UNIVERSIDAD NACIONAL AUTONOMA DE MEXICO
	FACULTAD DE ESTUDIOS SUPERIORES -ARAGON-

	Computadoras y programacion.
	(c) Ponga su nombre y numero de cuenta aqui.

	Quiso decir: Programa principal de la aplicacion de la distancia de Levenstein.

******************************************************************************************************************/


#include "stdafx.h"
#include <string.h>
#include<ctype.h>
#include "corrector.h"
#define ESPACIO       ' '
#define TABULADOR     '\t'
#define SALTOLINEA    '\n'
#define CARRO         '\r'
#define COMA          ','
#define PUNTO         '.'
#define PUNTOCOMA     ';'
#define PARENTESISIZQ '('
#define PARENTESISDER ')'
//Funciones publicas del proyecto
void ordenarAlfabeticamente(char szPalabras[][TAMTOKEN], int iEstadisticas[], int n)
{
	char buffer[TAMTOKEN];
	int frecuencia;

	for (int i = 0; i < n - 1; i++)
	{
		for (int j = 0; j < n - i - 1; j++)
		{
			if (strcmp(szPalabras[j], szPalabras[j + 1]) > 0)
			{
				strcpy_s(buffer, TAMTOKEN, szPalabras[j]);
				strcpy_s(szPalabras[j], TAMTOKEN, szPalabras[j + 1]);
				strcpy_s(szPalabras[j + 1], TAMTOKEN, buffer);

				frecuencia = iEstadisticas[j];
				iEstadisticas[j] = iEstadisticas[j + 1];
				iEstadisticas[j + 1] = frecuencia;
			}
		}
	}
}

void ordenarSugeridas(char szPalabrasSugeridas[][TAMTOKEN], int n)
{
	char buffer[TAMTOKEN];

	for (int i = 0; i < n - 1; i++)
	{
		for (int j = 0; j < n - i - 1; j++)
		{
			if (strcmp(szPalabrasSugeridas[j], szPalabrasSugeridas[j + 1]) > 0)
			{
				strcpy_s(buffer, TAMTOKEN, szPalabrasSugeridas[j]);
				strcpy_s(szPalabrasSugeridas[j], TAMTOKEN, szPalabrasSugeridas[j + 1]);
				strcpy_s(szPalabrasSugeridas[j + 1], TAMTOKEN, buffer);
			}
		}
	}
}

/*****************************************************************************************************************
	DICCIONARIO: Esta funcion crea el diccionario completo
	char *	szNombre				:	Nombre del archivo de donde se sacaran las palabras del diccionario
	char	szPalabras[][TAMTOKEN]	:	Arreglo con las palabras completas del diccionario
	int		iEstadisticas[]			:	Arreglo con el numero de veces que aparecen las palabras en el diccionario
	int &	iNumElementos			:	Numero de elementos en el diccionario
*****************************************************************************************************************/

void Diccionario(char* szNombre, char szPalabras[][TAMTOKEN], int iEstadisticas[], int& iNumElementos)
{
	
	char palabra[NUMPALABRAS];
	char palabrota[NUMPALABRAS][TAMTOKEN];
	char otraPalabra[TAMTOKEN];
	int encontrada;
	int conta;
	int conta2;
	FILE* fp;

	fopen_s(&fp, szNombre, "r");
	if (fp == NULL)
	{
		printf("Error al abrir archivo: %s\n", szNombre);
		return;
	}

	iNumElementos = 0;

	while (fgets(palabra, NUMPALABRAS, fp))
	{
		conta = 0;
		conta2 = 0;
		int aux = 0;
		while (palabra[conta] != '\0')
		{
			char c = palabra[conta];

			if (c == ESPACIO || c == TABULADOR || c == SALTOLINEA || c == CARRO || c == COMA || c == PUNTOCOMA || c == PARENTESISIZQ || c == PARENTESISDER || c == PUNTO)
			{
				if (aux > 0)
				{
					otraPalabra[aux] = '\0';
					strcpy_s(palabrota[conta2++], TAMTOKEN, otraPalabra);
					aux = 0;
				}
			}
			else
			{
				if (aux < TAMTOKEN - 1)
					otraPalabra[aux++] = tolower((unsigned char)c);
			}
			conta++;
		}

		if (aux > 0)
		{
			otraPalabra[aux] = '\0';
			strcpy_s(palabrota[conta2++], TAMTOKEN, otraPalabra);
		}
		for (int k = 0; k < conta2; k++)
		{
			encontrada = false;

			for (int i = 0; i < iNumElementos; i++)
			{
				if (strcmp(szPalabras[i], palabrota[k]) == 0)
				{
					iEstadisticas[i]++;
					encontrada = true;
					break;
				}
			}
			if (!encontrada)
			{
				strcpy_s(szPalabras[iNumElementos], TAMTOKEN, palabrota[k]);
				iEstadisticas[iNumElementos] = 1;
				iNumElementos++;
			}
		}
	}
	ordenarAlfabeticamente(szPalabras, iEstadisticas, iNumElementos);
	fclose(fp);
	}
	
/*****************************************************************************************************************
	ListaCandidatas: Esta funcion recupera desde el diccionario las palabras validas y su peso
	Regresa las palabras ordenadas por su peso
	char	szPalabrasSugeridas[][TAMTOKEN],	//Lista de palabras clonadas
	int		iNumSugeridas,						//Lista de palabras clonadas
	char	szPalabras[][TAMTOKEN],				//Lista de palabras del diccionario
	int		iEstadisticas[],					//Lista de las frecuencias de las palabras
	int		iNumElementos,						//Numero de elementos en el diccionario
	char	szListaFinal[][TAMTOKEN],			//Lista final de palabras a sugerir
	int		iPeso[],							//Peso de las palabras en la lista final
	int &	iNumLista)							//Numero de elementos en la szListaFinal
******************************************************************************************************************/
void	ListaCandidatas(
	char	szPalabrasSugeridas[][TAMTOKEN],	//Lista de palabras clonadas
	int		iNumSugeridas,						//Lista de palabras clonadas
	char	szPalabras[][TAMTOKEN],				//Lista de palabras del diccionario
	int		iEstadisticas[],					//Lista de las frecuencias de las palabras
	int		iNumElementos,						//Numero de elementos en el diccionario
	char	szListaFinal[][TAMTOKEN],			//Lista final de palabras a sugerir
	int		iPeso[],							//Peso de las palabras en la lista final
	int& iNumLista)							//Numero de elementos en la szListaFinal
{
	iNumLista = 0;
	for (int i = 0; i < iNumSugeridas; i++)
	{
		int encontrada = false;

		for (int j = 0; j < iNumElementos; j++)
		{
			if (strcmp(szPalabrasSugeridas[i], szPalabras[j]) == 0)
			{
				strcpy_s(szListaFinal[iNumLista], TAMTOKEN, szPalabrasSugeridas[i]);
				iPeso[iNumLista] = iEstadisticas[j];
				iNumLista++;
				encontrada = true;
				break;
			}
		}
	}

	for (int i = 0; i < iNumLista - 1; i++)
	{
		for (int j = 0; j < iNumLista - i - 1; j++)
		{
			if (iPeso[j] < iPeso[j + 1])
			{
				int tempPeso = iPeso[j];
				iPeso[j] = iPeso[j + 1];
				iPeso[j + 1] = tempPeso;

				char tempPalabra[TAMTOKEN];
				strcpy_s(tempPalabra, TAMTOKEN, szListaFinal[j]);
				strcpy_s(szListaFinal[j], TAMTOKEN, szListaFinal[j + 1]);
				strcpy_s(szListaFinal[j + 1], TAMTOKEN, tempPalabra);
			}
		}
	}
}

/*****************************************************************************************************************
	ClonaPalabras: toma una palabra y obtiene todas las combinaciones y permutaciones requeridas por el metodo
	char *	szPalabraLeida,						// Palabra a clonar
	char	szPalabrasSugeridas[][TAMTOKEN], 	//Lista de palabras clonadas
	int &	iNumSugeridas)						//Numero de elementos en la lista
******************************************************************************************************************/
void ClonaPalabras(
	char* szPalabraLeida,
	char szPalabrasSugeridas[][TAMTOKEN],
	int& iNumSugeridas)
{
	char alfabeto[] = "abcdefghijklmn\361opqrstuvwxyz\341\351\355\363\372";
	char candidata[TAMTOKEN];
	int lenPalabra = strlen(szPalabraLeida);
	int lenAlfabeto = strlen(alfabeto);
	int i;
	iNumSugeridas = 0;

	strcpy_s(szPalabrasSugeridas[iNumSugeridas], TAMTOKEN, szPalabraLeida);
	iNumSugeridas++;

	//ELIMINACIONES
	for (i = 0; i < lenPalabra; i++)
	{
		int pos = 0;
		for (int j = 0; j < lenPalabra; j++)
			if (j != i)
				candidata[pos++] = szPalabraLeida[j];
		candidata[pos] = '\0';

		strcpy_s(szPalabrasSugeridas[iNumSugeridas], TAMTOKEN, candidata);
		iNumSugeridas++;
	}

	//TRANSPOSICIONES
	for (i = 0; i < lenPalabra - 1; i++)
	{
		strcpy_s(candidata, TAMTOKEN, szPalabraLeida);

		char temp = candidata[i];
		candidata[i] = candidata[i + 1];
		candidata[i + 1] = temp;

		strcpy_s(szPalabrasSugeridas[iNumSugeridas], TAMTOKEN, candidata);
		iNumSugeridas++;
	}

	//SUSTITUCIONES
	for (i = 0; i < lenPalabra; i++)
	{
		for (int a = 0; a < lenAlfabeto; a++)
		{
			strcpy_s(candidata, TAMTOKEN, szPalabraLeida);
			candidata[i] = alfabeto[a];

			strcpy_s(szPalabrasSugeridas[iNumSugeridas], TAMTOKEN, candidata);
			iNumSugeridas++;
		}
	}

	//INSERCIONES
	for (i = 0; i <= lenPalabra; i++)
	{
		for (int a = 0; a < lenAlfabeto; a++)
		{
			int pos = 0;

			for (int j = 0; j < i; j++)
				candidata[pos++] = szPalabraLeida[j];

			candidata[pos++] = alfabeto[a];

			for (int j = i; j < lenPalabra; j++)
				candidata[pos++] = szPalabraLeida[j];

			candidata[pos] = '\0';

			strcpy_s(szPalabrasSugeridas[iNumSugeridas], TAMTOKEN, candidata);
			iNumSugeridas++;
		}
	}
	ordenarSugeridas(szPalabrasSugeridas, iNumSugeridas);
}
//tengo hambre we

