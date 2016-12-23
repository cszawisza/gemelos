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
#include "extrae.h"
#include "cambia.h"

/*EL FICHERO DE ENTRADA SERA EL FICHERO .SCH DEL ESQUEMA PARA GENERAR EL FICHERO CON TODAS LAS REFERENCIAS.
 SE PUEDE HACER EN UN PROGRAMA APARTE PARA QUE SIRVA CON ESQUEMAS SIN SUB-HOJAS*/
void main(int argc, char *argv[])
{

  
FILE *prueba;
FILE *pcb;		
FILE *nuevo;					//fichero generado
char ficherosalida[MAXCHAR];			//fichero de salida: matriz de huellas. Cada columna un bloque.
char linea[LONG];
char modulo[LONG];
char referencia[LONG]="";
char parte[LONG]="";
char path[LONG][200][200];
int anterior=1;
int actual=0;
int i=0, j=0;					//contadores
int limitecol=0;				//limitecol es el número de bloques
int limitefil=0;				//limitefil es el número de módulos por bloque
char ch;
char updown[CINCO];

quitaextension(argv[1],ficherosalida);
strcat(ficherosalida,"-Referencias.csv");

if (argc < 3)
	{
	printf("\n\tSe necesitan el fichero de entrada en línea de ejecución\n");
	printf("\n\ty el sentido de la ordenación up o down\n\n");
	printf("\tFichero: fichero original .sch\n\n");
	exit(0);
	}

strcpy(updown,argv[2]);		//updown define el sentido de la ordenación

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

if ((prueba=fopen("pruebaslinea", "w"))==NULL)
	{
	printf("\nNo se puede abrir el fichero pruebaslinea\n");
	exit(0);
	}

	
/*
 *Extraigo del fichero .sch las líneas que contienen definiciones de las referencias
 */
while (fgets(linea,LONG,pcb) != NULL)
	{
	strcpy(modulo, linea);
	modulo[7]='\0';						//Chapuza para encontrar la cadena "AR Path"
	if (!strcmp(modulo,"AR Path"))
		{
		if (actual==0)
			anterior=1;
		extraeschparte(linea,referencia,parte);
		if (!strcmp(parte,"1"))
			{
			if (referencia[0] != '#')
				{
				strcpy(path[i][j++],linea);
				actual=1;
				limitecol=j;
				}
			}
		}
	else
		anterior=0;
	if (actual==1 && anterior==0)
		{
		actual=0;
		anterior=1;
	
		if(strcmp(updown,"up"))
			ordenargemup(path,i,limitecol);			// Ordeno los componentes por hoja jerárquica, pues a veces están desordenados
		else										// en el fichero .sch y se pueden mezclar los bloques
			ordenargemdown(path,i,limitecol);
		
		for (j=0;j<limitecol;j++)
		    fputs(path[i][j],prueba);
		j=0;
	        limitefil=++i;
		fputs("\n",prueba);
		}
	}


	
//Escribo el número de bloques y el número de modulos por bloque en el fichero de referencias
fprintf(nuevo, "%d\t",limitecol);
fprintf(nuevo, "%d\n",limitefil);

for (i=0 ; i<limitefil ; i++, fputs("\n",nuevo))
  for (j=0 ; j<limitecol ; j++)
  {
    extraesch(path[i][j],referencia);
    fputs(referencia,nuevo);
    fputs("\t",nuevo);
  }

fclose(prueba);
fclose(pcb);
fclose(nuevo);

printf("\n           El número de bloques es %d\n",limitecol);
printf("\nEl número de módulos por bloque es %d\n",limitefil);

printf("\nEl fichero de referencias es %s\n",ficherosalida);
printf("\nPrograma finalizado\n\n");

}


