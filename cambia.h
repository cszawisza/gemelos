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

/*Función que cambia lee las coordenadas de los módulos del primer bloque*/
//Referencias y coordenadas: ref posx posy angle en listaref[i][j]
void cambiaposxposy(int huellastotales,char referencia[LONG],char lineat[LONG],char listaref[huellastotales][COL][LONG])
{
int i,j;

for (i=0; i<huellastotales; i++)
	{
	if (!strcmp(referencia,listaref[i][0]))
		{
		strcpy(lineat,"    (fp_text reference ");
		strcat(lineat,listaref[i][0]);
		strcat(lineat," (at ");
		strcat(lineat,listaref[i][1]);
		strcat(lineat," ");
		strcat(lineat,listaref[i][2]);
		if (strcmp(listaref[i][3],"0.0"))
			{
			strcat(lineat," ");
			strcat(lineat,listaref[i][3]);
			}
		strcat(lineat,") (layer F.SilkS)\n");	
		}
	}
} /*cambiacoordenadas() */


/*Función que cambia las coordenadas del módulo por las nuevas que queremos poner*/
//Referencias y coordenadas: ref posx posy angle en posiciones[i][j]
void cambiacoordenadas(int numhuellas,char referencia[LONG],char lineat[LONG],char posiciones[numhuellas][COL][LONG])
{
int i,j;

for (i=0; i<numhuellas; i++)
	{
	if (!strcmp(referencia,posiciones[i][0]))
		{
		strcpy(lineat,"    (at ");
		strcat(lineat,posiciones[i][1]);
		strcat(lineat," ");
		strcat(lineat,posiciones[i][2]);
		if (strcmp(posiciones[i][3],"0.0"))
			{
			strcat(lineat," ");
			strcat(lineat,posiciones[i][3]);
			}
		strcat(lineat,")\n");
		}
	}
} /*cambiacoordenadas() */


/*Elimina la extensión del nombre de archivo*/
void quitaextension(char entrada[MAXCHAR], char salida[MAXCHAR])
{
int i=0, j=0;
char ch;

while((ch=entrada[i++])!='.')
	{
	salida[j++]=ch;
	salida[j]='\0';
	}
}


/*Elimina la extensión del nombre de archivo*/
void extraenombre(char entrada[MAXCHAR], char salida[MAXCHAR])
{
int i=0, j=0;
char ch;

while((ch=entrada[i++])!='-')
	{
	salida[j++]=ch;
	salida[j]='\0';
	}
}

renombrafichero(char entrada[MAXCHAR],char salida[MAXCHAR])
{
char nuevo[MAXCHAR];
char viejo[MAXCHAR];

strcpy(nuevo,entrada);
strcpy(viejo,entrada);
strcat(viejo,"Old");
if (rename(entrada,viejo)==0)
	printf("\nEl fichero de entrada ha sido renombrado como %s\n",viejo);
else
	printf("\nEl fichero de entrada %s no se ha podido renombrar\n",entrada);
if (rename(salida,nuevo)==0)
	printf("\nEl fichero de salida ha sido renombrado como %s\n",nuevo);
else
	printf("\nEl fichero de salida %s no se ha podido renombrar\n",salida);
}


/*ordena cada bloque de coordenadas gemelas (D1,D2,D4,D5,D3) según su número de bloque
 * que está en la línea AR path */
void ordenargemdown(char path[LONG][200][200],int i, int limitecol)
{
  int j=0;
  char temp[LONG];
  
  
int tope, busca;
for (tope=0 ; tope < limitecol ; tope++)
  for (busca= tope+1 ; busca < limitecol ; busca++)
    if (strcmp(path[i][tope],path[i][busca])>0)
    {
      strcpy(temp,path[i][tope]);
      strcpy(path[i][tope],path[i][busca]);
      strcpy(path[i][busca],temp);
    }
}
  
/*Lo mismo que ordenagem pero en sentido inverso */   
   void ordenargemup(char path[LONG][200][200],int i, int limitecol)
{
  int j=0;
  char temp[LONG];
  
  
int tope, busca;
for (tope=0 ; tope < limitecol ; tope++)
  for (busca= tope+1 ; busca < limitecol ; busca++)
    if (strcmp(path[i][tope],path[i][busca])<0)
    {
      strcpy(temp,path[i][tope]);
      strcpy(path[i][tope],path[i][busca]);
      strcpy(path[i][busca],temp);
    }
}
    
    
    
    
    
    
    
    
    
    
    
    
