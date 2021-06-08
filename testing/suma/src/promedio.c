/* Copyright 2016, Laboratorio de Microprocesadores 
 * Facultad de Ciencias Exactas y Tecnología 
 * Universidad Nacional de Tucuman
 * http://www.microprocesadores.unt.edu.ar/
 * Copyright 2016, Esteban Volentini <evolentini@gmail.com>
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice,
 *    this list of conditions and the following disclaimer.
 *
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 *    this list of conditions and the following disclaimer in the documentation
 *    and/or other materials provided with the distribution.
 *
 * 3. Neither the name of the copyright holder nor the names of its
 *    contributors may be used to endorse or promote products derived from this
 *    software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 */

/** \brief Breve descripción del archivo
 **
 ** Descripción completa del archivo
 **
 ** \addtogroup Modulo Nombre del módulo
 ** \brief Breve descripción del conjunto de archivos
 ** @{ */

/* === Inclusiones de cabeceras ================================================================ */
#include "promedio.h"
#include "suma.h"

/* === Definicion y Macros ===================================================================== */

/* === Declaraciones de tipos de datos internos ================================================ */

/* === Declaraciones de funciones internas ===================================================== */

/* === Definiciones de variables externas ====================================================== */

/* === Definiciones de funciones internas =====================================================- */

/* === Definiciones de funciones externas ====================================================== */

int promediar(const int valores[], int cantidad, int * promedio) {
   int resultado;
   int indice;
   int acumulado = 0;

   for (indice = 0; indice < cantidad; indice++) {
      resultado = acumular(&acumulado, valores[indice]);
      if (resultado != 0) break;
   }
   if (resultado == 0) {
      *promedio = (acumulado / cantidad);
   }
   return resultado;
}

/* === Ciere de documentacion ================================================================== */

/** @} Final de la definición del modulo para doxygen */
