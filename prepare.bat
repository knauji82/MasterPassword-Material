@echo off

if not exist lib\MasterPassword goto CLONE_MPW
pushd lib\MasterPassword
git pull --rebase
popd
goto BUILD_SCRYPT

:CLONE_MPW
git clone https://github.com/Lyndir/MasterPassword lib\MasterPassword
patch -p1 < lib\masterpassword-windows.patch
pushd lib\MasterPassword
git commit -a -m "Windows compatibility"
popd
mklink /J lib\masterpassword-algorithm lib\MasterPassword\MasterPassword\C

:BUILD_SCRYPT
if exist lib\scrypt\built goto :eof
set includedir=%cd%\lib\include\scrypt
git svn clone --prefix=origin/ --stdlayout "http://scrypt-fork.googlecode.com/svn" lib\scrypt
pushd lib\scrypt
cd windows
SET PARAMS=-fexceptions -O -fno-omit-frame-pointer -fno-strict-aliasing -Wall -msse -msse2 -DHAVE_CONFIG_H=1
SET INC=-I. -I../ -I../lib/crypto -I../lib/util -I../libcperciva/alg -I../libcperciva/crypto -I../libcperciva/util -IC:/OpenSSL-Win32/include
gcc %PARAMS% %INC% -o ../scrypt-crypto_scrypt-nosse.o -c ../lib/crypto/crypto_scrypt-nosse.c
gcc %PARAMS% %INC% -o ../scrypt-crypto_aesctr.o -c ../libcperciva/crypto/crypto_aesctr.c
gcc %PARAMS% %INC% -o ../scrypt-scryptenc.o -c ../lib/scryptenc/scryptenc.c
gcc %PARAMS% %INC% -o ../scrypt-scryptenc_cpuperf.o -c ../lib/scryptenc/scryptenc_cpuperf.c
gcc %PARAMS% %INC% -o ../scrypt-crypto_scrypt-nosse.o -c ../lib/crypto/crypto_scrypt-nosse.c
gcc %PARAMS% %INC% -o ../scrypt-memlimit.o -c ../lib/util/memlimit.c
gcc %PARAMS% %INC% -o ../scrypt-sha256.o -c ../libcperciva/alg/sha256.c
cd ..
echo %date% > built
mkdir "%includedir%
call :copyHeader
popd
goto :eof

:copyHeader
for %%f in (*.h) do copy "%%f" "%includedir%"
for /D %%d in (*) do (
  cd %%d
  call :copyHeader
  cd ..
)
goto :eof
