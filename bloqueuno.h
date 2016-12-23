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

/*Genero el fichero csv de posicionado sin utilizar pcbnew. Las coordenadas del primer bloque están listas.
 Evito eliminar todos los módulos que no pertenecen a los bloques y tengo ya los bloques organizados*/

void generabloques(char ficheropcb[MAXCHAR],char ficherosch[MAXCHAR],int numhuellas,int numbloques, int huellastotales,char posiciones[huellastotales][COL][LONG], double distanciax, double distanciay)
{

FILE *pcb;			//fichero proyecto.kicad_pcb
FILE *nuevo;			//fichero generado
char ficherosalida[MAXCHAR];	
char linea[LONG];
char modulo[LONG];
char interna[LONG][LONG];
char referencia[LONG]="";
int encontrada=0;
int i=0, j=0, k=0;
char ch;
double intermedio;

extraenombre(ficherosch,ficherosalida);
strcat(ficherosalida,"-Bloques.csv");

if ((pcb=fopen(ficheropcb,"r"))==NULL)
	{
	printf("\nNo se puede abrir el fichero %s\n",ficheropcb);
	exit(0);
	}

while (fgets(linea,LONG,pcb) != NULL)
	{
	strcpy(modulo, linea);
	modulo[NUEVE]='\0';							//recorto la linea para encontrar la coincidencia "(module"
	i=0;
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
				extraecoordenadas(numhuellas,huellastotales,referencia,interna[0],posiciones);
				}
			i++;
			}
		}
	}
fclose(pcb);

/*Se crea el ficheropcbBloques.csv con las coordenadas del primer bloque y las referencias de todos los bloques*/
if ((nuevo=fopen(ficherosalida, "w"))==NULL)
	{
	printf("\nNo se puede abrir el fichero %s\n",ficherosalida);
	exit(0);
	}
for (i=0 ; i<numhuellas ; i++)				//primer bloque
	{
	for (j=0 ; j<COL ; j++)
		{
		fputs(posiciones[i][j],nuevo);
		fputs("\t",nuevo);
		}
	fputs("\n",nuevo);
	}

// Resto de bloques
// También podría hacer un array tridimensional, siendo la tercera dimensión el número de bloque
for (k=1 ; k<numbloques ; k++)
	{
	for (i=numhuellas*k; i<numhuellas*(k+1) ; i++)
		{
		for (j=0 ; j<COL ; j++)
			{
				switch(j)
					{
					case 0:
						fputs(posiciones[i][j],nuevo);
						fputs("\t",nuevo);
						break;
					case 1:
						if (distanciax != 0)
							{
							intermedio = atof(posiciones[i-(numhuellas*k)][j])+(distanciax * k);
							sprintf(posiciones[i][j],"%3.6f",intermedio);
							}
						else
							{
							strcpy(posiciones[i][j],posiciones[i-(numhuellas*k)][j]);
							}
						fputs(posiciones[i][j],nuevo);
						fputs("\t",nuevo);
						break;
					case 2:
						if (distanciay != 0)
							{
							intermedio = atof(posiciones[i-(numhuellas*k)][j])+(distanciay * k);
							sprintf(posiciones[i][j],"%3.6f",intermedio);
							}
						else
							{
							strcpy(posiciones[i][j],posiciones[i-(numhuellas*k)][j]);
							}
						fputs(posiciones[i][j],nuevo);
						fputs("\t",nuevo);
						break;
					case 3:
						strcpy(posiciones[i][j],posiciones[i-(numhuellas*k)][j]);
						fputs(posiciones[i][j],nuevo);
						fputs("\t",nuevo);
					}
			}
		fputs("\n",nuevo);
		}
	}



fclose(nuevo);

printf("\nEl fichero de posicionado de huellas es %s\n",ficherosalida);
}	/*generabloques() */


