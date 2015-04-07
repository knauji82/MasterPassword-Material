#include "masterkey.h"

QString MasterKey::encrypt(QString const &text)
{
  SimpleCrypt crypto;
  qint64 key;
  QString encrypted = text;

  for (int i=0; i < MP_dkLen; i += sizeof(qint64))
  {
    memcpy(&key, key_+i, sizeof(qint64));
    crypto.setKey(key);
    encrypted = crypto.encryptToString(encrypted);
  }

  return encrypted;
}

QString MasterKey::decrypt(QString const &encrypted)
{
  SimpleCrypt crypto;
  qint64 key;
  QString decrypted = encrypted;

  for (int i=MP_dkLen-sizeof(qint64); i >= 0; i -= sizeof(qint64))
  {
    memcpy(&key, key_+i, sizeof(qint64));
    crypto.setKey(key);
    decrypted = crypto.decryptToString(decrypted);
  }

  return decrypted;
}
