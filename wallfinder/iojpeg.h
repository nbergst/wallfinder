#ifndef _IOJPEG_H_
#define _IOJPEG_H_

#include <jpeglib.h>

#include <cstdio>

/**
 *  Defines the dimension of an image
 */
struct dimension {
    int x; /**< x-dimension (width) */
    int y; /**< y-dimension (height) */
    int z; /**< z-dimension (number of channels) */
};
//typedef struct dimension dim;

unsigned char* readJpegFile( const char* );
int writeJpegFile( const char* _name, unsigned char* _data, dimension _dim );

unsigned char* readJpegFile( const char* _name ) {
    
    struct jpeg_decompress_struct info;
    struct jpeg_error_mgr err;
    
    JSAMPROW rowPtr[1];
    
    FILE* file = fopen( _name, "rb" );
    unsigned long pos=0;
    unsigned int i=0;
    
    if( !file ) {
        // Throw exception
        printf("Couldn't find file %s\n", _name);
        return NULL;
    }
    
    info.err = jpeg_std_error( &err );
    jpeg_create_decompress( &info );
    jpeg_stdio_src( &info, file );
    
    // Read file info
    jpeg_read_header( &info, TRUE );
    
    jpeg_start_decompress( &info );
    
    unsigned char* data = (unsigned char *)malloc( info.image_width*info.image_height*info.num_components );
    
    rowPtr[0] = (unsigned char *)malloc( info.output_width*info.num_components );
    
    while( info.output_scanline < info.image_height ) {
        jpeg_read_scanlines( &info, rowPtr, 1 );
        for( i=0; i<info.image_width*info.num_components;i++) 
            data[pos++] = rowPtr[0][i];
    }
    
    jpeg_finish_decompress( &info );
    jpeg_destroy_decompress( &info );
    free( rowPtr[0] );
    fclose( file );
    
    return data;
    
}



int writeJpegFile( const char* _name, unsigned char* _data, struct dimension _dim ) {
    
    struct jpeg_compress_struct info;
    struct jpeg_error_mgr err;
	
    /* this is a pointer to one row of image data */
    JSAMPROW rowPtr[1];
    FILE *file = fopen( _name, "wb" );
	
    if ( !file ) {
        printf("Error opening file %s\n", _name );
        return -1;
    }
    
    info.err = jpeg_std_error( &err );
    jpeg_create_compress(&info);
    jpeg_stdio_dest(&info, file);
    
    /* Setting the parameters of the output file here */
    info.image_width = _dim.x;
    info.image_height = _dim.y;
    info.input_components = _dim.z;
    
    switch( _dim.z ) {
        case 1:
            info.in_color_space = JCS_GRAYSCALE;
            break;
        case 3:
            info.in_color_space = JCS_RGB;
            break;
        default:
            info.in_color_space = JCS_UNKNOWN;
            break;
    }
    
    jpeg_set_defaults( &info );
    
    jpeg_start_compress( &info, TRUE );
    
    while( info.next_scanline < info.image_height )
    {
        rowPtr[0] = &_data[ info.next_scanline * info.image_width *  info.input_components];
        jpeg_write_scanlines( &info, rowPtr, 1 );
    }
    
    jpeg_finish_compress( &info );
    jpeg_destroy_compress( &info );
    fclose( file );
    
    return 0;
    
}

#endif // _IOJPEG_H_
