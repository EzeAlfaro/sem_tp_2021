#include <stdio.h>      // libreria estandar
#include <stdlib.h>     // para usar exit y funciones de la libreria standard
#include <string.h>
#include <pthread.h>    // para usar threads
#include <semaphore.h>  // para usar semaforos
#include <unistd.h>


#define LIMITE 50




pthread_mutex_t sem_salero = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t sem_sarten = PTHREAD_MUTEX_INITIALIZER;
sem_t sem_horno;// para que acepte dos panes a la vez
int podio =0; // indica lugar ganador


//creo estructura de semaforos 
struct semaforos {
    sem_t sem_mezclar;
    sem_t sem_salar;
    sem_t sem_agregarCarne;
   sem_t sem_rebozar;
   sem_t sem_fritar;
   sem_t sem_milanesaLista;
   sem_t sem_panListo;
   sem_t sem_extrasListo;
	//poner demas semaforos aqui
};

//creo los pasos con los ingredientes
struct paso {
   char accion [LIMITE];
   char ingredientes[4][LIMITE];
   
};

//creo los parametros de los hilos 
struct parametro {
 int equipo_param;
 struct semaforos semaforos_param;
 struct paso pasos_param[9];
};


//funcion para imprimir las acciones y los ingredientes de la accion
void* imprimirAccion(void *data, char *accionIn) {
	struct parametro *mydata = data;
	//calculo la longitud del array de pasos 
	int sizeArray = (int)( sizeof(mydata->pasos_param) / sizeof(mydata->pasos_param[0]));
	//indice para recorrer array de pasos 
	int i;
	for(i = 0; i < sizeArray; i ++){
		//pregunto si la accion del array es igual a la pasada por parametro (si es igual la funcion strcmp devuelve cero)
		if(strcmp(mydata->pasos_param[i].accion, accionIn) == 0){
		printf("\tEquipo %d - accion %s \n " , mydata->equipo_param, mydata->pasos_param[i].accion);
		//calculo la longitud del array de ingredientes
		int sizeArrayIngredientes = (int)( sizeof(mydata->pasos_param[i].ingredientes) / sizeof(mydata->pasos_param[i].ingredientes[0]) );
		//indice para recorrer array de ingredientes
		int h;
		printf("\tEquipo %d -----------ingredientes : ----------\n",mydata->equipo_param); 
			for(h = 0; h < sizeArrayIngredientes; h++) {
				//consulto si la posicion tiene valor porque no se cuantos ingredientes tengo por accion 
				if(strlen(mydata->pasos_param[i].ingredientes[h]) != 0) {
							printf("\tEquipo %d ingrediente  %d : %s \n",mydata->equipo_param,h,mydata->pasos_param[i].ingredientes[h]);
				}
			}
		}
	}
}

//funcion para tomar de ejemplo
void* cortar(void *data) {
    	usleep( 20000);
	char *accion = "cortar";
	struct parametro *mydata = data;
	imprimirAccion(mydata,accion);
	usleep( 1000000 );
		FILE * salida = fopen("salida.txt", "a");
	fputs("Equipo ", salida); 
	fprintf(salida, "%d", mydata->equipo_param);
	fputs(" TERMINO DE CORTAL EL AJO Y EL PEREJIL! \n\n", salida);
	fclose(salida);
    sem_post(&mydata->semaforos_param.sem_mezclar);
    pthread_exit(NULL);
}


void* mezclarHuevo(void *data) {
	
	char *accion = "mezclar";
	struct parametro *mydata = data;
	sem_wait(&mydata->semaforos_param.sem_mezclar);
	imprimirAccion(mydata,accion);
	usleep( 1000000 );
		FILE * salida = fopen("salida.txt", "a");// escribe en el archivo salida.txt si existe, sino sexiste escribe al final del archivpp
	fputs("Equipo ", salida); //fputs permite escribir cadena de texto
	fprintf(salida, "%d", mydata->equipo_param);
	fputs(" TERMINO DE MEZCLAR EL HUEVO! \n\n", salida);
	fclose(salida);
    sem_post(&mydata->semaforos_param.sem_salar);
    pthread_exit(NULL);
}

void* salando(void *data) {

	char *accion = "salando";
	struct parametro *mydata = data;
	pthread_mutex_lock(&sem_salero);
	sem_wait(&mydata->semaforos_param.sem_salar);
	imprimirAccion(mydata,accion);
	usleep( 1000000 );
	pthread_mutex_unlock(&sem_salero);
    FILE * salida = fopen("salida.txt", "a");// escribe en el archivo salida.txt si existe, sino sexiste escribe al final del archivpp
	fputs("Equipo ", salida); //fputs permite escribir cadena de texto
	fprintf(salida, "%d", mydata->equipo_param);
	fputs(" TERMINO DE SALAR LA MEZCLA! \n\n", salida);
	fclose(salida);
	 printf("\tEQUIPO %d SALERO LIBRE!\n\n",mydata->equipo_param);
    sem_post(&mydata->semaforos_param.sem_agregarCarne);
    pthread_exit(NULL);
}

void* agregandoCarne(void *data) {
	char *accion = "agregando la carne";
	struct parametro *mydata = data;
	sem_wait(&mydata->semaforos_param.sem_agregarCarne);
	imprimirAccion(mydata,accion);
	usleep( 1000000 );
		FILE * salida = fopen("salida.txt", "a");// escribe en el archivo salida.txt si existe, sino sexiste escribe al final del archivpp
	fputs("Equipo ", salida); //fputs permite escribir cadena de texto
	fprintf(salida, "%d", mydata->equipo_param);
	fputs(" TERMINO DE AGREGAR LA CARNE! \n\n", salida);
	fclose(salida);
    sem_post(&mydata->semaforos_param.sem_rebozar);
    pthread_exit(NULL);
}

void* rebozar(void *data) {
	char *accion = "rebozando la carne";
	struct parametro *mydata = data;
	sem_wait(&mydata->semaforos_param.sem_rebozar);
	imprimirAccion(mydata,accion);
	usleep( 1000000 );
		FILE * salida = fopen("salida.txt", "a");// escribe en el archivo salida.txt si existe, sino sexiste escribe al final del archivpp
	fputs("Equipo ", salida); //fputs permite escribir cadena de texto
	fprintf(salida, "%d", mydata->equipo_param);
	fputs(" TERMINO DE REBOZAR LA CARNE! \n\n", salida);
	fclose(salida);
   sem_post(&mydata->semaforos_param.sem_fritar);
    pthread_exit(NULL);
}

void* fritarMila(void *data) {
	char *accion = "fritando";
	struct parametro *mydata = data;
	pthread_mutex_lock(&sem_sarten);
	sem_wait(&mydata->semaforos_param.sem_fritar);
	 printf("\tEQUIPO %d SARTEN UTILIZANDOSE!\n\n",mydata->equipo_param);
	imprimirAccion(mydata,accion);
	usleep( 5000000 );
		FILE * salida = fopen("salida.txt", "a");// escribe en el archivo salida.txt si existe, sino sexiste escribe al final del archivpp
	fputs("Equipo ", salida); //fputs permite escribir cadena de texto
	fprintf(salida, "%d", mydata->equipo_param);
	fputs(" TERMINO DE FRITAR LA MILANESA! \n\n", salida);
	fclose(salida);
	 printf("\tEQUIPO %d SARTEN LIBERADA!\n\n",mydata->equipo_param);
	pthread_mutex_unlock(&sem_sarten);
    sem_post(&mydata->semaforos_param.sem_milanesaLista);
    pthread_exit(NULL);
}
    
void* hornear(void *data) {
	usleep( 20000);
	char *accion = "horneando";
	struct parametro *mydata = data;
	sem_wait(&sem_horno);
	 printf("\tEQUIPO %d USANDO EL HORNO!\n\n",mydata->equipo_param);
	imprimirAccion(mydata,accion);
	usleep( 10000000);
		FILE * salida = fopen("salida.txt", "a");// escribe en el archivo salida.txt si existe, sino sexiste escribe al final del archivpp
	fputs("Equipo ", salida); //fputs permite escribir cadena de texto
	fprintf(salida, "%d", mydata->equipo_param);
	fputs(" TERMINO DE HORNEAR LOS PANES! \n\n", salida);
	fclose(salida);
	 printf("\tEQUIPO %d PANES HORNEADOS!\n\n",mydata->equipo_param);
   sem_post(&mydata->semaforos_param.sem_panListo);
   sem_post(&sem_horno);
    pthread_exit(NULL);

}

void* cortarExtras(void *data) {
	usleep( 20000);
	char *accion = "extras";
	struct parametro *mydata = data;
	imprimirAccion(mydata,accion);
	usleep( 10000000 );
		FILE * salida = fopen("salida.txt", "a");// escribe en el archivo salida.txt si existe, sino sexiste escribe al final del archivpp
	fputs("Equipo ", salida); //fputs permite escribir cadena de texto
	fprintf(salida, "%d", mydata->equipo_param);
	fputs(" TERMINO DE CORTAR LOS EXTRAS! \n\n", salida);
	fclose(salida);
   sem_post(&mydata->semaforos_param.sem_extrasListo);
	pthread_exit(NULL);

}

void* prepararSanguche(void *data) {
   
	//creo el nombre de la accion de la funcion
	char *accion = "Sanguche_listo";
	//creo el puntero para pasarle la referencia de memoria (data) del struct pasado por parametro (la cual es un puntero). 
	struct parametro *mydata = data;
	sem_wait(&mydata->semaforos_param.sem_milanesaLista);
    sem_wait(&mydata->semaforos_param.sem_panListo);
    sem_wait(&mydata->semaforos_param.sem_extrasListo);
    
	//llamo a la funcion imprimir le paso el struct y la accion de la funcion

	imprimirAccion(mydata,accion);
	//uso sleep para simular que que pasa tiempo
	usleep( 20000 );
	
	 if(podio==0){
	     	FILE * salida = fopen("salida.txt", "a");
	fputs("Equipo ", salida); 
	fprintf(salida, "%d", mydata->equipo_param);
	fputs(" - FELICIDADES! ES EL GANADOR DE LA COMPETENCIA ! \n\n", salida);
	fclose(salida);
        printf("\tEQUIPO %d GANADOR DE LA COMPETENCIA, 1ER LUGAR!\n\n",mydata->equipo_param);
     
        podio++; }
    else if (podio==1){
        printf("\tEQUIPO %d EN EL PODIO 2DO LUGAR!\n\n",mydata->equipo_param);
      
        podio++; }
    else if (podio==2){
        printf("\tEQUIPO %d EN EL PODIO 3ER LUGAR!\n\n",mydata->equipo_param);
      
        podio++; }
    else if (podio==3){
        printf("\tEQUIPO %d HA QUEDADO ULTIMO! 4TO LUGAR!\n\n",mydata->equipo_param);
      
        podio++; }
    
	
    pthread_exit(NULL);

}

void* ejecutarReceta(void *i) {
	
	//variables semaforos
	sem_t sem_mezclar;
	sem_t sem_salar;
	sem_t sem_agregarCarne;
	sem_t sem_rebozar;
	sem_t sem_fritar;
	//crear variables semaforos aqui
	
	//variables hilos
	pthread_t p1;
	pthread_t p2;
	pthread_t p3;
	pthread_t p4;
	pthread_t p5;
	pthread_t p6;
	pthread_t p7;
	pthread_t p8;
	pthread_t p9;
	//crear variables hilos aqui
	
	//numero del equipo (casteo el puntero a un int)
	int p = *((int *) i);
	
	printf("Ejecutando equipo %d \n\n", p);

	//reservo memoria para el struct
	struct parametro *pthread_data = malloc(sizeof(struct parametro));

	//seteo los valores al struct
	
	//seteo numero de grupo
	pthread_data->equipo_param = p;

	//seteo semaforos
	pthread_data->semaforos_param.sem_mezclar = sem_mezclar;
	pthread_data->semaforos_param.sem_salar = sem_salar;
	pthread_data->semaforos_param.sem_agregarCarne = sem_agregarCarne;
	pthread_data->semaforos_param.sem_rebozar = sem_rebozar;
	pthread_data->semaforos_param.sem_fritar = sem_fritar;
	//setear demas semaforos al struct aqui





FILE *archivo;
	archivo = fopen("receta.txt", "r");
	if (archivo == NULL)
		{
    	exit(EXIT_FAILURE); 
	}
	char * line = NULL;
	size_t len = 0;
	ssize_t read;
	int linea=0;
	while ((read = getline(&line, &len, archivo)) != -1) { 
	int init_size = strlen(line);
	int i = 0;
	char *ptr = strtok(line,":");
		while (ptr != NULL)
		{
			if(i ==0){
				strcpy(pthread_data->pasos_param[linea].accion, ptr);
				ptr = strtok(NULL, ",");
			}else{
				strcpy(pthread_data->pasos_param[linea].ingredientes[i], ptr);
				ptr = strtok(NULL, ",");
			}
			i++;
		}	
			linea++;
    }
    fclose(archivo);








	

	
/*
	//seteo las acciones y los ingredientes (Faltan acciones e ingredientes) ¿Se ve hardcodeado no? ¿Les parece bien?
     	strcpy(pthread_data->pasos_param[0].accion, "cortar");
	    strcpy(pthread_data->pasos_param[0].ingredientes[0], "ajo");
      strcpy(pthread_data->pasos_param[0].ingredientes[1], "perejil");


	    strcpy(pthread_data->pasos_param[1].accion, "mezclar");
	    strcpy(pthread_data->pasos_param[1].ingredientes[0], "ajo");
      strcpy(pthread_data->pasos_param[1].ingredientes[1], "perejil");
 	    strcpy(pthread_data->pasos_param[1].ingredientes[2], "huevo");
	    //strcpy(pthread_data->pasos_param[1].ingredientes[3], "carne");
	    
	    
	    strcpy(pthread_data->pasos_param[2].accion, "salando");
	    strcpy(pthread_data->pasos_param[2].ingredientes[0], "sal");
	    
	    strcpy(pthread_data->pasos_param[3].accion, "agregando la carne");
	    strcpy(pthread_data->pasos_param[3].ingredientes[0], "Carne de primera calidad");
	    
	      strcpy(pthread_data->pasos_param[4].accion, "rebozando la carne");
	    strcpy(pthread_data->pasos_param[4].ingredientes[0], "pan rallado");
	    
	     strcpy(pthread_data->pasos_param[5].accion, "fritando");
	    strcpy(pthread_data->pasos_param[5].ingredientes[0], "sarten");
	    
	    strcpy(pthread_data->pasos_param[6].accion, "horneando");
	    strcpy(pthread_data->pasos_param[6].ingredientes[0], "pancito");
	    
	    strcpy(pthread_data->pasos_param[7].accion, "extras");
	    strcpy(pthread_data->pasos_param[7].ingredientes[0], "lechuga, tomate y no se que");
	    
	    
	    strcpy(pthread_data->pasos_param[8].accion, "Sanguche_listo");
	    strcpy(pthread_data->pasos_param[8].ingredientes[0], "GANO LA COMPETENCIA");
	    
	*/    
	    
	    
	    
	
	
	//inicializo los semaforos

	sem_init(&(pthread_data->semaforos_param.sem_mezclar),0,0);
	sem_init(&(pthread_data->semaforos_param.sem_salar),0,0);
	sem_init(&(pthread_data->semaforos_param.sem_agregarCarne),0,0);
	sem_init(&(pthread_data->semaforos_param.sem_rebozar),0,0);
	sem_init(&(pthread_data->semaforos_param.sem_fritar),0,0);
	
	//inicializar demas semaforos aqui


	//creo los hilos a todos les paso el struct creado (el mismo a todos los hilos) ya que todos comparten los semaforos 
    int rc;
    rc = pthread_create(&p1,                           //identificador unico
                            NULL,                          //atributos del thread
                                cortar,             //funcion a ejecutar
                                pthread_data);                     //parametros de la funcion a ejecutar, pasado por referencia
    rc = pthread_create(&p2,                           //identificador unico
                            NULL,                          //atributos del thread
                                mezclarHuevo,             //funcion a ejecutar
                                pthread_data);                     //parametros de la funcion a ejecutar, pasado por referencia
    rc = pthread_create(&p3,                           //identificador unico
                            NULL,                          //atributos del thread
                                salando,             //funcion a ejecutar
                                pthread_data);                     //parametros de la funcion a ejecutar, pasado por referencia
    rc = pthread_create(&p4,                           //identificador unico
                            NULL,                          //atributos del thread
                                agregandoCarne,             //funcion a ejecutar
                                pthread_data);                     //parametros de la funcion a ejecutar, pasado por referencia
                                
      rc = pthread_create(&p5,                           //identificador unico
                            NULL,                          //atributos del thread
                                rebozar,             //funcion a ejecutar
                                pthread_data);                     //parametros de la funcion a ejecutar, pasado por referencia

 rc = pthread_create(&p6,                           //identificador unico
                            NULL,                          //atributos del thread
                                fritarMila,             //funcion a ejecutar
                                pthread_data);                     //parametros de la funcion a ejecutar, pasado por referencia
                                
 rc = pthread_create(&p7,                           //identificador unico
                            NULL,                          //atributos del thread
                                hornear,             //funcion a ejecutar
                                pthread_data);                     //parametros de la funcion a ejecutar, pasado por referencia
rc = pthread_create(&p8,                           //identificador unico
                            NULL,                          //atributos del thread
                                cortarExtras,             //funcion a ejecutar
                                pthread_data);                     //parametros de la funcion a ejecutar, pasado por referencia
                                
rc = pthread_create(&p9,                           //identificador unico
                            NULL,                          //atributos del thread
                                prepararSanguche,             //funcion a ejecutar
                                pthread_data);                     //parametros de la funcion a ejecutar, pasado por referencia



	
	//join de todos los hilos
	pthread_join (p1,NULL);
    pthread_join (p2,NULL);
    pthread_join (p3,NULL);
    pthread_join (p4,NULL);
    pthread_join (p5,NULL);
    pthread_join (p6,NULL);
    pthread_join (p7,NULL);
    pthread_join (p8,NULL);
    pthread_join (p9,NULL);
    
    
	//crear join de demas hilos


	//valido que el hilo se alla creado bien 
    if (rc){
       printf("Error:unable to create thread, %d \n", rc);
       exit(-1);
     }

	 
	//destruccion de los semaforos 
	sem_destroy(&sem_mezclar);
	sem_destroy(&sem_salar);
	sem_destroy(&sem_agregarCarne);
	sem_destroy(&sem_rebozar);
	sem_destroy(&sem_fritar);
	//destruir demas semaforos 
	
	//salida del hilo
	 pthread_exit(NULL);
}


int main ()
{

   
   
FILE * salida = fopen("salida.txt", "w");// para borrar datos

fclose(salida);
 

   
   
   
   
   //
    
    sem_init(&sem_horno,0,2);
	//creo los nombres de los equipos 
	int rc;
	int *equipoNombre1 =malloc(sizeof(*equipoNombre1));
	int *equipoNombre2 =malloc(sizeof(*equipoNombre2));
	int *equipoNombre3 =malloc(sizeof(*equipoNombre3));
	int *equipoNombre4 =malloc(sizeof(*equipoNombre4));
//faltan equipos
  
	*equipoNombre1 = 1;
	*equipoNombre2 = 2;
	*equipoNombre3 = 3;
	*equipoNombre4 = 4;

	//creo las variables los hilos de los equipos
	pthread_t equipo1; 
	pthread_t equipo2;
	pthread_t equipo3; 
	pthread_t equipo4;
//faltan hilos
  
	//inicializo los hilos de los equipos
    rc = pthread_create(&equipo1,                           //identificador unico
                            NULL,                          //atributos del thread
                                ejecutarReceta,             //funcion a ejecutar
                                equipoNombre1); 

    rc = pthread_create(&equipo2,                           //identificador unico
                            NULL,                          //atributos del thread
                                ejecutarReceta,             //funcion a ejecutar
                                equipoNombre2);

    rc = pthread_create(&equipo3,                           //identificador unico
                            NULL,                          //atributos del thread
                                ejecutarReceta,             //funcion a ejecutar
                                equipoNombre3); 

    rc = pthread_create(&equipo4,                           //identificador unico
                            NULL,                          //atributos del thread
                                ejecutarReceta,             //funcion a ejecutar
                                equipoNombre4);
  //faltn inicializaciones


   if (rc){
       printf("Error:unable to create thread, %d \n", rc);
       exit(-1);
     } 

	//join de todos los hilos
	pthread_join (equipo1,NULL);
	pthread_join (equipo2,NULL);
	pthread_join (equipo3,NULL);
	pthread_join (equipo4,NULL);
//.. faltan joins


    pthread_exit(NULL);
}


//Para compilar:   gcc subwayArgento.c -o ejecutable -lpthread
