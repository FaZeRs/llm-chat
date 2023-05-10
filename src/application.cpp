#include <QApplication>

#include <sentry.h>
#include "spdlog/spdlog.h"

#include "application.h"
#include "version.h"
#include "logger.h"
#include "parameters.h"

static void qtLogMessageHandler(QtMsgType type,
                                const QMessageLogContext& context,
                                const QString& msg) {
  QByteArray loc = msg.toUtf8();
  switch (type) {
    case QtDebugMsg:
      spdlog::log(
          spdlog::source_loc{context.file, context.line, context.function},
          spdlog::level::debug, loc.constData());
      break;
    case QtInfoMsg:
      spdlog::log(
          spdlog::source_loc{context.file, context.line, context.function},
          spdlog::level::info, loc.constData());
      break;
    case QtWarningMsg:
      spdlog::log(
          spdlog::source_loc{context.file, context.line, context.function},
          spdlog::level::warn, loc.constData());
      sentry_capture_event(sentry_value_new_message_event(SENTRY_LEVEL_WARNING, "default", loc.constData()));
      break;
    case QtCriticalMsg:
      spdlog::log(
          spdlog::source_loc{context.file, context.line, context.function},
          spdlog::level::err, loc.constData());
      sentry_capture_event(sentry_value_new_message_event(SENTRY_LEVEL_ERROR, "default", loc.constData()));
      break;
    case QtFatalMsg:
      spdlog::log(
          spdlog::source_loc{context.file, context.line, context.function},
          spdlog::level::critical, loc.constData());
      sentry_capture_event(sentry_value_new_message_event(SENTRY_LEVEL_FATAL, "default", loc.constData()));
      break;
  }
}

namespace room_sketcher {

static Scope<QCoreApplication> createApplication(int& argc, char** argv)
{
  QCoreApplication::setApplicationName("Room Sketcher");
  QCoreApplication::setOrganizationName("Giraffe360");
  QCoreApplication::setApplicationVersion(version::getVersionString());

  for (int i = 1; i < argc; ++i) {
    if (!qstrcmp(argv[i], "-no-gui"))
      return CreateScope<QCoreApplication>(argc, argv);
  }
  return CreateScope<QApplication>(argc, argv);
}

Application::Application(int& argc, char** argv)
    : m_Application(createApplication(argc, argv)) {
  Logger::instance();

  SPDLOG_INFO("*** ************* ***");
  SPDLOG_INFO("*** Room Sketcher ***");
  SPDLOG_INFO("*** v: {} ***", version::getVersionString());
  SPDLOG_INFO("*** ************* ***\n");

  qInstallMessageHandler(qtLogMessageHandler);

  initializeSentry();
  auto sentryClose = qScopeGuard([] { sentry_close(); });

  registerQmlTypes();

  m_Engine->rootContext()->setContextProperty("settings", m_Settings.get());

  m_Engine->load(QUrl(QStringLiteral("qrc:/src/qml/main.qml")));
  if (m_Engine->rootObjects().isEmpty()) SPDLOG_WARN("Failed to load main.qml");
}

Application::~Application()
{
  m_Engine.reset();
  m_Settings.reset();
}

int Application::run() const {
  return m_Application->exec();
}

QQmlApplicationEngine* Application::qmlEngine() const {
  return m_Engine.get();
}

Settings* Application::settings() const
{
  return m_Settings.get();
}

void Application::initializeSentry() {
  sentry_options_t* options = sentry_options_new();
  sentry_options_set_symbolize_stacktraces(options, true);
  sentry_options_set_dsn(options, parameters::sentry_dsn);
  sentry_options_set_database_path(options, ".sentry-native");
  sentry_options_set_release(options, version::getVersionString());
  sentry_options_set_traces_sample_rate(options, 0.2);
  sentry_init(options);
}

void Application::registerQmlTypes() const
{
  qRegisterMetaType<Settings*>();
}

} // namespace room_sketcher
