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
Programa para colocar las referencias en bloques gemelos a partir del primero en pcbnew.
Ficheros de origen: el del propio layout, proyecto.kicad_pcb y fichero de bloques con posiciones de referencias
*/

#include "gemelos.h"
#include "cuenta.h"
#include "lee.h"
#include "extrae.h"
#include "cambia.h"

/*EL FICHERO DE ENTRADA ES EL FICHERO GENERADO POR generaref.c CON LAS REFERENCIAS DE TODOS LOS BLOQUES.
 SE HA HECHO EN UN PROGRAMA APARTE PARA QUE SIRVA CON ESQUEMAS SIN SUB-HOJAS*/
void main(int argc, char *argv[])
{
// argv[1] fichero original .kicad_pcb
// argv[2] fichero con las coordenadas de posicionamiento de los módulos
// argv[3] distancia x respecto del bloque anterior donde coloco el siguiente bloque
// argv[4] distancia y respecto del bloque anterior donde coloco el siguiente bloque
FILE *pcb;		//fichero proyecto.kicad_pcb que se renombra como .kicad_pcb.old
FILE *nuevo;		//fichero generado por generaref.c
char ficherosalida[MAXCHAR]="Salida.kicad_pcb";	//fichero de salida que tomará el nombre original del fichero de entrada
char ficherosalida2[MAXCHAR];			//fichero de comprobación
FILE *nuevo2;	
char new[MAXCHAR];
char old[MAXCHAR];				//nuevo nombre del fichero de entrada ,renombrado como .kicad_pcb.old
char linea[LONG];
char modulo[LONG];
char interna[LONG][LONG];
char referencia[LONG]="";
int encontrada=0;
int i=0,j=0;
int k,l;
int numhuellas=0; 	//número de módulos por bloque
int numbloques=0;
int huellastotales=0;
char ch;


FILE *entrada;	//fichero de entrada argv[2] para leer el número de bloque y módulos directamente




if (argc < 3)
	{
	printf("\n\tSe necesitan 2 ficheros de entrada en línea de ejecución\n");
	printf("\tFichero1: fichero original .kicad_pcb\n");
	printf("\tFichero2: fichero con las referencias ordenadas por bloques: ficheroSCH-Referencias.csv\n\n");
	exit(0);
	}
	
	
if ((entrada=fopen(argv[2], "r"))==NULL)
	{
	printf("\nNo se puede abrir el fichero %s\n",argv[2]);
	exit(0);
	}
	
/*	Ya no se usan estas dos funciones
numhuellas=cuentahuellas(argv[2]);		//Número de huellas de un bloque
numbloques=cuentabloques(argv[2]);		//Número de bloques
*/

extraenombre(argv[2],ficherosalida2);
strcat(ficherosalida2,"-Relativas.csv");

fscanf(entrada,"%d", &numbloques);
fscanf(entrada,"%d", &numhuellas);
huellastotales=numhuellas*numbloques;		//Número total de huellas
fclose(entrada);




char listaref[huellastotales][COL][LONG];	//Array de las coordenadas relativas de posicionado de las referencias

for (i=0; i<huellastotales ; i++)	//inicializo listaref por si acaso
	for(l=1; l<COL ; l++)
		{
		strcpy(listaref[i][l],"0");
		}

//Copio todas las columnas en una sola, por bloques
leereferencias(argv[2],numhuellas,huellastotales,listaref);

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

/*Meto en el array listaref las referencias de todos los bloques y las posiciones relativas del primer bloque*/
while (fgets(linea,LONG,pcb) != NULL)
	{
	strcpy(modulo, linea);
	modulo[VEINTIDOS]='\0';
	if (!strcmp(modulo,"    (fp_text reference"))
	    {
	    extraeref(linea,referencia);
	    extraeposxposy(numhuellas,huellastotales,referencia,linea,listaref);
	    }
	}
	/*copio las posiciones relativas del primer bloque a los demás bloques*/
	for (j=1 ; j<numbloques ; j++)
		for(i=0, k=numhuellas*j ; i<numhuellas ; i++, k++)
			for(l=1; l<COL ; l++)
				strcpy(listaref[k][l],listaref[i][l]);

fseek(pcb,0,0);				//vuelvo al comienzo del fichero de entrada

/*Sustituyo cada línea con referencia por las nuevas referencias, dejando el resto sin tocar*/
while (fgets(linea,LONG,pcb) != NULL)
	{
	strcpy(modulo, linea);
	modulo[VEINTIDOS]='\0';
	if (!strcmp(modulo,"    (fp_text reference"))
	    {
	    extraeref(linea,referencia);
	    cambiaposxposy(huellastotales,referencia,linea,listaref);
	    }
	fputs(linea,nuevo);
	}
fclose(pcb);
fclose(nuevo);	


/*comprobación de que se forma el array.*/
if ((nuevo2=fopen(ficherosalida2, "w"))==NULL)
	{
	printf("\nNo se puede abrir el fichero %s\n",ficherosalida);
	exit(0);
	}
for (i=0 ; i<huellastotales ; i++)
  {
  for (j=0; j<COL ; j++)
	{
    fputs(listaref[i][j],nuevo2);
	fputs("\t",nuevo2);
	}
  fputs("\n",nuevo2);
  }
fclose(nuevo2);

renombrafichero(argv[1],ficherosalida);
printf("\nEl fichero de posiciones relativas de referencias es %s\n",ficherosalida2);

printf("\nPrograma finalizado\n\n");

}


