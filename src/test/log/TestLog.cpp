#include "Path.h"
#include "Level.h"
#include "FileAppender.h"
#include "Logger.h"

#include <iostream>

using namespace std;

void test_level();

void test_append();

void test_file_append();

void test_log();

int main(int argc, const char* argv[])
{
	test_level();
    //test_append();
    test_file_append();
    test_log();
	return 0;
}

void test_level()
{
    Level level;
    cout << level.Str() << endl;
}

void test_append()
{
    DateFormat dateFormat;
    Appender ap("screen", dateFormat);

    if (!ap.Append(Level(), "this a test"))
    {
        cout << "Err" << endl;
    }
}

void test_file_append()
{
    DateFormat dateFormat;
    FileAppender ap("file", dateFormat, Path::GetExecuteFilePath().ExtractPath() + "app.log");

    if (!ap.Append(Level(Level::ERROR), "this a file append test"))
    {
        cout << "Err" << endl;
    }
}

void test_log()
{
    DateFormat dateFormat;
    SmartPtr<Appender> app1(new Appender("screen", dateFormat));
    string path = Path::GetExecuteFilePath().ExtractPath() + "app.log";
    cout << path << endl;
    SmartPtr<Appender> app2(new FileAppender("file", dateFormat, path));

    Logger logger;
    logger.AddAppender(app1);
    logger.AddAppender(app2);
    logger.Fatal("this a fatal test");
    logger.Error("this a error test");
    logger.Warn("this a warn test");
    logger.Notice("this a notice test");
    logger.Info("this a info test");
    logger.Debug("this a debug test");
}
