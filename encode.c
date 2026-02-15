#include <stdio.h>
#include <string.h>
#include "encode.h"
#include "types.h"
#include "common.h"
#define PRINT_POS(msg, encInfo) \
    printf("%s src = %ld  stego = %ld\n", \
        msg, \
        ftell(encInfo->fptr_src_image), \
        ftell(encInfo->fptr_stego_image))



uint get_image_size_for_bmp(FILE *fptr_image)
{
    uint width, height;
    // Seek to 18th byte
    fseek(fptr_image, 18, SEEK_SET);

    // Read the width (an int)
    fread(&width, sizeof(int), 1, fptr_image);
    printf("width = %u\n", width);

    // Read the height (an int)
    fread(&height, sizeof(int), 1, fptr_image);
    printf("height = %u\n", height);

    // Return image capacity
    return width * height * 3;
}


Status open_files(EncodeInfo *encInfo)
{
    // Src Image file
    encInfo->fptr_src_image = fopen(encInfo->src_image_fname, "r");
    // Do Error handling
    if (encInfo->fptr_src_image == NULL)
    {
    	perror("fopen");
    	fprintf(stderr, "ERROR: Unable to open file %s\n", encInfo->src_image_fname);

    	return e_failure;
    }

    // Secret file
    encInfo->fptr_secret = fopen(encInfo->secret_fname, "r");
    // Do Error handling
    if (encInfo->fptr_secret == NULL)
    {
    	perror("fopen");
    	fprintf(stderr, "ERROR: Unable to open file %s\n", encInfo->secret_fname);

    	return e_failure;
    }

    // Stego Image file
    encInfo->fptr_stego_image = fopen(encInfo->stego_image_fname, "w");
    // Do Error handling
    if (encInfo->fptr_stego_image == NULL)
    {
    	perror("fopen");
    	fprintf(stderr, "ERROR: Unable to open file %s\n", encInfo->stego_image_fname);
          return e_failure;
    }
     // No failure return e_success
    return e_success;
}

Status read_and_validate_encode_args(char *argv[], EncodeInfo *encInfo)
{
      if(argv[2] == NULL)
        {
         printf(".bmp file not passed\n");
         return e_failure;
        }

        if(strstr(argv[2], ".bmp") == NULL)
        {
              printf("invalid image file name");
                return e_failure;
        }
        
        encInfo->src_image_fname = argv[2];
        
        if(argv[3] == NULL)
        {
            printf(".txt file not passed");
                return e_failure;
        }
      if(strstr(argv[3], ".txt") == NULL)
      {
           printf(" invalid sec file name");
                return e_failure;
      }
        encInfo->secret_fname = argv[3];
    
       if(argv[4] == NULL)
       {
       encInfo->stego_image_fname = "stego.bmp";
       }
       else
       {
        if(strstr(argv[4],".bmp")==NULL)
        {
        printf(" invalid stego image file name");
               return e_failure; 
        }
       
        encInfo->stego_image_fname = argv[4];
       }
         char *chr = strchr(encInfo->secret_fname, '.');
         strcpy(encInfo->extn_secret_file, chr);
        return e_success;

}
Status do_encoding(EncodeInfo *encInfo)
{
    int ret = open_files(encInfo);
      if( ret == e_failure)
        {
            printf("open file failed");
                return e_failure;
        }

        ret = check_capacity(encInfo);
        if(ret == e_failure)
        {
                printf("check capacity failed");
                return e_failure;
        }
        copy_bmp_header(encInfo->fptr_src_image, encInfo->fptr_stego_image);
        PRINT_POS("After copy bmp header", encInfo);
         printf("copied bmp header successfully\n");

        ret = encode_magic_string(MAGIC_STRING,encInfo);
        PRINT_POS("After magic string", encInfo);
          printf("encoded magic string successfully\n");
        if(ret==e_failure)
        {
                printf("Magic string failed\n");
                return e_failure;
        }
        ret=encode_secret_file_extn_size(strlen(encInfo->extn_secret_file),encInfo);
          PRINT_POS("After extn size", encInfo);
           printf("encoded secret_file_extn_size successfully\n");
         if(ret==e_failure)
        {
                printf("secret file extension file size failed\n");
                return e_failure;
        }
        ret=encode_secret_file_extn(encInfo->extn_secret_file,encInfo);
         PRINT_POS("After extn", encInfo);
         printf("encoded secret_file_extn successfully\n ");
        if(ret==e_failure)
        {
                printf("secret file extension  failed\n");
                return e_failure;
        }
        ret=encode_secret_file_size(encInfo->size_secret_file,encInfo);
        PRINT_POS("After secret file size", encInfo);
        printf("encoded secret_file_size successfully\n");
         if(ret==e_failure)
        {
                printf("secret file size failed\n");
                return e_failure;
        }
        ret=encode_secret_file_data(encInfo);
         PRINT_POS("After secret file data", encInfo);
         printf("encoded secret_file_data successfully\n");
        if(ret==e_failure)
        {
                printf("secret file data failed\n");
                return e_failure;
        }
        ret=copy_remaining_img_data(encInfo->fptr_src_image,encInfo->fptr_stego_image);
        PRINT_POS("After copying remaining image data", encInfo);
        printf("copied remaining data successfully\n");
        if(ret==e_failure)
        { 
                printf("encoding remaining failed\n");
                return e_failure;
        }
       // printf("after copying data");
        fclose(encInfo-> fptr_src_image);
        fclose(encInfo-> fptr_secret);
        fclose(encInfo-> fptr_stego_image);
          return e_success;

        

}
Status check_capacity(EncodeInfo *encInfo)
{
   
        int size_required;
        int src_fsize;
        int sec_file;

        src_fsize=get_image_size_for_bmp(encInfo->fptr_src_image);
       fseek(encInfo->fptr_src_image,0,SEEK_SET);
        fseek(encInfo->fptr_secret,0,SEEK_END);
        encInfo->size_secret_file=ftell(encInfo->fptr_secret);
       fseek(encInfo->fptr_secret,0,SEEK_SET);

        
        int count=((strlen(MAGIC_STRING)+sizeof(int)+strlen(encInfo->extn_secret_file)+
        sizeof(int)+encInfo->size_secret_file)*8)+54;
        if(count<=src_fsize)
        {
            return e_success;
        }
        return e_failure;
    
}
Status copy_bmp_header(FILE *fptr_src_image, FILE *fptr_stego_image)
{
        char temp[55];  
        fread(temp, 54, 1,fptr_src_image);
        fwrite(temp , 54, 1 ,fptr_stego_image);
        return e_success;
}

Status encode_byte_to_lsb(char data, char *image_buffer)
{
    for(int i = 0; i < 8; i++)
    {
        char bit = (data >> (7 - i)) & 1;
        image_buffer[i] = (image_buffer[i] & 0xFE) | bit;
    }
    return e_success;
}

Status encode_size_to_lsb(int data, char *image_buffer)
{
    for(int i = 0; i < 32; i++)
    {
        char bit = (data >> (31 - i)) & 1;  
        image_buffer[i] = (image_buffer[i] & 0xFE) | bit;
    }
    return e_success;
}



Status encode_magic_string(const char *magic_string, EncodeInfo *encInfo)
{
    // for (i=1; i<=2; i++)
    for(int i=0;i<strlen(MAGIC_STRING);i++)
    {
        char temp[8];
        //1.read 8 bytes buffer from src file and store to temp array
         fread( temp,1, 8, encInfo->fptr_src_image);
         // 2.byte_to_lsb(magic_string[i], temp);
         encode_byte_to_lsb(magic_string[i], temp);
         //3.write temp array 8 bytes to stego file
         fwrite(temp,1, 8, encInfo->fptr_stego_image);
    }
    
     return e_success;
}

Status encode_secret_file_extn_size(int file_extn_size, EncodeInfo *encInfo)
{
    
        char temp_buffer[32];
       // 1.read 32 bytes buff from src file
        fread( temp_buffer,1, 32, encInfo->fptr_src_image);
        //2. call size_to_lsb(file_extn_size, temp_buffer)
        encode_size_to_lsb(file_extn_size, temp_buffer);
        //3. write temp_buffer to stego file
     fwrite(temp_buffer,1, 32, encInfo->fptr_stego_image);
     return e_success;
}
/* Encode secret file extenstion */
Status encode_secret_file_extn(const char *file_extn, EncodeInfo *encInfo)
{
   
        //1.run a loop 0 to file_extn_len-1 times
        for(int i=0;i<strlen(file_extn);i++)
        {
            char temp[8];
            //2.read 8 bytes of buffer from src file
            fread(temp,1, 8, encInfo->fptr_src_image);
            //3.call byte_to_lsb(file_extn[i], temp);
            encode_byte_to_lsb(file_extn[i], temp);
            //4. write 8 bytes temp to stego file
            fwrite(temp,1,8, encInfo->fptr_stego_image);
        }  
      return e_success;
}

Status encode_secret_file_size(int file_size, EncodeInfo *encInfo)
{
    char temp[32];
        //2. read 32 bytes of buff from src file
     fread(temp,1, 32, encInfo->fptr_src_image);   
        //3. cal size_to_lsb(file_size, temp);
        encode_size_to_lsb(file_size, temp);
        //4. write temp to stego file
        fwrite(temp,1,32,encInfo->fptr_stego_image);
    
}
Status encode_secret_file_data(EncodeInfo *encInfo)
{
    char ch=0;    
    char temp[8];
        //1. run a loop until src_file reaching EOF
            //1. read a ch from sec file
         //  while (fread(&ch, 1, 1, encInfo->fptr_secret != ))
         while((ch=fgetc(encInfo->fptr_secret))!=EOF)
            {  
                //2. read 8 bytes buff from src file
                fread(temp, 1, 8, encInfo->fptr_src_image);
                 // 3.call byte_to_lsb(ch, temp)
                encode_byte_to_lsb(ch, temp);
                 // 4.write temp to stego file
                fwrite(temp, 1, 8, encInfo->fptr_stego_image);
    
        }
         printf("offset position of secret file %ld\n",ftell(encInfo->fptr_secret));
         printf("offset position of secret file %ld\n",ftell(encInfo->fptr_stego_image));
    return e_success;
}
Status copy_remaining_img_data(FILE *fptr_src, FILE *fptr_dest)
{
       char ch;
       ch=fgetc(fptr_src);
        //1. run a loop until src file is reaching EOF
        //1. read a 1 byte from src file
         while(!feof(fptr_src))
        {
          //2.write 1 byte to dest file
         fputc(ch,fptr_dest);
         ch=fgetc(fptr_src);
        }
        return e_success;
        
}

