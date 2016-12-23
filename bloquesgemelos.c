/* 
 * Copyright (C) 2016 Pedro Martin "pkicad" <lucobos@cgac.es>
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, you may find one here:
 * http://www.gnu.org/licenses/old-licenses/gpl-2.0.html
 * or you may search the http://www.gnu.org website for the version 2 license,
 * or you may write to the Free Software Foundation, Inc.,
 * 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA
 */

/*
Programa para colocar un bloque gemelo a otro a una distancia concreta en pcbnew.
Ficheros de origen: el del propio layout, proyecto.kicad_pcb y fichero bloques
*/

#include "gemelos.h"
#include "cuenta.h"
#include "lee.h"
#include "extrae.h"
#include "cambia.h"
#include "bloqueuno.h"

void main(int argc, char *argv[])
{
// argv[1] fichero original .kicad_pcb
// argv[2] fichero con las coordenadas de posicionamiento de los módulos
FILE *pcb;		//fichero proyecto.kicad_pcb que se renombra como .kicad_pcbOld
FILE *nuevo;	//fichero generado
FILE *entrada;	//fichero con las referencias ordenadas por bloques: ficheroSCH-Referencias.csv
char ficherosalida[MAXCHAR]="Salida.kicad_pcb";	//fichero de salida que tomará el nombre original del fichero de entrada
char new[MAXCHAR];
char old[MAXCHAR];				//nuevo nombre del fichero de entrada ,renombrado como .kicad_pcbOld
char linea[LONG];
char modulo[LONG];
char interna[LONG][LONG];
char referencia[LONG]="";
int encontrada=0;
int i,j, l;			//contadores
int numhuellas=0; 	//número de módulos por bloque
int numbloques=0;
int huellastotales=0;
double distanciax=0; 
double distanciay=0;

if (argc < 5)
	{
	printf("\n\tSe necesitan 2 ficheros de entrada en línea de ejecución\n");
	printf("\tFichero1: fichero original .kicad_pcb\n");
	printf("\tFichero2: fichero con las referencias ordenadas por bloques: ficheroSCH-Referencias.csv\n");
    printf("\n\ty las distancias en mm entre huellas gemelas de cada bloque: distanciax y distanciay\n\n");
	exit(0);
	}


if ((entrada=fopen(argv[2], "r"))==NULL)
	{
	printf("\nNo se puede abrir el fichero %s\n",argv[2]);
	exit(0);
	}
	

fscanf(entrada,"%d", &numbloques);
fscanf(entrada,"%d", &numhuellas);
huellastotales=numhuellas*numbloques;		//Número total de huellas
fclose(entrada);

distanciax = atof(argv[3]);
distanciay = atof(argv[4]);

printf("\n\t distanciax: %f", distanciax);
printf("\n\t distanciay: %f\n\n", distanciay);


char listacoord[huellastotales][COL][LONG];	//Array de las coordenadas de posicionado de las huellas

for (i=0; i<huellastotales ; i++)	//inicializo listacoord por si acaso
	for(l=1; l<COL ; l++)
		{
		strcpy(listacoord[i][l],"0");
		}
		
//Copio todas las columnas en una sola, por bloques
leereferencias(argv[2],numhuellas,huellastotales,listacoord);


if ((pcb=fopen(argv[1],"r"))==NULL)
	{
	printf("\nNo se puede abrir el fichero %s\n",argv[1]);
	exit(0);
	}

if ((nuevo=fopen(ficherosalida, "w"))==NULL)
	{
	printf("\nNo se puede abrir el fichero %s\n",ficherosalida);
	exit(0);
	}


/*Genero el fichero csv de posiciones de los módulos*/
generabloques(argv[1],argv[2],numhuellas,numbloques,huellastotales,listacoord,distanciax,distanciay);

if ((pcb=fopen(argv[1],"r"))==NULL)
	{
	printf("\nNo se puede abrir el fichero %s\n",argv[1]);
	exit(0);
	}

if ((nuevo=fopen(ficherosalida, "w"))==NULL)
	{
	printf("\nNo se puede abrir el fichero %s\n",ficherosalida);
	exit(0);
	}

while (fgets(linea,LONG,pcb) != NULL)
	{
	strcpy(modulo, linea);
	modulo[NUEVE]='\0';							//recorto la linea para encontrar la coincidencia "(module"
	i=0;
	fputs(linea,nuevo);
	if (!strcmp("  (module",modulo))	
		{
		encontrada=0;
		while(encontrada==0)
			{
			fgets(interna[i],LONG,pcb);
			strcpy(modulo, interna[i]);
			modulo[VEINTIDOS]='\0';				//recorto la linea para encontrar la coincidencia 	"(fp_text reference"
			if (!strcmp(modulo,"    (fp_text reference"))
				{
				encontrada=1;
				extraeref(interna[i],referencia);	//busco la referencia del componente dentro de la linea
				cambiacoordenadas(huellastotales,referencia,interna[0],listacoord); //cambia la línea (at
				for(j=0;j<=i;j++)
					fputs(interna[j],nuevo);			//imprimo la lineas intermedias entre (at y (fp_text
				}
			i++;
			}
		}
	}
fclose(pcb);
fclose(nuevo);

renombrafichero(argv[1],ficherosalida);
printf("\nPrograma finalizado\n\n");

} //main()










