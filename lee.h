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

/*Función que carga en el array listaref las referencias del primer bloque*/
void leereferencias(char ficheroblo[MAXCHAR],int numhuellas,int huellastotales,char listaref[huellastotales][COL][LONG])
{
FILE *blo; 		//Fichero de referencias del primer bloque: ref  listarf[i][0]
int i,j,k;
int numbloques;
numbloques=huellastotales/numhuellas;
char listacomp[numhuellas][numbloques][LONG];

printf("\nHuellas %i   Bloques %i   Totales %i\n",numhuellas,numbloques,huellastotales);

if ((blo=fopen(ficheroblo,"r"))==NULL)
	{
	printf("\nNo se puede abrir el fichero %s\n",ficheroblo);
	exit(0);
	}

fscanf(blo,"%d", &numbloques);			//vuelvo a leer los valores para no hacer nada con ellos
fscanf(blo,"%d", &numhuellas);	


for(i=0; i<numhuellas ; i++) 		//Leo el fichero de referencias, cada columna es un bloque
	for (j=0; j<numbloques ; j++)
		{
		fscanf(blo,"%s",listacomp[i][j]);
		}	
									//se puede hacer en un único for, pero no queda código limpio
for (j=0,k=0; j<numbloques ; j++)	//Copio todas las columnas en una sola, por bloques
	for(i=0; i<numhuellas ; i++)
		{
		strcpy(listaref[k][0],listacomp[i][j]);
		strcpy(listaref[k++][3],"0");
		}
} /*leerReferencias() */


/*Función que carga en el array posiciones el fichero de nuevas coordenadas*/
void leebloques(char ficheroblo[MAXCHAR],int numhuellas,char posiciones[numhuellas][COL][LONG])
{
FILE *blo; 		//Fichero de referencias y coordenadas: ref posx posy angle en posiciones[i][j]
int i,j;

if ((blo=fopen(ficheroblo,"r"))==NULL)
	{
	printf("\nNo se puede abrir el fichero %s\n",ficheroblo);
	exit(0);
	}
for (i=0; i<numhuellas; i++)
	for (j=0;j<COL;j++)
		{
		fscanf(blo,"%s",posiciones[i][j]);
		}
fclose(blo);
} /*leerbloques() */