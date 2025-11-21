#include <stdio.h>
#include <string.h>
#define MAX_PROD 5
#define MAX_NOM 30

// Comparación de strings
int iguales(const char *a, const char *b) {
    int i = 0;
    while (a[i] != '\0' && b[i] != '\0') {
        if (a[i] != b[i]) return 0;
        i++;
    }
    return a[i] == b[i];
}

// Buscador de producto
int buscar_producto(char nombres[][MAX_NOM], int n, const char *buscado) {
    for(int i = 0; i < n; i++) {
        if(iguales(nombres[i], buscado)) return i;
    }
    return -1;
}

void ingresar_productos(char nombres[][MAX_NOM], int *demanda, float *tiempo, float *recurso, int *n_prod) {
    int n, i;
    
    if (*n_prod >= MAX_PROD) {
        printf("\nYa se alcanzo el maximo de %d productos.\n", MAX_PROD);
        return;
    }
    
    printf("\nCuantos productos desea ingresar (max %d): ", MAX_PROD - *n_prod);
    scanf("%d", &n);
    
    if (n < 1) n = 1;
    if (n > MAX_PROD - *n_prod) n = MAX_PROD - *n_prod;
    
    for(i = 0; i < n; i++) {
        printf("\nProducto %d:\n", *n_prod + 1);
        
        if (*n_prod >= MAX_PROD) {
            printf("Se alcanzo el maximo de productos.\n");
            break;
        }
        
        printf("Nombre: ");
        scanf("%s", nombres[*n_prod]);
        
        if (buscar_producto(nombres, *n_prod, nombres[*n_prod]) != -1) {
            printf("Error! El producto '%s' ya existe.\n", nombres[*n_prod]);
            i--;
            continue;
        }
        
        printf("Demanda (unidades): ");
        scanf("%d", &demanda[*n_prod]);
        
        printf("Tiempo por unidad (horas): ");
        scanf("%f", &tiempo[*n_prod]);
        
        printf("Recursos por unidad: ");
        scanf("%f", &recurso[*n_prod]);
        
        (*n_prod)++;
    }
}

void editar_producto(char nombres[][MAX_NOM], int *demanda, float *tiempo, float *recurso, int n_prod) {
    if(n_prod == 0) {
        printf("\nNo hay productos\n");
        return;
    }
    
    printf("\nProductos existentes:\n");
    for(int i = 0; i < n_prod; i++) {
        printf("%d. %s\n", i+1, nombres[i]);
    }
    
    char buscado[MAX_NOM];
    printf("\nNombre del producto a editar: ");
    scanf("%s", buscado);
    
    int pos = buscar_producto(nombres, n_prod, buscado);
    if(pos == -1) {
        printf("Producto '%s' no existe.\n", buscado);
        return;
    }
    
    printf("\nEditando '%s':\n", nombres[pos]);
    
    printf("Nuevo nombre (actual: %s): ", nombres[pos]);
    char nuevo[MAX_NOM];
    scanf("%s", nuevo);
    
    if(nuevo[0] != '\0') {
        if (strcmp(nuevo, nombres[pos]) != 0 && buscar_producto(nombres, n_prod, nuevo) != -1) {
            printf("Error! El producto '%s' ya existe.\n", nuevo);
        } else {
            strcpy(nombres[pos], nuevo);
        }
    }
    
    printf("Nueva demanda (actual: %d): ", demanda[pos]);
    scanf("%d", &demanda[pos]);
    
    printf("Nuevo tiempo por unidad (actual: %.2f): ", tiempo[pos]);
    scanf("%f", &tiempo[pos]);
    
    printf("Nuevo recurso por unidad (actual: %.2f): ", recurso[pos]);
    scanf("%f", &recurso[pos]);
}

void eliminar_producto(char nombres[][MAX_NOM], int *demanda, float *tiempo, float *recurso, int *n_prod) {
    if(*n_prod == 0) {
        printf("\nNo existen productos.\n");
        return;
    }
    
    printf("\nProductos existentes:\n");
    for(int i = 0; i < *n_prod; i++) {
        printf("%d. %s\n", i+1, nombres[i]);
    }
    
    char buscado[MAX_NOM];
    printf("\nNombre del producto a eliminar: ");
    scanf("%s", buscado);
    
    int pos = buscar_producto(nombres, *n_prod, buscado);
    if(pos == -1) {
        printf("Producto '%s' no existe.\n", buscado);
        return;
    }
    
    printf("\nEsta seguro de eliminar '%s'? (s/n): ", nombres[pos]);
    char confirmar;
    scanf(" %c", &confirmar);
    
    if(confirmar == 's' || confirmar == 'S') {
        for(int i = pos; i < (*n_prod) - 1; i++) {
            strcpy(nombres[i], nombres[i+1]);
            demanda[i] = demanda[i+1];
            tiempo[i] = tiempo[i+1];
            recurso[i] = recurso[i+1];
        }
        (*n_prod)--;
        printf("Producto '%s' eliminado.\n", buscado);
    } else {
        printf("Eliminacion cancelada.\n");
    }
}

void mostrar_productos(char nombres[][MAX_NOM], int *demanda, float *tiempo, float *recurso, int n_prod) {
    if(n_prod == 0) {
        printf("\nNo hay productos registrados.\n");
        return;
    }
    
    printf("\nLista de Productos:\n");
    
    for(int i = 0; i < n_prod; i++) {
        printf("\nProducto %d:\n", i+1);
        printf("  Nombre: %s\n", nombres[i]);
        printf("  Demanda: %d unidades\n", demanda[i]);
        printf("  Tiempo por unidad: %.2f horas\n", tiempo[i]);
        printf("  Recursos por unidad: %.2f\n", recurso[i]);
    }
    
    float total_tiempo = 0, total_recursos = 0;
    for(int i = 0; i < n_prod; i++) {
        total_tiempo += demanda[i] * tiempo[i];
        total_recursos += demanda[i] * recurso[i];
    }
    
    printf("\nResumen total:\n");
    printf("  Tiempo total requerido: %.2f horas\n", total_tiempo);
    printf("  Recursos totales requeridos: %.2f\n", total_recursos);
}

void calcular_produccion(char nombres[][MAX_NOM], int *demanda, float *tiempo, float *recurso, int n_prod) {
    if(n_prod == 0) {
        printf("\nNo hay productos para calcular.\n");
        return;
    }
    
    float sumaT = 0, sumaR = 0;
    
    printf("\nCalculo de Produccion:\n");
    
    for(int i = 0; i < n_prod; i++) {
        float tiempo_total = demanda[i] * tiempo[i];
        float recurso_total = demanda[i] * recurso[i];
        sumaT += tiempo_total;
        sumaR += recurso_total;
        
        printf("\n%s:\n", nombres[i]);
        printf("  Demanda: %d unidades\n", demanda[i]);
        printf("  Tiempo total: %.2f horas\n", tiempo_total);
        printf("  Recursos totales: %.2f\n", recurso_total);
    }
    
    printf("\nTotales generales:\n");
    printf("  Tiempo total requerido: %.2f horas\n", sumaT);
    printf("  Recursos totales requeridos: %.2f\n", sumaR);
    
    float dispoT, dispoR;
    printf("\nVerificacion de Capacidad:\n");
    printf("Tiempo disponible (horas): ");
    scanf("%f", &dispoT);
    printf("Recursos disponibles: ");
    scanf("%f", &dispoR);
    
    printf("\nResultado:\n");
    if(sumaT <= dispoT && sumaR <= dispoR) {
        printf("SE PUEDE CUMPLIR con la demanda\n");
        if(sumaT < dispoT) printf("  Tiempo sobrante: %.2f horas\n", dispoT - sumaT);
        if(sumaR < dispoR) printf("  Recursos sobrantes: %.2f\n", dispoR - sumaR);
    } else {
        printf("NO SE PUEDE CUMPLIR con la demanda\n");
        if(sumaT > dispoT) 
            printf("  Falta tiempo: %.2f horas\n", sumaT - dispoT);
        if(sumaR > dispoR) 
            printf("  Faltan recursos: %.2f\n", sumaR - dispoR);
    }
}

int main() {
    char nombres[MAX_PROD][MAX_NOM];
    int demanda[MAX_PROD];
    float tiempo[MAX_PROD], recurso[MAX_PROD];
    int n_prod = 0, op;
    
    do {
        printf("\nMENU PRINCIPAL\n");
        printf("1. Ingresar productos\n");
        printf("2. Editar producto\n");
        printf("3. Eliminar producto\n");
        printf("4. Mostrar productos\n");
        printf("5. Calcular produccion\n");
        printf("6. Salir\n");
        printf("Opcion: ");
        scanf("%d", &op);
        
        switch(op) {
            case 1: 
                ingresar_productos(nombres, demanda, tiempo, recurso, &n_prod); 
                break;
            case 2: 
                editar_producto(nombres, demanda, tiempo, recurso, n_prod); 
                break;
            case 3: 
                eliminar_producto(nombres, demanda, tiempo, recurso, &n_prod); 
                break;
            case 4: 
                mostrar_productos(nombres, demanda, tiempo, recurso, n_prod); 
                break;
            case 5: 
                calcular_produccion(nombres, demanda, tiempo, recurso, n_prod); 
                break;
            case 6: 
                printf("Saliendo del sistema...\n"); 
                break;
            default: 
                printf("Opcion invalida. Intente nuevamente.\n");
        }
    } while(op != 6);
    
    return 0;
}
