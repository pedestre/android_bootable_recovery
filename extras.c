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
static char* headers[] = {  "Apolo Extras Menu",
							"-----------------",
					NULL
					};
char* list[] = {  "Language",
				  "Backup/Restore efs",
				  "Clean Screen (clean logs)",
				  "Toggle double Touch (to confirm)",
				  "DEVS: Toggle logcat",
				  "Remove Apolo Data",
				  "Toggle MDNIE Sharpness",
				  "Toggle CRT-OFF",	
						   NULL };
	
    
     for (;;)
    {
		int cont;
		int salir;
		if (idiom==0){
			headers[0] = "Apolo Extras Menu";
			list[0]="Language";
			list[1]="Backup/Restore efs";
			list[2]="Clean Screen (clean logs)";
			list[3]="Toggle double Touch (to confirm)";
			list[4]="DEVS: Toggle logcat";
			list[5]="Remove Apolo Data";
			list[6]="Toggle MDNIE Sharpness";
			list[7]="Toggle CRT-OFF";
			list[8]=NULL;
			
		}else{
			headers[0] = "Menu Extras Apolo";
			
			list[0] = "Idioma";
			list[1] = "Backup/Restaurar efs";
			list[2] = "Limpiar Pantalla";
			list[3] = "Activar/Desactivar Doble Toque para Aceptar";
			list[4] = "DEVS: Habilitar/Deshabilitar logcat";
			list[5] = "Borrar datos de inicio Apolo";
			list[6] = "Activar/Desactivar MDNIE Sharpness";
			list[7] = "Activar/Desactivar efecto CRT-OFF";	
			list[8] = NULL;
		}
        int chosen_item = get_filtered_menu_selection(headers, list, 0, 0, sizeof(list) / sizeof(char*));
		if (chosen_item == GO_BACK)
			break;
		switch (chosen_item)
        {
		case 0:
            change_lenguage();
			salir=1;
			break;
	    case 1:
            show_efs_menu();
			break;
	    case 2:
		
			for (cont= 0; cont < 32; cont++) {
				ui_print("\n");
			}
			break;
	    case 3:
			Change_Touch_disable();
			break;
	    case 4:
			Habilitar_Logs();
			break;
	    case 5:
			BorrarDatosApolo();
			break;
	    case 6:
			show_mdnie_menu();
			break;
	    case 7:
			show_crt_menu();
			break;
           }
     }

}

void change_lenguage() {
static char* headers[] = {  "Language",
                            "",
                            NULL
    };

    char* list[] = { "English",
                     "Spanish",
                     NULL
    };
    if (idiom==0){
	headers[0]="Language";
	list[0]="English";
	list[1]="Spanish";
    }else{
	headers[0]="Idioma";
	list[0]="English";
	list[1]="Spanish";
	}
    int chosen_item = get_menu_selection(headers, list, 0, 0);
    ensure_path_mounted("/data");
    FILE * file = fopen("/data/.spanish","r");
    switch (chosen_item) {
		
        case 0:
	       idiom=0; //0 English
		   if (file)
		   {	
			//ui_print( "1\n" );
			fclose(file);
			ensure_path_mounted("/data");
			//ui_print( "2\n" );
			if( remove("/data/.spanish") != 0 ) 
				{
				ui_print( "Error Removing File\n" );
				}
			else
				{
				//ui_print( "3\n" );
				ui_print( "Lenguage set to English\n" );
				}
		    }
		    else{
				//ui_print( "4\n" );
				ui_print( "Lenguage set to English.\n" );
			}
				
                break;
		case 1:
			   idiom=1; //1 Spanish
			   if (file){
					fclose(file);
					ui_print( "Idioma fijado a castellano\n" );
					}
				else{
					FILE *file2 = fopen("/data/.spanish", "w");
					fprintf(file2, "%s", "1");
					fclose(file2);
				}
                break;
	}
}
void show_efs_menu() {
static char* headers[] = {  "Backup/Restore /efs",
					"",
					NULL
};

char* list[] = { "Backup /efs to internal sd",
		 "Restore  /efs from internal sd",
		 "Backup /efs to external sd",
		 "Restore  /efs from external sd",
		 NULL
};
			
	if (idiom==0){
		headers[0] = "Backup/Restore /efs";

		list[0] = "Backup /efs to internal sd";
        list[1] = "Restore  /efs from internal sd";
		list[2] = "Backup /efs to external sd";
        list[3] = "Restore  /efs from external sd";
        list[4] = NULL;		
	}
	else{
		headers[0] = "Gestion de /efs";

		list[0] = "Copia Seguridad de /efs a la interna";
        list[1] = "Restaurar  /efs a la interna";
		list[2] = "Copia Seguridad de /efs a la externa";
        list[3] = "Restaurar  /efs a la externa";
        list[4] = NULL;
		
		}
    

    int chosen_item = get_menu_selection(headers, list, 0, 0);
    switch (chosen_item) {
        case 0:
                ensure_path_mounted("/emmc");
                ensure_path_unmounted("/efs");
                __system("backup-efs.sh /emmc");
                ui_print("/emmc/clockworkmod/.efsbackup\n");
				if (idiom==0){
					ui_print("efs.img created\n");
				}
				else{
					ui_print("efs.img creado\n");
				}
		
                break;
        case 1:
                ensure_path_mounted("/emmc");
                ensure_path_unmounted("/efs");
                if( access("/emmc/clockworkmod/.efsbackup/efs.img", F_OK ) != -1 ) {
                   __system("restore-efs.sh /emmc");
                   ui_print("/emmc/clockworkmod/.efsbackup\n");
				   
				    if (idiom==0){
						ui_print("efs.img restored to /efs\n");
					}
					else{
						ui_print("efs.img restaurada a /efs\n");
					}
                } else {
				    if (idiom==0){
						ui_print("efs.img not found \n");
					}
					else{
						ui_print("No se encuentra la copia efs.img.\n");
					}
                   
                }
                break;
        case 2:
                ensure_path_mounted("/sdcard");
                ensure_path_unmounted("/efs");
                __system("backup-efs.sh /sdcard");
                ui_print("/sdcard/clockworkmod/.efsbackup\n");
				if (idiom==0){
					ui_print("efs.img created\n");
				}
				else{
					ui_print("efs.img creado\n");
				}
                break;
        case 3:
                ensure_path_mounted("/sdcard");
                ensure_path_unmounted("/efs");
                if( access("/sdcard/clockworkmod/.efsbackup/efs.img", F_OK ) != -1 ) {
                   __system("restore-efs.sh /sdcard");
                   ui_print("/sdcard/clockworkmod/.efsbackup\n");
                   if (idiom==0){
						ui_print("efs.img restored to /efs\n");
					}
					else{
						ui_print("efs.img restaurada a /efs\n");
					}
                } else {
                    if (idiom==0){
						ui_print("efs.img not found \n");
					}
					else{
						ui_print("No se encuentra la copia efs.img.\n");
					}
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
		if (idiom==0){
			ui_print( "Error Removing File\n" );
		}
		else{
			ui_print( "Error Borrando Fichero\n" );
		}
	    
	}
	else {
	    //ui_print( "\n" );
		if (idiom==0){
			ui_print( "Diable touch to confirm in Menu.\n" );
		}
		else{
			ui_print( "Toque Deshabilitado para Aceptar en Menus.\n" );
		}
	    
        }
    }
    else {
	FILE *file2 = fopen("/data/.disable_touchselect", "w");
    	fprintf(file2, "%s", "1");
    	fclose(file2);
	//ui_print( "\n" );
		if (idiom==0){
			ui_print( "Enable double touch to confirm in Menu.\n" );
		}
		else{
			ui_print( "Toque habilitado para Aceptar en Menus.\n" );
		}
	
    }

}

void Habilitar_Logs() {

	ensure_path_mounted("/data");

    FILE * file = fopen("/data/.enable_logs","r");
    if (file){
        fclose(file);
		if( remove( "/data/.enable_logs" ) != 0 ) {
			if (idiom==0){
				ui_print( "Error Removing File\n" );
			}
			else{
				ui_print( "Error Borrando Fichero\n" );
			}
		}	
		else {
				if (idiom==0){
					ui_print( "Disable Logs\n" );
				}
				else{
					ui_print( "Logcat Deshabilitado.\n" );
				}
			}
    }
    else {
	FILE *file2 = fopen("/data/.enable_logs", "w");
    	fprintf(file2, "%s", "1");
    	fclose(file2);
		if (idiom==0){
			ui_print( "Enable Logs\n" );
		}
		else{
			ui_print( "Logcat Habilitado.\n" );
		}
    }

}

void BorrarDatosApolo() {
	char *directorio;
	directorio="/system/Apolo";
	ensure_path_mounted("/system");
	//ui_print("system montado\n");
	dirUnlinkHierarchy(directorio);
	//Borrando opciones del configurador
	if( remove( "/system/etc/init.d/S999ApoloConf" ) != 0 ) {
	    if (idiom==0){
			ui_print( "Error Removing File\n" );
		}
		else{
			ui_print( "Error Borrando Fichero\n" );
		}
	}
	else {
	    
		if (idiom==0){
			ui_print( "Apolo files removed\n" );
		}
		else{
			ui_print( "Borrados Ficheros de inicio\n" );
		}
	}
	
}

void show_mdnie_menu() {
	static char* headers[] = {  "MDNIE Sharpness",
                                "",
                                NULL
			};
	char* list[] = { "ENABLE mdnie Sharpness",
                     "DISABLE mdnie sharpness",
					 NULL
			};  
	if (idiom==0){
		list[0] = "ENABLE mdnie Sharpness";
        list[1] = "DISABLE mdnie sharpness";
		list[2] = NULL;    
		}
	else{
		list[0] = "ACTIVAR mdnie Sharpness";
        list[1] = "DESACTIVAR mdnie sharpness";
		list[2] = NULL;   
		}
    
    int chosen_item = get_menu_selection(headers, list, 0, 0);
    ensure_path_mounted("/data");
    FILE * file = fopen("/data/.disable_mdnie","r");
    switch (chosen_item) {
        case 0:
	    if (file){
		fclose(file);
		if( remove( "/data/.disable_mdnie" ) != 0 ) {
		    if (idiom==0){
				ui_print( "Error Removing File\n" );
			}
			else{
				ui_print( "Error Borrando Fichero\n" );
			}
		}
		else {
			if (idiom==0){
				ui_print( "mdnie sharpness Enable\n" );
			}
			else{
				ui_print( "mdnie sharpness habilitado\n" );
			}
		    
		}
	    }
	    else{
			if (idiom==0){
				ui_print( "mdnie sharpness Enable\n" );
			}
			else{
				ui_print( "mdnie sharpness habilitado\n" );
			}
	    }
	    break;
	case 1:
	    if (file){
			if (idiom==0){
				ui_print( "mdnie sharpness Disable\n" );
			}
			else{
				ui_print( "mdnie sharpness Deshabilitado\n" );
			}
	    }
	    else {
		FILE *file2 = fopen("/data/.disable_mdnie", "w");
	    	fprintf(file2, "%s", "1");
	    	fclose(file2);
		//ui_print( "\n" );
			if (idiom==0){
				ui_print( "mdnie sharpness Disable\n" );
			}
			else{
				ui_print( "mdnie sharpness Deshabilitado\n" );
			}
	    }
	    break;
	}
}

void show_crt_menu() {
	    static char* headers[] = {  "CRT-OFF Effect",
                                "",
                                NULL
		};

		char* list[] = { "ENABLE CRT-OFF",
                     "DISABLE CRT-OFF",
                     NULL
		};
	if (idiom==0){
		headers[0] = "CRT-OFF Effect";

		list[0] = "ENABLE CRT-OFF";
		list[1] = "DISABLE CRT-OFF";
	}
	else{
		headers[0] = "Efecto CRT-OFF";

		list[0] = "ACTIVAR CRT-OFF";
		list[1] = "DESACTIVAR CRT-OFF";
	}

    int chosen_item = get_menu_selection(headers, list, 0, 0);
    ensure_path_mounted("/data");
    FILE * file = fopen("/data/.enable_crt","r");
    switch (chosen_item) {
        case 0:    
	    if (file){
		fclose(file);
		if( remove( "/data/.enable_crt" ) != 0 ) {
		    if (idiom==0){
				ui_print( "Error Removing File\n" );
			}
			else{
				ui_print( "Error Borrando Fichero\n" );
			}
		}
		else {
		FILE *file2 = fopen("/data/.enable_crt", "w");
	    	fprintf(file2, "%s", "50");
	    	fclose(file2);
			if (idiom==0){
				ui_print( "crt-off enable\n" );
			}
			else{
				ui_print( "Efecto crt-off habilitado\n" );
			}
		
	    	}
	    }
	    else {
		FILE *file2 = fopen("/data/.enable_crt", "w");
	    	fprintf(file2, "%s", "50");
	    	fclose(file2);
			if (idiom==0){
				ui_print( "crt-off enable\n" );
			}
			else{
				ui_print( "Efecto crt-off habilitado\n" );
			}
	    }
            break;
	case 1:
	    if (file){
		fclose(file);
		if( remove( "/data/.enable_crt" ) != 0 ) {
		    if (idiom==0){
				ui_print( "Error Removing File\n" );
			}
			else{
				ui_print( "Error Borrando Fichero\n" );
			}
		}
		else {
		    FILE *file2 = fopen("/data/.enable_crt", "w");
	    	    fprintf(file2, "%s", "0");
	    	    fclose(file2);
				if (idiom==0){
					ui_print( "crt-off disable\n" );
				}
				else{
					ui_print( "Efecto crt-off deshabilitado\n" );
				}
		    
			}
	    }
	    else {
		FILE *file2 = fopen("/data/.enable_crt", "w");
	    	fprintf(file2, "%s", "0");
	    	fclose(file2);
			if (idiom==0){
				ui_print( "crt-off disable\n" );
			}
			else{
				ui_print( "Efecto crt-off deshabilitado\n" );
			}
	    }
	    break;
	   
}
}


 
