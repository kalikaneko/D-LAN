#include <Tests.h>
using namespace FileManager;

#include <QtDebug>
#include <QDir>
#include <QStringList>

#include <Protos/common.pb.h>

#include <Exceptions.h>

Tests::Tests()
{
}

void Tests::initTestCase()
{
   this->fileManager = Builder::newFileManager();
}

void Tests::addSharedDirectories()
{
   QStringList dirs;
   dirs << QDir::currentPath().append("/../../terms");
   dirs << QDir::currentPath().append("/asdasdasd"); // This directory doesn't exit.

   try
   {
      this->fileManager->setSharedDirsReadOnly(dirs);
   }
   catch (FileManager::DirsNotFoundException& e)
   {
      foreach (QString path, e.getPaths())
         qDebug() << "This directory hasn't been found : " << path << " (Exception thrown)";
   }
}

void Tests::search()
{
   /**
     * Execute many times the search in parallel
     * with the indexing.
     */
   for (int i = 0; i < 20; i++)
   {
      QString terms("aaaa bbbb cccc");
      quint32 levelResults[] = {
         0, 1, 2, 3, 3, 4, 5, 5
      };
      QString fileResults[] = {
         "aaaa bbbb cccc.txt",
         "aaaa bbbb.txt",
         "aaaa cccc.txt",
         "cccc bbbbbb.txt",
         "bbbb cccc.txt",
         "aaaa dddddd.txt",
         "bbbb dddd.txt",
         "bbbb.txt"
      };
      Protos::Common::FindResult result = this->fileManager->find(terms);
      qDebug() << "Search : '" << terms << "'";
      for (int i = 0; i < result.files_size(); i++)
      {
         qDebug() << "[" << result.files(i).level() << "] " << result.files(i).file().file().name().data();
         QVERIFY(result.files(i).level() == levelResults[i]);
         QVERIFY(QString(result.files(i).file().file().name().data()) == QString(fileResults[i]));
      }
   }
}

void Tests::cleanupTestCase()
{
}

