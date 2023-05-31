#include<stdio.h>
#include<conio.h>
#include<stdlib.h>
#include<string.h>

#define continuar printf("Presione cualquier tecla para continuar\n");\
                  getch();\
                  system("cls");

typedef enum RET{
    RET_OK,
    RET_FAIL
} RET;

// Structs para los diferentes datos: producto, fechas y registro ventas
typedef struct 
{
    int idProducto;
    char nombreProducto[41];
    float precioProducto;
    int cantProducto;
} tproducto;

typedef struct 
{
    int dd;
    int mm;
    int aa;
} tfechaVenta;

typedef struct
{
    char nomProVendido[41];
    int cantidadVendida;
    float precioUnidad;
    float precioTotal;
}tprodVendido;

typedef struct 
{
    char nombreComprador[60];
    tprodVendido productosVendidos[100];
    int numActProd;
} tComprador;

typedef struct 
{
    int id_venta; 
    tfechaVenta fechaVenta;
    tComprador comprador;
    float totalVentas;
} tventas;

//Creación del producto en un archivo.txt
void productosCreados(){

    FILE *archivo;
    archivo = fopen("inventario.dat", "wb");
    if (archivo==NULL)
    {
        exit(1);
    }
    tproducto productos;
    productos.idProducto = 1;
    strcpy(productos.nombreProducto, "Carne");
    productos.precioProducto = 12000;
    productos.cantProducto=15;
    fwrite(&productos, sizeof(tproducto), 1, archivo);
    productos.idProducto = 2;
    strcpy(productos.nombreProducto, "Leche");
    productos.precioProducto = 6000;
    productos.cantProducto=20;
    fwrite(&productos, sizeof(tproducto), 1, archivo);
    productos.idProducto = 3;
    strcpy(productos.nombreProducto, "Arroz");
    productos.precioProducto = 5000;
    productos.cantProducto=15;
    fwrite(&productos, sizeof(tproducto), 1, archivo);
    fclose(archivo);
}

//Generar codigo de los productos del inventario
int obtenerIdProductos(){

    FILE *archivo;
    archivo = fopen("inventario.dat", "rb");
    if (archivo==NULL)
    {
        exit(1);
    }   

    fseek(archivo, -sizeof(tproducto),SEEK_END);
    tproducto productos;
    fread(&productos, sizeof(tproducto), 1, archivo);

    fclose(archivo);

    return productos.idProducto;

}

//Ingresar nuevos productos al archivo
void crearProductos(){
    
    FILE *archivo;
    archivo = fopen("inventario.dat", "ab");
    if (archivo==NULL)
    {
        exit(1);
    }
    tproducto productos;
    int ultimoId = obtenerIdProductos();
    productos.idProducto = ultimoId +1;
    printf("Ingrese el nombre del producto: ");
    gets(productos.nombreProducto);
    printf("Ingrese el precio del producto: ");
    scanf("%f", &productos.precioProducto);
    printf("Ingrese la cantidad disponible del producto: ");
    scanf("%i", &productos.cantProducto);
    fflush(stdin);
    fwrite(&productos, sizeof(tproducto), 1, archivo);
    fclose(archivo);
    continuar;
}


//Listado de todos los productos existentes 
void listarProductos(){
    FILE *archivo;
    archivo = fopen("inventario.dat", "rb");
    if (archivo==NULL)
    {
        exit(1);
    }
    tproducto productos;
    printf("Listado de los productos existentes\n");

    fread(&productos, sizeof(tproducto), 1, archivo);
    while (!feof(archivo))
    {
        printf("CODIGO: %i | PRODUCTO: %s | PRECIO: %0.2f | CANT. DISP.: %i |\n", productos.idProducto, productos.nombreProducto, productos.precioProducto, productos.cantProducto);
        fread(&productos, sizeof(tproducto), 1, archivo);
    }
    fclose(archivo);
    continuar;
}

//Manejo menú de inventario de los productos
void inventario(){
    int opcInv;
    do
    {
        printf("¿Que desea hacer en el inventario?\n");
        printf("1. Crear producto\n");
        printf("2. Listar productos\n");
        printf("3. Regresar\n");
        printf("Ingrese la opcion: ");
        scanf("%i", &opcInv);
        fflush(stdin);
        switch (opcInv)
        {
        case 1:
            crearProductos();
            break;
        case 2:
            listarProductos(); 
            break;
        default:
            break;
        }
    } while (opcInv!=3);
   continuar;
}


//Creación y registro de las ventas que se hacen 
 
void crearVentas(){

    FILE *archivo;
    archivo = fopen("ventas.dat", "wb");
    if (archivo==NULL)
    {
        exit(1);
    }
    fclose(archivo);
}

//Generar codigo de las ventas registradas

int obtenerIdVentas(){

    FILE *archivo;
    archivo = fopen("ventas.dat", "rb");
    if (archivo==NULL)
    {
        exit(1);
    }

    fseek(archivo, -sizeof(tventas),SEEK_END);
    tventas ventasRegistradas;
    fread(&ventasRegistradas, sizeof(tventas), 1, archivo);

    fclose(archivo);

    return ventasRegistradas.id_venta;

}


/* 
Nombre:
Parametros: id, lista
Retuns: OK RET_OK, FAIL RET_FAIL */
RET buscarProductoId(int idVendido, tproducto* productoEncontrado){

    FILE* archivo = fopen("inventario.dat", "rb");
    if (archivo == NULL) {
        exit(1);
    }

    tproducto productos;
    fread(&productos, sizeof(tproducto), 1, archivo);
    while (!feof(archivo)) {
        if (productos.idProducto == idVendido) {
            *productoEncontrado = productos;
            fclose(archivo);
            return RET_OK;
        }
        fread(&productos, sizeof(tproducto), 1, archivo);
    }
 
    fclose(archivo);
    return RET_FAIL;
}

// Modificar Cantidad en el inventario de Productos 

void modificarCantProductos(tproducto *productoEncontrado){

    FILE* archivo = fopen("inventario.dat", "rb+");
    if (archivo == NULL) {
        exit(1);
    }

    tproducto productos;

    while (fread(&productos, sizeof(tproducto), 1, archivo)==1)
    {
        if (productoEncontrado->idProducto==productos.idProducto)
    {
        productos.cantProducto=productoEncontrado->cantProducto;
        fseek(archivo, -sizeof(tproducto),SEEK_CUR);
        fwrite(&productos, sizeof(tproducto), 1, archivo);
    } 
    fseek(archivo, 0, SEEK_CUR);
    }

    //fread(&productos, sizeof(tproducto), 1, archivo);
    fclose(archivo);
}


void registrarVentas(){
    tventas ventsRegistradas;
    FILE *archivo1;
    archivo1 = fopen("ventas.dat", "ab");
    if (archivo1==NULL)
    {
        exit(1);
    }

    int ultimoIdVentas = obtenerIdVentas();
    ventsRegistradas.id_venta = ultimoIdVentas +1;
    ventsRegistradas.fechaVenta.dd = 25;
    ventsRegistradas.fechaVenta.mm = 05;
    ventsRegistradas.fechaVenta.aa = 2023;
    printf("Ingrese el nombre del Cliente: ");
    gets(ventsRegistradas.comprador.nombreComprador);
    printf("Ingrese el numero de productos para este cliente: ");
    int numProVen;
    scanf("%i", &numProVen);
    for (int i = 0; i < numProVen; i++)
    {
        listarProductos();
        printf("Ingrese el Id del producto vendido: ");
        int idVendido;
        scanf("%i", &idVendido);
        fflush(stdin);
        tproducto productoEncontrado;
        if (buscarProductoId(idVendido, &productoEncontrado) == RET_OK)
        {
            tprodVendido productosVendidos;
            strcpy(productosVendidos.nomProVendido, productoEncontrado.nombreProducto);
            printf("%s \n", productosVendidos.nomProVendido);
            printf("Ingrese la cantidad que vendio de este producto: ");
            scanf("%i", &productosVendidos.cantidadVendida);
            if (productosVendidos.cantidadVendida<=productoEncontrado.cantProducto)
            {
                productosVendidos.precioUnidad = productoEncontrado.precioProducto;
                productosVendidos.precioTotal=productosVendidos.precioUnidad*productosVendidos.cantidadVendida;
                ventsRegistradas.totalVentas+=productosVendidos.precioTotal;
                ventsRegistradas.comprador.productosVendidos[i]=productosVendidos;
                productoEncontrado.cantProducto-=productosVendidos.cantidadVendida;
                modificarCantProductos(&productoEncontrado);
                printf("Venta registrada de manera exitosa\n");
            }else{
                printf("La cantidad vendida no puede ser mayor a la cantidad disponible\n");
                i--;
                }
        }else{
                printf("No existe dicho Id de producto\n");
                i--;
             }        
    }
    ventsRegistradas.comprador.numActProd = numProVen;    
    fwrite(&ventsRegistradas, sizeof(tventas), 1, archivo1);
    fclose(archivo1);
    continuar;
}

//Listado del Registro de las Ventas 

void listarVentas(){

    FILE *archivo;
    archivo = fopen("ventas.dat", "rb");
    if (archivo==NULL)
    {
        exit(1);
    }
    tventas ventasRegistradas;
    printf("Listado de las ventas registradas\n");
    printf("Id Venta, Fecha Venta, Nombre Comprador, Producto Vendido, Cantidad Vendida\n");
    fread(&ventasRegistradas, sizeof(tventas), 1, archivo);
    while (!feof(archivo))
    {
        printf("%i, %i/%i/%i, %s\n", ventasRegistradas.id_venta
        , ventasRegistradas.fechaVenta.dd, ventasRegistradas.fechaVenta.mm,
        ventasRegistradas.fechaVenta.aa, ventasRegistradas.comprador.nombreComprador);
        for (int i = 0; i < ventasRegistradas.comprador.numActProd; i++)
        {
            printf("\t|> %i: %s | CANT: %i | PRE. UNI: %.2f | PRECIO TOTAL: %.2f |\n", i+1, ventasRegistradas.comprador.productosVendidos[i].nomProVendido, 
            ventasRegistradas.comprador.productosVendidos[i].cantidadVendida, ventasRegistradas.comprador.productosVendidos[i].precioUnidad,
            ventasRegistradas.comprador.productosVendidos[i].precioTotal);
        }
        printf("PRECIO TOTAL VENTA: %.2f\n", ventasRegistradas.totalVentas);
        printf("\n");
        fread(&ventasRegistradas, sizeof(tventas), 1, archivo);
    }
    fclose(archivo);
    continuar;
}

int main(){

    int opcMenu;
    productosCreados();
    do
    {
        printf("Menu Principal\n");
        printf("1. Registrar Venta\n");
        printf("2. Inventario\n");
        printf("3. Listar las ventas\n");
        printf("4. Salir\n");
        printf("Ingrese la opcion: ");
        scanf("%i", &opcMenu);
        fflush(stdin);
        system("cls");
        switch (opcMenu)
        {
        case 1:
            registrarVentas();
            break;
        case 2:
            inventario();
            break;
        case 3:
            listarVentas();
            break;
        default:
            printf("Fin del programa\n");
            break;
        }
    } while (opcMenu!=4);
    
    return 0;
}