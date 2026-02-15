LSB Image Steganography 🔐

A command-line interface (CLI) application developed in C that enables users to hide secret text messages within BMP (Bitmap) images. By utilizing Least Significant Bit (LSB) substitution, this tool embeds data into the raw pixel bytes of an image, ensuring the changes are imperceptible to the human eye.
🚀 Overview:
Steganography is the art of hiding information within a non-secret medium. This project focuses on the BMP file format because of its uncompressed nature, which allows for direct manipulation of pixel data without the artifacts found in formats like JPEG.
How it Works:

    The Header: The program reads the BMP header to ensure the file is valid and has enough capacity to store the message.

    The Magic String: A unique identifier is encoded first, allowing the decoder to verify if an image actually contains hidden data.

    LSB Encoding: The LSB of each color byte (Red, Green, Blue) is replaced with one bit of the secret message.

    Visual Integrity: Since only the least significant bit is changed, the color value only fluctuates by a maximum of 1, making the change invisible.

✨ Key Features

    Capacity Validation: Automatically calculates if the provided BMP image is large enough to hold the secret text file.

    Security Layer: Includes a "Magic String" encoding step to identify and validate stego-images during decoding.

    Bit-Level Precision: Uses advanced bit manipulation to ensure data integrity.

    Error Handling: Robust checks for file existence, format compatibility, and memory allocation.

    Performance: Optimized File I/O handling for fast encoding and decoding.

🛠️ Technical Skills Demonstrated

    Low-Level C Programming: Extensive use of pointers and structures.

    Binary Operations: Proficiency in bitwise operators (&, |, <<, >>) for LSB modification.

    File Systems: Deep dive into BMP file architecture (Header, DIB Header, and Pixel Array).

    Memory Management: Efficient handling of buffers and file pointers.

📂 Project Structure

    encode.c / encode.h: Logic for embedding data into the image.

    decode.c / decode.h: Logic for extracting data from the stego-image.

    types.h: User-defined types and structures.

    main.c: The entry point for the CLI.
