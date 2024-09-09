#include <QtQml/qqmlextensionplugin.h>
#include <core/application.h>

Q_IMPORT_QML_PLUGIN(QuickWidgetsPlugin)

int main(int argc, char* argv[]) {
  using namespace llm_chat;
  Application app(argc, argv);
  if (app.qmlEngine()->rootObjects().isEmpty()) return 1;
  return app.run();
}
