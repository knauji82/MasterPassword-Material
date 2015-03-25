/**
 * clipboard.cc
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

#include "clipboard.h"

#define SLEEP_TIME 250

ResetClipboardThread::ResetClipboardThread() : secs_(0) {}

ResetClipboardThread::ResetClipboardThread(uint secs) : secs_(secs) {}

void ResetClipboardThread::run()
{
  emit progressChanged(0, secs_);
  float progress = 0;
  clear_now_ = false;
  for (unsigned long slept=0; slept < secs_*1000 && !clear_now_; slept += SLEEP_TIME)
  {
    progress = (float) slept / (float) (secs_*1000) ;
    emit progressChanged(progress, secs_ - slept/1000);
    msleep(SLEEP_TIME);
  }
  emit progressChanged(1.0, 0);
  qDebug("reset clipboard");
#ifndef Q_OS_WIN32
  // fix threading bug:
  // on windows the clipboard is cleared by the MasterPasswordApp instance after the 'finished' signal is received
  QApplication::clipboard()->setText("");
#endif
}

void ResetClipboardThread::blockUntilFinished()
{
  if (isRunning())
  {
    QEventLoop loop;
    connect(this, SIGNAL(finished()), &loop, SLOT(quit()));
    // wait until finished
    loop.exec();
  }
}
