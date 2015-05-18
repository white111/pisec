Package requirements :
- httpd
- mysql server
- PHP
- wiringPi( GPIO ) / pfio ( PiFace )


1. Copy all files from 'web' project directory to your website dir
( /var/www)

2. Download 'src' folder to any location

3. Within 'src', edit 'pisecWPI'/'pisecPiF' with input/output options as required.

4. Compile source file ('pisecWPI' or 'pisecPiF') with :
- g++ -Iinclude -Llib pisecWPI.cpp -lwiringPi -o pisecWPI ( GPIO )
or
- g++ -Iinclude -Llib pisecPiF.cpp -lpfio -o pisecPiF ( PiFace )

5. Create empty file (shared object) in '/var/www/html' , with 0666 privilege, named as in C++ and PHP source, default is 'pisecSHM'  ( touch pisecSHM, chmod 666 pisecSHM).