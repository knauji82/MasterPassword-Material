/*
  Copyright (c) 2011, Andre Somers
  All rights reserved.

  Redistribution and use in source and binary forms, with or without
  modification, are permitted provided that the following conditions are met:
      * Redistributions of source code must retain the above copyright
        notice, this list of conditions and the following disclaimer.
      * Redistributions in binary form must reproduce the above copyright
        notice, this list of conditions and the following disclaimer in the
        documentation and/or other materials provided with the distribution.
      * Neither the name of the Rathenau Instituut, Andre Somers nor the
        names of its contributors may be used to endorse or promote products
        derived from this software without specific prior written permission.

  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
  ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
  WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
  DISCLAIMED. IN NO EVENT SHALL ANDRE SOMERS BE LIABLE FOR ANY
  DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
  (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
  LOSS OF USE, DATA, OR #######; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
  ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
  (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
  SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/

#include "simplecrypt.h"
#include <QByteArray>
#include <QtDebug>
#include <QtGlobal>
#include <QDateTime>
#include <QCryptographicHash>
#include <QDataStream>

SimpleCrypt::SimpleCrypt()
    : key_(0),
      compression_mode_(CompressionAuto),
      protection_mode_(ProtectionChecksum),
      last_error_(ErrorNoError)
{
  qsrand(uint(QDateTime::currentMSecsSinceEpoch() & 0xFFFF));
}

SimpleCrypt::SimpleCrypt(quint64 key)
    : key_(key),
      compression_mode_(CompressionAuto),
      protection_mode_(ProtectionChecksum),
      last_error_(ErrorNoError)
{
  qsrand(uint(QDateTime::currentMSecsSinceEpoch() & 0xFFFF));
  splitKey();
}

void SimpleCrypt::setKey(quint64 key)
{
  key_ = key;
  splitKey();
}

void SimpleCrypt::splitKey()
{
  key_parts_.clear();
  key_parts_.resize(8);
  for (int i=0;i<8;i++)
  {
    quint64 part = key_;
    for (int j=i; j>0; j--)
      part = part >> 8;
    part = part & 0xff;
    key_parts_[i] = static_cast<char>(part);
  }
}

QByteArray SimpleCrypt::encryptToByteArray(QString const &plaintext)
{
  QByteArray plaintext_array = plaintext.toUtf8();
  return encryptToByteArray(plaintext_array);
}

QByteArray SimpleCrypt::encryptToByteArray(QByteArray plaintext)
{
  if (key_parts_.isEmpty())
  {
    qWarning() << "No key set.";
    last_error_ = ErrorNoKeySet;
    return QByteArray();
  }

  QByteArray ba = plaintext;

  CryptoFlags flags = CryptoFlagNone;
  if (compression_mode_ == CompressionAlways)
  {
    ba = qCompress(ba, 9); //maximum compression
    flags |= CryptoFlagCompression;
  }
  else if (compression_mode_ == CompressionAuto)
  {
    QByteArray compressed = qCompress(ba, 9);
    if (compressed.count() < ba.count())
    {
      ba = compressed;
      flags |= CryptoFlagCompression;
    }
  }

  QByteArray integrity_protection;
  if (protection_mode_ == ProtectionChecksum)
  {
    flags |= CryptoFlagChecksum;
    QDataStream s(&integrity_protection, QIODevice::WriteOnly);
    s << qChecksum(ba.constData(), ba.length());
  }
  else if (protection_mode_ == ProtectionHash)
  {
    flags |= CryptoFlagHash;
    QCryptographicHash hash(QCryptographicHash::Sha1);
    hash.addData(ba);

    integrity_protection += hash.result();
  }

  //prepend a random char to the string
  char rand_char = char(qrand() & 0xFF);
  ba = rand_char + integrity_protection + ba;

  int pos(0);
  char last_char(0);

  int cnt = ba.count();

  while (pos < cnt)
  {
    ba[pos] = ba.at(pos) ^ key_parts_.at(pos % 8) ^ last_char;
    last_char = ba.at(pos);
    ++pos;
  }

  QByteArray result_array;
  result_array.append(char(0x03));  //version for future updates to algorithm
  result_array.append(char(flags)); //encryption flags
  result_array.append(ba);

  last_error_ = ErrorNoError;

  return result_array;
}

QString SimpleCrypt::encryptToString(QString const &plaintext)
{
  QByteArray plaintext_array = plaintext.toUtf8();
  QByteArray cypher = encryptToByteArray(plaintext_array);
  QString cypher_string = QString::fromLatin1(cypher.toBase64());

  return cypher_string;
}

QString SimpleCrypt::encryptToString(QByteArray plaintext)
{
  QByteArray cypher = encryptToByteArray(plaintext);
  QString cypher_string = QString::fromLatin1(cypher.toBase64());

  return cypher_string;
}

QString SimpleCrypt::decryptToString(QString const &cyphertext)
{
  QByteArray cyphertext_array = QByteArray::fromBase64(cyphertext.toLatin1());
  QByteArray plaintext_array = decryptToByteArray(cyphertext_array);
  QString plaintext = QString::fromUtf8(plaintext_array, plaintext_array.size());

  return plaintext;
}

QString SimpleCrypt::decryptToString(QByteArray cypher)
{
  QByteArray ba = decryptToByteArray(cypher);
  QString plaintext = QString::fromUtf8(ba, ba.size());

  return plaintext;
}

QByteArray SimpleCrypt::decryptToByteArray(QString const &cyphertext)
{
  QByteArray cyphertext_array = QByteArray::fromBase64(cyphertext.toLatin1());
  QByteArray ba = decryptToByteArray(cyphertext_array);

  return ba;
}

QByteArray SimpleCrypt::decryptToByteArray(QByteArray cypher)
{
  if (key_parts_.isEmpty())
  {
    qWarning() << "No key set.";
    last_error_ = ErrorNoKeySet;
    return QByteArray();
  }

  QByteArray ba = cypher;

  if (cypher.count() < 3 )
    return QByteArray();

  char version = ba.at(0);

  //we only work with version 3
  if (version !=3)
  {
    last_error_ = ErrorUnknownVersion;
    qWarning() << "Invalid version or not a cyphertext.";
    return QByteArray();
  }

  CryptoFlags flags = CryptoFlags(ba.at(1));

  ba = ba.mid(2);
  int pos(0);
  int cnt(ba.count());
  char last_char = 0;

  while (pos < cnt)
  {
    char current_char = ba[pos];
    ba[pos] = ba.at(pos) ^ last_char ^ key_parts_.at(pos % 8);
    last_char = current_char;
    ++pos;
  }

  //chop off the random number at the start
  ba = ba.mid(1);

  bool integrity_ok(true);
  if (flags.testFlag(CryptoFlagChecksum))
  {
    if (ba.length() < 2)
    {
      last_error_ = ErrorIntegrityFailed;
      return QByteArray();
    }
    quint16 stored_checksum;
    {
      QDataStream s(&ba, QIODevice::ReadOnly);
      s >> stored_checksum;
    }
    ba = ba.mid(2);
    quint16 checksum = qChecksum(ba.constData(), ba.length());
    integrity_ok = (checksum == stored_checksum);
  }
  else if (flags.testFlag(CryptoFlagHash))
  {
    if (ba.length() < 20)
    {
      last_error_ = ErrorIntegrityFailed;
      return QByteArray();
    }
    QByteArray stored_hash = ba.left(20);
    ba = ba.mid(20);
    QCryptographicHash hash(QCryptographicHash::Sha1);
    hash.addData(ba);
    integrity_ok = (hash.result() == stored_hash);
  }

  if (!integrity_ok)
  {
    last_error_ = ErrorIntegrityFailed;
    return QByteArray();
  }

  if (flags.testFlag(CryptoFlagCompression))
    ba = qUncompress(ba);

  last_error_ = ErrorNoError;

  return ba;
}
