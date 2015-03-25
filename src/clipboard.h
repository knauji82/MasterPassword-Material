/**
 * clipboard.h
 * This file is part of MasterPassword-Material
 *
 * Copyright (c) 2015 Kilian Schweppe
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program. If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef CLIPBOARD_H
#define CLIPBOARD_H

#include <QApplication>
#include <QThread>
#include <QClipboard>

class ResetClipboardThread: public QThread
{
  Q_OBJECT

private:
  uint secs_;
  bool clear_now_;

protected:
  void run();

public:
  ResetClipboardThread();
  ResetClipboardThread(uint secs);

  inline uint timeout() const
  {
    return secs_;
  }

  inline void setTimout(uint secs)
  {
    secs_ = secs;
  }

  Q_INVOKABLE inline void clearNow()
  {
    clear_now_ = true;
  }

  void blockUntilFinished();

signals:
  void progressChanged(float progress, int seconds_left);
};

#endif // CLIPBOARD_H

