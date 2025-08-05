#include "client.h"
#include <readline/readline.h>

int main(void)
{
	/*---------------------------------------------------PARTE 2-------------------------------------------------------------*/

	int conexion;
	char* ip = NULL;
	char* puerto = NULL;
	char* valor = NULL;

	t_log* logger;
	t_config* config;

	/* ---------------- LOGGING ---------------- */

	logger = iniciar_logger();
	// Usando el logger creado previamente
	// Escribi: "Hola! Soy un log"


	logger = log_create("tp0.log", "LOGS TP0", 1, LOG_LEVEL_INFO);
	log_info(logger, "Hola! Soy un log", "INFO");
	log_destroy(logger);
	/* ---------------- ARCHIVOS DE CONFIGURACION ---------------- */

	config = iniciar_config();

	// Usando el config creado previamente, leemos los valores del config y los 
	// dejamos en las variables 'ip', 'puerto' y 'valor'
	
	
	config = config_create("cliente.config");

	if(config_has_property(config, "CLAVE"))
		valor = config_get_string_value(config, "CLAVE");

	if(config_has_property(config, "IP"))
		ip = config_get_string_value(config, "IP");

	if(config_has_property(config, "PUERTO"))
		puerto = config_get_string_value(config, "PUERTO");

	// Loggeamos el valor de config

 logger = log_create("tp0.log", "LOGS TP0", 1, LOG_LEVEL_INFO);
 log_info(logger, valor, "INFOR");
 log_info(logger, ip, "INFO");
 log_info(logger, puerto, "INFO");
	/* ---------------- LEER DE CONSOLA ---------------- */

	leer_consola(logger);

	/*---------------------------------------------------PARTE 3-------------------------------------------------------------*/

	// ADVERTENCIA: Antes de continuar, tenemos que asegurarnos que el servidor esté corriendo para poder conectarnos a él

	// Creamos una conexión hacia el servidor
	conexion = crear_conexion(ip, puerto);

	// Enviamos al servidor el valor de CLAVE como mensaje
	enviar_mensaje(valor, conexion);

	// Armamos y enviamos el paquete
	paquete(conexion);

	terminar_programa(conexion, logger, config);

	/*---------------------------------------------------PARTE 5-------------------------------------------------------------*/
	// Proximamente
}

t_log* iniciar_logger(void)
{
    t_log* nuevo_logger = log_create("tp0.log", "client", 1, LOG_LEVEL_INFO);
    if (nuevo_logger == NULL) {
        printf("No se pudo crear el logger\n");
        exit(1);
    }
    return nuevo_logger;
}

t_config* iniciar_config(void)
{
	t_config* nuevo_config = config_create("cliente.config");
	if(nuevo_config == NULL) {
		printf("No se pudo leer el archivo de configuración\n");
		exit(2);
	}
	return nuevo_config;
}

void leer_consola(t_log* logger)
{
	char* leido;

	// La primera te la dejo de yapa
	leido = readline("> ");

	// El resto, las vamos leyendo y logueando hasta recibir un string vacío
	while(strcmp(leido, "")){
		log_info(logger, "MENSAJE LEIDO", "INFO");
		leido = readline("> ");
	}

	// ¡No te olvides de liberar las lineas antes de regresar!
	free(leido);
}

void paquete(int conexion)
{
	// Ahora toca lo divertido!
	char* leido;
	t_paquete* paquete;

	// Leemos y esta vez agregamos las lineas al paquete
	paquete = crear_paquete();
	leido = readline("> ");
	while(strcmp(leido, "")){
		agregar_a_paquete(paquete, leido, strlen(leido)+1);
		leido = readline("> ");
	}
	enviar_paquete(paquete, conexion);
	// ¡No te olvides de liberar las líneas y el paquete antes de regresar!
	free(leido);
	eliminar_paquete(paquete);
}

void terminar_programa(int conexion, t_log* logger, t_config* config)
{
	/* Y por ultimo, hay que liberar lo que utilizamos (conexion, log y config) 
	  con las funciones de las commons y del TP mencionadas en el enunciado */
	liberar_conexion(conexion);
	log_destroy(logger);
	config_destroy(config);
}