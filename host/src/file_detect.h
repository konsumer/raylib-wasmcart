#include <stdio.h>
#include <stdint.h>
#include <sys/stat.h>
#include <unistd.h>
#include <string.h>

typedef enum {
    FILE_TYPE_UNKNOWN,
    FILE_TYPE_ZIP,
    FILE_TYPE_WASM,
    FILE_TYPE_PNG,
    FILE_TYPE_JPEG,
    FILE_TYPE_WAV,
    FILE_TYPE_OGG,
    FILE_TYPE_MP3,
    FILE_TYPE_DIR
} DetectFileType;

// just detect filetype from first 4 bytes
DetectFileType parse_magic_bytes(uint32_t magic_number) {
  switch (magic_number) {
    case 0x4034b50:  // ZIP
        return FILE_TYPE_ZIP;
        
    case 0x6d736100:  // WASM
        return FILE_TYPE_WASM;
        
    case 0x89504E47:  // PNG
        return FILE_TYPE_PNG;
        
    // JPEG variants
    case 0xe0ffd8ff:  // JPEG/JFIF
    case 0xe1ffd8ff:  // JPEG/Exif
    case 0xeeffd8ff:  // JPEG/SPIFF
    case 0xfeffd8ff:  // JPEG/COM
        return FILE_TYPE_JPEG;
        
    case 0x46464952:  // WAV ("RIFF")
        return FILE_TYPE_WAV;
        
    case 0x5367674f:  // OGG ("OggS")
        return FILE_TYPE_OGG;
        
    // MP3 variants
    case 0x03334449:  // MP3 with ID3v2.3
    case 0x02334449:  // MP3s with ID3v2.2
    case 0x04334449:  // MP3 with ID3v2.4
        return FILE_TYPE_MP3;
        
    default:
        return FILE_TYPE_UNKNOWN;
  }
}

// detect filetype from a real file
DetectFileType detect_real_file_type(char* filename) {
  struct stat sb;
  
  if (stat(filename, &sb) != 0){
    return FILE_TYPE_UNKNOWN;
  }
  if (S_ISDIR(sb.st_mode)) {
    return FILE_TYPE_DIR;
  }

  uint32_t magic_number = 0;
  FILE* file = fopen(filename, "rb");
  if (!file) {
    return FILE_TYPE_UNKNOWN;
  }
  fread(&magic_number, sizeof(uint32_t), 1, file);
  fclose(file);

  return parse_magic_bytes(magic_number);
}