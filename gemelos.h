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

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define LONG 100		//caracteres máximos que creo que puede tener una línea
#define COMIENZOCOORD 8		//posición del primer carácter de la coordenada x de la huella
#define COMIENZOREF 23		//posición del primer carácter de la referencia en el fichero.kicad_pcb
#define COMIENZOPOSX 28		//posicion del primer carácter de la coordenada x de la referencia
#define COMIENZOSCH 9		//posición del primer carácter de la referencia en el fichero.sch
#define NUEVE 9			//final de la palabra "module" en la línea que define el comienzo del módulo
#define VEINTIDOS 22		//final de la palabra "(fp_text reference" que define la línea de referencia
#define COL 4			//número de columnas del fichero de bloques: ref posx posy angle
#define MAXCHAR 256		//máximo número de caracteres que puede tener el nombre de un fichero en linux
#define CINCO 5			//caracteres máximos de las tiras up y down
