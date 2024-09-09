#include "application.h"

#include <config.h>
#if USE_SENTRY
#include <sentry.h>

#include "parameters.h"
#endif

#include <QFontDatabase>
#include <QQmlContext>

#if USE_SENTRY
/// @brief The Sentry message handler.
/// @param type The type of the message.
/// @param context The context of the message.
/// @param msg The message.
static void sentryMessageHandler(QtMsgType type,
                                 const QMessageLogContext& context,
                                 const QString& msg) {
  QByteArray loc = msg.toUtf8();
  switch (type) {
    case QtDebugMsg:
      QMessageLogger(context.file, context.line, context.function).debug()
          << msg;
      break;
    case QtInfoMsg:
      QMessageLogger(context.file, context.line, context.function).info()
          << msg;
      break;
    case QtWarningMsg:
      QMessageLogger(context.file, context.line, context.function).warning()
          << msg;
      sentry_capture_event(sentry_value_new_message_event(
          SENTRY_LEVEL_WARNING, "default", loc.constData()));
      break;
    case QtCriticalMsg:
      QMessageLogger(context.file, context.line, context.function).critical()
          << msg;
      sentry_capture_event(sentry_value_new_message_event(
          SENTRY_LEVEL_ERROR, "default", loc.constData()));
      break;
    case QtFatalMsg:
      QMessageLogger(context.file, context.line, context.function).fatal()
          << msg;
      sentry_capture_event(sentry_value_new_message_event(
          SENTRY_LEVEL_FATAL, "default", loc.constData()));
      break;
  }
}
#endif

namespace llm_chat {

/// @brief Creates the application.
/// @param argc The number of arguments.
/// @param argv The arguments.
/// @return The application.
static QScopedPointer<QGuiApplication> createApplication(int& argc,
                                                         char** argv) {
  QCoreApplication::setApplicationName(config::project_name);
  QCoreApplication::setOrganizationName(config::organization_name);
  QCoreApplication::setApplicationVersion(config::project_version);
  return QScopedPointer(new QGuiApplication(argc, argv));
}

Application::Application(int& argc, char** argv)
    : m_Application(createApplication(argc, argv)) {
  qSetMessagePattern(
      "[%{time h:mm:ss.zzz}] [%{type}] [t:%{threadid}] "
      "[%{function}:%{line}] %{message}");

  qInfo("*** ************* ***");
  qInfo("*** %s ***", config::project_name);
  qInfo("*** v: %s ***", config::project_version);
  qInfo("*** ************* ***\n");

#if USE_SENTRY
  initializeSentry();
  qInstallMessageHandler(sentryMessageHandler);
  auto sentryClose = qScopeGuard([] { sentry_close(); });
#endif

  registerQmlTypes();
  addFonts();

  m_Engine->rootContext()->setContextProperty("settings", m_Settings.get());
  m_Engine->rootContext()->setContextProperty("chatBackend",
                                              m_ChatBackend.get());
  m_Engine->load(QUrl(QStringLiteral("qrc:/qml/main.qml")));
  if (m_Engine->rootObjects().isEmpty()) qWarning("Failed to load main.qml");
}

int Application::run() const { return m_Application->exec(); }

QQmlApplicationEngine* Application::qmlEngine() const { return m_Engine.get(); }

Settings* Application::settings() const { return m_Settings.get(); }

void Application::initializeSentry() {
#if USE_SENTRY
  constexpr double sample_rate = 0.2;
  sentry_options_t* options = sentry_options_new();
  sentry_options_set_symbolize_stacktraces(options, true);
  sentry_options_set_dsn(options, parameters::sentry_dsn);
  sentry_options_set_database_path(options, ".sentry-native");
  sentry_options_set_release(options, config::project_version);
  sentry_options_set_traces_sample_rate(options, sample_rate);
  sentry_init(options);
#endif
}

void Application::registerQmlTypes() const {
  qRegisterMetaType<Settings*>();
  qRegisterMetaType<ChatThread*>();
}

void Application::addFonts() const {
  QFontDatabase::addApplicationFont(":/assets/fonts/font-awesome-regular.otf");
  QFontDatabase::addApplicationFont(":/assets/fonts/font-awesome-solid.otf");
  QFontDatabase::addApplicationFont(":/assets/fonts/font-awesome-brands.otf");
}

}  // namespace llm_chat
