/**
  * D-LAN - A decentralized LAN file sharing software.
  * Copyright (C) 2010-2012 Greg Burri <greg.burri@gmail.com>
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
  * along with this program.  If not, see <http://www.gnu.org/licenses/>.
  */
  
#include <D-LAN_GUI.h>
using namespace GUI;

#include <QMessageBox>
#include <QPushButton>
#include <QSharedMemory>

#include <Common/LogManager/Builder.h>
#include <Common/Constants.h>
#include <Common/Settings.h>

#include <Common/RemoteCoreController/Builder.h>

static const QString sharedMemoryKeyname("D-LAN GUI instance");
static QSharedMemory sharedMemory;

/**
  * @class GUI::D_LAN_GUI
  * This class control the trayIcon and create the main window.
  * The main window can be hid and deleted, the tray icon will still remain and will permit to relaunch the main window.
  */

D_LAN_GUI::D_LAN_GUI(int argc, char *argv[]) :
   QApplication(argc, argv),
   mainWindow(0),
   coreConnection(RCC::Builder::newCoreConnection(SETTINGS.get<quint32>("socket_timeout"))),
   trayIcon(QIcon(":/icons/ressources/icon.png"))
{
   LM::Builder::setLogDirName("log_gui");

   // If multiple instance isn't allowed we will test if a particular
   // shared memory segment alreydy exists. There is actually no
   // easy way to bring the already existing GUI windows to the front without
   // dirty polling.
   // Under linux the flag may persist after process crash.
#ifndef Q_OS_LINUX
   if (!SETTINGS.get<bool>("multiple_instance_allowed"))
   {
      sharedMemory.lock();
      sharedMemory.setKey(sharedMemoryKeyname);
      if (!sharedMemory.create(1))
      {
         QMessageBox message;
         message.setWindowTitle(QObject::tr("D-LAN already launched"));
         message.setText(QObject::tr("An instance of D-LAN is already launched"));
         message.setIcon(QMessageBox::Information);
         QAbstractButton* abortButton = message.addButton(QMessageBox::Abort);
         message.addButton(QObject::tr("Launch anyway"), QMessageBox::ActionRole);
         message.exec();
         if (message.clickedButton() == abortButton)
         {
            sharedMemory.unlock();
            QSharedPointer<LM::ILogger> mainLogger = LM::Builder::newLogger("D-LAN GUI");
            mainLogger->log("GUI already launched, exited..", LM::SV_END_USER);
            throw AbortException();
         }
      }
      sharedMemory.unlock();
   }
#endif

   this->setQuitOnLastWindowClosed(false);

   this->showMainWindow();

   connect(&this->trayIcon, SIGNAL(activated(QSystemTrayIcon::ActivationReason)), this, SLOT(trayIconActivated(QSystemTrayIcon::ActivationReason)));

   this->updateTrayIconMenu();

   this->trayIcon.setContextMenu(&this->trayIconMenu);
   #ifndef Q_OS_LINUX
      // Fix a bug on ubuntu x86_64 (core dumped)
      this->trayIcon.setToolTip("D-LAN");
   #endif
   this->trayIcon.show();
}

bool D_LAN_GUI::event(QEvent* event)
{
   if (event->type() == QEvent::LanguageChange)
      this->updateTrayIconMenu();

   return QApplication::event(event);
}

void D_LAN_GUI::trayIconActivated(QSystemTrayIcon::ActivationReason reason)
{
   if (reason == QSystemTrayIcon::Trigger)
      this->showMainWindow();
}

void D_LAN_GUI::mainWindowClosed()
{
   if (this->coreConnection->isConnected())
      this->trayIcon.showMessage("D-LAN GUI closed", "D-LAN Core is still running in background. Select 'exit' from the contextual menu if you want to stop it.");
   this->coreConnection->disconnectFromCore();
   this->mainWindow = nullptr;
}

void D_LAN_GUI::showMainWindow()
{
   if (this->mainWindow)
   {
      this->mainWindow->setWindowState(Qt::WindowActive);
      this->mainWindow->raise();
      this->mainWindow->activateWindow();
   }
   else
   {
      this->mainWindow = new MainWindow(this->coreConnection);
      connect(this->mainWindow, SIGNAL(destroyed()), this, SLOT(mainWindowClosed()));
      this->mainWindow->show();
   }
}

/**
  * Stop only the GUI.
  */
void D_LAN_GUI::exitGUI()
{
   this->exit(false);
}

void D_LAN_GUI::exit(bool stopTheCore)
{
   this->trayIcon.hide();

   if (stopTheCore)
      this->coreConnection->stopLocalCore();

   if (this->mainWindow)
   {
      disconnect(this->mainWindow, SIGNAL(destroyed()), this, SLOT(mainWindowClosed()));
      delete this->mainWindow;
   }

   this->quit();
}

void D_LAN_GUI::updateTrayIconMenu()
{
   this->trayIconMenu.clear();
   this->trayIconMenu.addAction(tr("Show the GUI"), this, SLOT(showMainWindow()));
   if (!this->coreConnection->isRunningAsSubProcess()) // We cannot stop a parent process without killing his child.
      this->trayIconMenu.addAction(tr("Stop the GUI"), this, SLOT(exitGUI()));
   this->trayIconMenu.addSeparator();
   this->trayIconMenu.addAction(tr("Exit"), this, SLOT(exit()));
}
