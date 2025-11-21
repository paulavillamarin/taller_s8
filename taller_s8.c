#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_PROD 5
#define MAX_NOM 30

void limpiar_buffer() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF) {}
}

void limpiar_nombre(char *nombre) {
    int len = strlen(nombre);
    int i = len - 1;

    while (i >= 0 && (nombre[i] == '\n' || nombre[i] == '\r')) {
        nombre[i] = '\0';
        i--;
    }

    i = strlen(nombre) - 1;
    while (i >= 0 && nombre[i] == ' ') {
        nombre[i] = '\0';
        i--;
    }
}

int iguales(const char *a, const char *b) {
    return strcmp(a, b) == 0;
}

void pedir_nombre(char *dest) {
    if (fgets(dest, MAX_NOM, stdin) == NULL) {
        dest[0] = '\0'; 
        return;
    }
    limpiar_nombre(dest);
}

int buscar_producto(char nombres[][MAX_NOM], int n, const char *buscado) {
    for(int i = 0; i < n; i++) {
        if(iguales(nombres[i], buscado)) return i; 
    }
    return -1;
}


void ingresar_productos(char nombres[][MAX_NOM], int *demanda, float *tiempo, float *recurso, int *n_prod) {
    int n, i;

    printf("\nCuantos productos desea ingresar (max %d): ", MAX_PROD);
    if (scanf("%d", &n) != 1) {
        limpiar_buffer();
        printf("Error de entrada. Cancelando.\n");
        return;
    }
    limpiar_buffer(); 

    if (n < 1) n = 1;
    if (n > MAX_PROD) n = MAX_PROD;
    
    if (*n_prod + n > MAX_PROD) {
        n = MAX_PROD - *n_prod;
        if (n <= 0) {
            printf("El inventario está lleno. (Máximo %d productos)\n", MAX_PROD);
            return;
        }
        printf("Solo se pueden agregar %d productos más.\n", n);
    }
    
    int inicio = *n_prod;

    for(i = inicio; i < inicio + n; i++) {
        printf("\n--- Producto %d ---\n", i + 1);
        printf("Nombre prod %d: ", i + 1);
        pedir_nombre(nombres[i]);
        
        printf("Demanda (unidades): ");
        if (scanf("%d", &demanda[i]) != 1) { limpiar_buffer(); demanda[i] = 0; }
        
        printf("Tiempo por unidad (horas): ");
        if (scanf("%f", &tiempo[i]) != 1) { limpiar_buffer(); tiempo[i] = 0.0f; }
        
        printf("Recursos por unidad: ");
        if (scanf("%f", &recurso[i]) != 1) { limpiar_buffer(); recurso[i] = 0.0f; }

        limpiar_buffer(); 
    }

    *n_prod = inicio + n;
}

void editar_producto(char nombres[][MAX_NOM], int *demanda, float *tiempo, float *recurso, int n_prod) {
    if(n_prod == 0) {
        printf("\nNo hay productos!\n"); return;
    }
    
    char buscado[MAX_NOM];
    printf("\nNombre producto a editar: ");
    pedir_nombre(buscado); 

    int pos = buscar_producto(nombres, n_prod, buscado);
    if(pos == -1) { 
        printf("Producto '%s' no existe.\n", buscado); return; 
    }
    
    printf("Editando '%s':\n", nombres[pos]);
    
    printf("Nuevo nombre (dejar vacío para no cambiar): ");
    char nuevo[MAX_NOM];
    pedir_nombre(nuevo); 
    if(nuevo[0] != '\0') {
        strncpy(nombres[pos], nuevo, MAX_NOM - 1);
        nombres[pos][MAX_NOM - 1] = '\0';
    }
    
    printf("Nueva demanda (-1 igual): ");
    int nd; 
    if (scanf("%d", &nd) == 1 && nd >= 0) demanda[pos] = nd;
    
    printf("Nuevo tiempo por unidad (-1 igual): ");
    float nt; 
    if (scanf("%f", &nt) == 1 && nt >= 0) tiempo[pos] = nt;
    
    printf("Nuevo recurso por unidad (-1 igual): ");
    float nr; 
    if (scanf("%f", &nr) == 1 && nr >= 0) recurso[pos] = nr;
    
    limpiar_buffer(); 

    printf("Producto editado.\n");
}

void eliminar_producto(char nombres[][MAX_NOM], int *demanda, float *tiempo, float *recurso, int *n_prod) {
    if(*n_prod == 0) { printf("\nNo existen productos.\n"); return; }
    
    char buscado[MAX_NOM];
    printf("\nNombre producto a eliminar: ");
    pedir_nombre(buscado);
    
    int pos = buscar_producto(nombres, *n_prod, buscado);
    if(pos == -1) { 
        printf("Producto '%s' no existe.\n", buscado); return;
    }
    
    for(int i = pos; i < (*n_prod) - 1; i++) {
        strncpy(nombres[i], nombres[i+1], MAX_NOM);
        nombres[i][MAX_NOM - 1] = '\0';
        
        demanda[i] = demanda[i+1];
        tiempo[i] = tiempo[i+1];
        recurso[i] = recurso[i+1];
    }
    
    (*n_prod)--;
    printf("Eliminado.\n");
}

void mostrar_productos(char nombres[][MAX_NOM], int *demanda, float *tiempo, float *recurso, int n_prod) {
    if(n_prod == 0) { 
        printf("\nSin productos.\n"); 
        return;
    }
    
    printf("\n--- LISTA DE PRODUCTOS (%d en total) ---\n", n_prod);
    
    for(int i = 0; i < n_prod; i++) {
        printf(" [%d] Producto: %s\n", i + 1, nombres[i]);
        printf("     Demanda: %d unidades\n", demanda[i]);
        printf("     Tiempo por unidad: %.2f horas\n", tiempo[i]);
        printf("     Recurso por unidad: %.2f\n", recurso[i]);
    }
    
    printf("------------------------------------------\n");
}

void calcular_produccion(char nombres[][MAX_NOM], int *demanda, float *tiempo, float *recurso, int n_prod) {
    if(n_prod == 0) { printf("\nNada para calcular.\n"); return;}
    
    float sumaT = 0, sumaR = 0;
    for(int i = 0; i < n_prod; i++) {
        sumaT += (float)demanda[i] * tiempo[i];
        sumaR += (float)demanda[i] * recurso[i];
    }
    
    printf("\n--- Resumen de Requerimientos ---\n");
    printf("Tiempo total necesitado: %.2f horas\n", sumaT);
    printf("Recursos totales requeridos: %.2f\n", sumaR);
    
    float dispoT, dispoR;
    printf("\nTiempo disponible (horas): "); 
    if (scanf("%f", &dispoT) != 1) dispoT = 0.0f;
    
    printf("Recursos disponibles: "); 
    if (scanf("%f", &dispoR) != 1) dispoR = 0.0f;
    
    limpiar_buffer(); 
    
    printf("\n--- Análisis de Capacidad ---\n");
    if(sumaT <= dispoT && sumaR <= dispoR)
        printf("CONCLUSION: Se puede cumplir TODA la demanda con los recursos disponibles.\n");
    else {
        printf("CONCLUSION: NO se puede cumplir la demanda.\n");
        if(sumaT > dispoT) printf("-> Falta tiempo: %.2f horas\n", sumaT - dispoT);
        if(sumaR > dispoR) printf("-> Faltan recursos: %.2f unidades\n", sumaR - dispoR);
    }
}


int main() {
    char nombres[MAX_PROD][MAX_NOM];
    int demanda[MAX_PROD];
    float tiempo[MAX_PROD], recurso[MAX_PROD];
    int n_prod = 0, op;

    printf("--- GESTOR DE PRODUCCIÓN V1.0 ---\n");

    do {
        printf("\n--- MENU ---\n");
        printf("1) Ingresar productos\n2) Editar producto\n3) Eliminar producto\n4) Mostrar productos\n5) Calcular produccion\n6) Salir\nOpcion: ");
        
        if (scanf("%d", &op) != 1) {
            op = 0; 
        }
        limpiar_buffer(); 

        switch(op) {
            case 1: ingresar_productos(nombres, demanda, tiempo, recurso, &n_prod); break;
            case 2: editar_producto(nombres, demanda, tiempo, recurso, n_prod); break;
            case 3: eliminar_producto(nombres, demanda, tiempo, recurso, &n_prod); break;
            case 4: mostrar_productos(nombres, demanda, tiempo, recurso, n_prod); break;
            case 5: calcular_produccion(nombres, demanda, tiempo, recurso, n_prod); break;
            case 6: printf("Saliendo del programa...\n"); break;
            default: printf("Opcion inválida. Por favor, ingrese un número del 1 al 6.\n");
        }
    } while(op != 6);

    return 0;
}