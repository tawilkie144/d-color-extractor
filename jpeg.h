#include <turbojpeg.h>

int decompress_image(){
    long unsigned int _jpegSize; //!< _jpegSize from above
    unsigned char* _compressedImage; //!< _compressedImage from above

    int jpegSubsamp, width, height;
    unsigned char buffer[width*height*COLOR_COMPONENTS]; //!< will contain the decompressed image

    tjhandle _jpegDecompressor = tjInitDecompress();

    tjDecompressHeader2(_jpegDecompressor, _compressedImage, _jpegSize, &width, &height, &jpegSubsamp);

    tjDecompress2(_jpegDecompressor, _compressedImage, _jpegSize, buffer, width, 0/*pitch*/, height, TJPF_RGB, TJFLAG_FASTDCT);

    tjDestroy(_jpegDecompressor);

}