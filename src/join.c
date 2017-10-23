////////////////////////////////////////////////////////////////////////////////////////////////////
//
// Programa para unir archivo cortados
//
// La motivación de ésta mini aplicación fue unificar los archivo del siguiente artículo:
//   https://sites.google.com/site/tangchq/qemu-fvd
// Los archivo se nombran de la siguiente manera:
//   qemu-fvd-June-9-2011.tar.gz-00
//   qemu-fvd-June-9-2011.tar.gz-01
//   qemu-fvd-June-9-2011.tar.gz-02
//
// Germán Martínez - 08/03/2017
//
////////////////////////////////////////////////////////////////////////////////////////////////////
#include <stdio.h>
#include <errno.h>


// Tamaño del buffer de transferencia
#define BUFFER_SIZE					0x1000

// Strings (English, Español)
#if 1
#define STR_CANNOT_OPEN_DST_FILE	"Can't open destination file \"%s\""
#define STR_CANNOT_OPEN_SRC_FILE	"Can't open source file \"%s\""
#define STR_CANNOT_READ_SRC_FILE	"Can't read from source file \"%s\""
#define STR_CANNOT_WRITE_DST_FILE	"Can't write to destination file \"%s\""
#else
#define STR_CANNOT_OPEN_DST_FILE	"No se puede abrir el archivo de destino \"%s\""
#define STR_CANNOT_OPEN_SRC_FILE	"No se puede abrir el archivo de origen \"%s\""
#define STR_CANNOT_READ_SRC_FILE	"No se puede leer desde el archivo de origen \"%s\""
#define STR_CANNOT_WRITE_DST_FILE	"No se puede escribir en el archivo de destino \"%s\""
#endif

int main(int argc, char* argv[])
{
	if(argc >= 3)
	{
		int i;
		FILE* in;
		FILE* out;
		int n;
		long size;
		static char buffer[BUFFER_SIZE];
		int x;
		
		//
		// Crear el archivo de destino
		//
		if(NULL == (out = fopen(argv[1], "wb")))
		{
			fprintf(stderr, STR_CANNOT_OPEN_DST_FILE, argv[1]);
			return -1;
		}
		//
		// Iterar sobre cada uno de los archivo de origen y escribirlos en el archivo de destino
		//
		for(i = 2; i < argc; i++)
		{
			// Abrir archivo de origen
			if(NULL == (in = fopen(argv[i], "rb")))
			{
				fclose(out);
				fprintf(stderr, STR_CANNOT_OPEN_SRC_FILE, argv[1]);
				return -1;
			}
			//
			// Calcular el tamaño del archivo de origen
			//
			fseek(in, 0, SEEK_END);
			size = ftell(in);
			fseek(in, 0, SEEK_SET);
			
			//
			// Leer y escribir
			//
			while(size > 0)
			{
				// Calcular cantidad de bytes de la transferencia
				n = size;
				if(n > BUFFER_SIZE) n = BUFFER_SIZE;
				
				// Leer
				if(n != fread(buffer, 1, n, in))
				{
					fclose(in);
					fclose(out);
					fprintf(stderr, STR_CANNOT_READ_SRC_FILE, argv[i]);
					return -1;
				}
				// Escribir
				if(n != fwrite(buffer, 1, n, out))
				{
					fclose(in);
					fclose(out);
					fprintf(stderr, STR_CANNOT_WRITE_DST_FILE, argv[i]);
					return -1;
				}
				// ...
				size -= n;
			}
			
			// Cerrar el archivo actual de origen
			fclose(in);
		}
		// Cerra el archivo de destino
		fclose(out);
	}
	return 0;
}
