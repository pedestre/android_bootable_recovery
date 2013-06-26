/*
 * Copyright (C) 2009 The Android Open Source Project
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include <linux/input.h>

#include "recovery_ui.h"
#include "common.h"
#include "extendedcommands.h"
#include <stdio.h>

char* MENU_HEADERS[] = { NULL };

char* MENU_ITEMS[] = { "Reiniciar el sistema",
                       "Instalar zip desde las memorias ",
                       "Borrar datos (Wipe Data)",
					   "Borrar cache y Dalvik (Wipe Cache y Dalvik)",
                       "Copias de Seguridad y Restauracion",
                       "Montaje de Unidades y Almacenamiento Masivo",
					   "Instalar ROM en zip (LIMPIEZA INCLUIDA)",
                       "Opciones Avanzadas",
					   "Extras Apolo",
					   "Apagar",
                       NULL };

void load_initial_menu(){
	
    ensure_path_mounted("/data");

    FILE * file = fopen("/data/.spanish","r");
    if (file)
    {
	idiom = 1;
	fclose(file);
    }
    else {
	idiom = 0;
    }
	   
	if (idiom==0){
	   MENU_ITEMS[0] = "Reboot system";
	   MENU_ITEMS[1] = "Install zip";
	   MENU_ITEMS[2] = "Wipe Data";
	   MENU_ITEMS[3] = "Wipe Cache and Dalvik";
	   MENU_ITEMS[4] = "Backup and Restore";
	   MENU_ITEMS[5] = "Mounts and Mass Storage";
	   MENU_ITEMS[6] = "Install New ROM (CLEAN INCLUDED)";
	   MENU_ITEMS[7] = "Advanced";
	   MENU_ITEMS[8] = "Apolo Extras";
	   MENU_ITEMS[9] = "Power Off";
	   MENU_ITEMS[10] = NULL;
	}
	else{
	   MENU_ITEMS[0] = "Reiniciar el sistema";
	   MENU_ITEMS[1] = "Instalar zip desde las memorias ";
	   MENU_ITEMS[2] = "Borrar datos (Wipe Data)";
	   MENU_ITEMS[3] = "Borrar cache y Dalvik (Wipe Cache y Dalvik)";
	   MENU_ITEMS[4] = "Copias de Seguridad y Restauracion";
	   MENU_ITEMS[5] = "Montaje de Unidades y Almacenamiento Masivo";
	   MENU_ITEMS[6] = "Instalar ROM en zip (LIMPIEZA INCLUIDA)";
	   MENU_ITEMS[7] = "Opciones Avanzadas";
	   MENU_ITEMS[8] = "Apolo Extras";
	   MENU_ITEMS[9] = "Apagar";
	   MENU_ITEMS[10] = NULL;	
	}
}	   
void device_ui_init(UIParameters* ui_parameters) {
}

int device_recovery_start() {
    return 0;
}

int device_reboot_now(volatile char* key_pressed, int key_code) {
    return 0;
}

int device_perform_action(int which) {
    return which;
}

int device_wipe_data() {
    return 0;
}
