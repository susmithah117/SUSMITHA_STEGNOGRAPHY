#include <stdio.h>
#include "encode.h"
#include "decode.h"
#include "types.h"
#include <string.h>

int main(int argc,char *argv[])
{
   // char argv []array of character of pointer
   EncodeInfo encInfo;

   int ret = check_operation_type(argv);

   if(ret == e_unsupported)
{
    printf("invalid args\n");
    return 0;
}
if(ret == e_encode)
{
    //encode
    int ret = read_and_validate_encode_args(argv, &encInfo);
    if(ret == e_failure)
    {
        printf("invalid args\n");
        return 0;
    }
    //start encoding
    ret = do_encoding(&encInfo);
    if(ret == e_failure)
    {
        printf("encoding failed");    
        return 0;
    }
    else
    {
        printf("Encoding sucessfull\n");
        return 0;
    }
}
 
DecodeInfo decInfo;

if(ret == e_decode)
{ //decoding
    //1.call read_and_validate_decode_args(argv,&decode)
    int ret = read_and_validate_decode_args(argv, &decInfo);
    if(ret == e_failure)
    {
        printf("Invalid decode arguments\n");
        return 0;
    }

    ret = do_decoding(&decInfo);
    if(ret == e_failure)
    {
        printf("Decoding failed\n");
        return 0;
    }
    else
    {
        printf("Decoded successful\n");
        return 0;
    }
}

}

   OperationType check_operation_type(char *argv[])

   {
    
    if(argv[1] ==NULL)
    {
       
       return e_unsupported;
    }
     if(strcmp(argv[1],"-e")==0)
     {
      return e_encode;
     }
     if(strcmp(argv[1],"-d")==0)
     {
       
       return e_decode;
     }
     else
     {
        printf("there\n");
         return e_unsupported;
     }
    
   }
   // Status check_capacity(EncodeInfo *encInfo){
        /*
        1.calculate no of bytes needed for encoding from src files
           count = (magic_str_len + extn_size(int) + extn_len + 
           file_size(int) + file_data_len + 54) * 8 + 54 

        2. check count <= src_file_size
        get the src_file_size from the function it is already written
        
    
        */
    

