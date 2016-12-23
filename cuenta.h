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
 * Estas 2 funciones ya no se utilizan
 * Aprovecho la cuenta hecha en generaref
 * Dejo el código por no borrar los algoritmos
 * Fecha 2016-10-31
 */


int cuentahuellas (char ficheroblo[MAXCHAR])
{
FILE *blo;
int numhuellas=0; //número de módulos por bloque
char ch;
if ((blo=fopen(ficheroblo, "r"))==NULL)
	{
	printf("\nNo se puede abrir el fichero %s\n",ficheroblo);
	exit(0);
	}
while((ch=getc(blo))!=EOF)
	if(ch=='\n')
		numhuellas++;
fclose(blo);
return(numhuellas);
} /*cuentahuellas() */

int cuentabloques(char ficheroblo[MAXCHAR])
{
FILE *blo;
int numbloques=0; //número de bloques
char linea[LONG];
char ch;
int i=0;
if ((blo=fopen(ficheroblo, "r"))==NULL)
	{
	printf("\nNo se puede abrir el fichero %s\n",ficheroblo);
	exit(0);
	}
fgets(linea,LONG,blo);
while((ch=linea[i++])!='\n')
	if(isspace(ch))					//Obliga a poner \t después de cada referencia de la última columna
		numbloques++;
return(numbloques);
} /*cuentabloques*/
	