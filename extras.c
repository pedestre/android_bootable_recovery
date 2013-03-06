#include <ctype.h>
#include <errno.h>
#include <fcntl.h>
#include <getopt.h>
#include <limits.h>
#include <linux/input.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/reboot.h>
#include <sys/types.h>
#include <time.h>
#include <unistd.h>


#include <sys/wait.h>
#include <sys/limits.h>
#include <dirent.h>
#include <sys/stat.h>
#include <sys/statfs.h>

#include <signal.h>
#include <sys/wait.h>

#include "bootloader.h"
#include "common.h"
#include "cutils/properties.h"
#include "firmware.h"
#include "install.h"
#include "make_ext4fs.h"
#include "minui/minui.h"
#include "minzip/DirUtil.h"
#include "roots.h"
#include "recovery_ui.h"

#include "extendedcommands.h"
#include "nandroid.h"
#include "mounts.h"
#include "flashutils/flashutils.h"
#include "edify/expr.h"
#include <libgen.h>
#include "mtdutils/mtdutils.h"
#include "bmlutils/bmlutils.h"
#include "cutils/android_reboot.h"
#include "extras.h"
#include "recovery.h"

void show_extras_menu()
{
    static char* headers[] = {  "Menu Extras Apolo",
                                "-----------------",
				NULL
    };
    
    char* list[] = {  "Backup/Restaurar efs",
                      "Limpiar Pantalla",
		      "Activar/Desactivar Doble Toque para Aceptar",
		      "DEVS: Habilitar/Deshabilitar logcat",
		      "Borrar datos de inicio Apolo",
		      "Activar/Desactivar MDNIE Sharpness",
		      "Activar/Desactivar efecto CRT-OFF",	
                       NULL };
    
     for (;;)
    {
	int cont;
        int chosen_item = get_filtered_menu_selection(headers, list, 0, 0, sizeof(list) / sizeof(char*));
		if (chosen_item == GO_BACK)
			break;
		switch (chosen_item)
        {
	    case 0:
            show_efs_menu();
			break;
	    case 1:
		
			for (cont= 0; cont < 32; cont++) {
				ui_print("\n");
			}
			break;
	    case 2:
			Change_Touch_disable();
			break;
	    case 3:
			Habilitar_Logs();
			break;
	    case 4:
			BorrarDatosApolo();
			break;
	    case 5:
			show_mdnie_menu();
			break;
	    case 6:
			show_crt_menu();
			break;
           }
     }

}



void show_efs_menu() {
    static char* headers[] = {  "Gestion de /efs",
                                "",
                                NULL
    };

    char* list[] = { "Copia Seguridad de /efs a la interna",
                     "Restaurar  /efs a la interna",
 		     "Copia Seguridad de /efs a la externa",
                     "Restaurar  /efs a la externa",
                     NULL
    };

    int chosen_item = get_menu_selection(headers, list, 0, 0);
    switch (chosen_item) {
        case 0:
                ensure_path_mounted("/emmc");
                ensure_path_unmounted("/efs");
                __system("backup-efs.sh /emmc");
                ui_print("/emmc/clockworkmod/.efsbackup/");
		ui_print("efs.img creado\n");
                break;
        case 1:
                ensure_path_mounted("/emmc");
                ensure_path_unmounted("/efs");
                if( access("/emmc/clockworkmod/.efsbackup/efs.img", F_OK ) != -1 ) {
                   __system("restore-efs.sh /emmc");
                   ui_print("/emmc/clockworkmod/.efsbackup/");
		   ui_print("efs.img restaurada a /efs");
                } else {
                   ui_print("No se encuentra la copia efs.img.\n");
                }
                break;
        case 2:
                ensure_path_mounted("/sdcard");
                ensure_path_unmounted("/efs");
                __system("backup-efs.sh /sdcard");
                ui_print("/sdcard/clockworkmod/.efsbackup/");
	   	ui_print("efs.img creado\n");
                break;
        case 3:
                ensure_path_mounted("/sdcard");
                ensure_path_unmounted("/efs");
                if( access("/sdcard/clockworkmod/.efsbackup/efs.img", F_OK ) != -1 ) {
                   __system("restore-efs.sh /sdcard");
                   ui_print("/sdcard/clockworkmod/.efsbackup/");
                   ui_print("efs.img restaurada a /efs"); 
                } else {
                  ui_print("No se encuentra la copia efs.img.\n");
                }
                break;
    }
}

void Change_Touch_disable() {

    ensure_path_mounted("/data");

    FILE * file = fopen("/data/.disable_touchselect","r");
    if (file){
        fclose(file);
	if( remove( "/data/.disable_touchselect" ) != 0 ) {
	    ui_print( "Error Borrando Fichero" );
	}
	else {
	    //ui_print( "\n" );
	    ui_print( "Toque Deshabilitado para Aceptar en Menus.\n" );
        }
    }
    else {
	FILE *file2 = fopen("/data/.disable_touchselect", "w");
    	fprintf(file2, "%s", "1");
    	fclose(file2);
	//ui_print( "\n" );
	ui_print( "Toque habilitado para Aceptar en Menus.\n" );
    }

}

void Habilitar_Logs() {

	ensure_path_mounted("/data");

    FILE * file = fopen("/data/.enable_logs","r");
    if (file){
        fclose(file);
	if( remove( "/data/.enable_logs" ) != 0 ) {
	    ui_print( "Error Borrando Fichero" );
	}
	else {
	    ui_print( "Logcat Deshabilitado.\n" );
        }
    }
    else {
	FILE *file2 = fopen("/data/.enable_logs", "w");
    	fprintf(file2, "%s", "1");
    	fclose(file2);
		ui_print( "Logcat Habilitado.\n" );
    }

}

void BorrarDatosApolo() {
	char *directorio;
	directorio="/system/Apolo";
	ensure_path_mounted("/system");
	ui_print("system montado\n");
	dirUnlinkHierarchy(directorio);
	//Borrando opciones del configurador
	if( remove( "/system/etc/init.d/S999ApoloConf" ) != 0 ) {
	    ui_print( "Error Borrando Fichero\n" );
	}
	else {
	    ui_print( "Borrados Ficheros de inicio\n" );
	}
	
}

void show_mdnie_menu() {
    static char* headers[] = {  "MDNIE Sharpness",
                                "",
                                NULL
    };

    char* list[] = { "ACTIVAR mdnie Sharpness",
                     "DESACTIVAR mdnie sharpness",
                     NULL
    };

    int chosen_item = get_menu_selection(headers, list, 0, 0);
    ensure_path_mounted("/data");
    FILE * file = fopen("/data/.disable_mdnie","r");
    switch (chosen_item) {
        case 0:
	    if (file){
		fclose(file);
		if( remove( "/data/.disable_mdnie" ) != 0 ) {
		    ui_print( "Error Borrando Fichero" );
		}
		else {
		    ui_print( "mdnie sharpness habilitado\n" );
		}
	    }
	    else{
		ui_print( "mdnie sharpness habilitado\n" );
	    }
	    break;
	case 1:
	    if (file){
		ui_print( "mdnie sharpness deshabilitado\n" );
	    }
	    else {
		FILE *file2 = fopen("/data/.disable_mdnie", "w");
	    	fprintf(file2, "%s", "1");
	    	fclose(file2);
		//ui_print( "\n" );
		ui_print( "mdnie sharpness deshabilitado\n" );
	    }
	    break;
	}
}

void show_crt_menu() {
    static char* headers[] = {  "Efecto CRT-OFF",
                                "",
                                NULL
    };

    char* list[] = { "ACTIVAR CRT-OFF",
                     "DESACTIVAR CRT-OFF",
                     NULL
    };

    int chosen_item = get_menu_selection(headers, list, 0, 0);
    ensure_path_mounted("/data");
    FILE * file = fopen("/data/.enable_crt","r");
    switch (chosen_item) {
        case 0:    
	    if (file){
		ui_print( "Efecto crt-off habilitado\n" );
	    }
	    else {
		FILE *file2 = fopen("/data/.enable_crt", "w");
	    	fprintf(file2, "%s", "50");
	    	fclose(file2);
		ui_print( "Efecto crt-off habilitado\n" );
	    }
            break;
	case 1:
	    if (file){
		fclose(file);
		if( remove( "/data/.enable_crt" ) != 0 ) {
		    ui_print( "Error Borrando Fichero" );
		}
		else {
		    FILE *file2 = fopen("/data/.enable_crt", "w");
	    	    fprintf(file2, "%s", "0");
	    	    fclose(file2);
		    ui_print( "Efecto crt-off deshabilitado\n" );
		}
	    }
	    else {
		FILE *file2 = fopen("/data/.enable_crt", "w");
	    	fprintf(file2, "%s", "0");
	    	fclose(file2);
		ui_print( "Efecto crt-off deshabilitado\n" );
	    }
	    break;
	   
}
}


 
