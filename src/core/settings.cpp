#include "settings.h"

#include <QKeySequence>
#include <QLatin1String>
#include <QObject>
#include <QVariant>
#include <QVector>

namespace llm_chat {

Settings::Settings(QObject *parent) : QSettings(parent) {}

QString Settings::defaultLanguage() const { return "en_GB"; }

QString Settings::language() const {
  return contains("language") ? value("language").toString()
                              : defaultLanguage();
}

void Settings::setLanguage(const QString &language) {
  if (language == value("language", defaultLanguage()).toString()) return;

  setValue("language", language);
  emit languageChanged();
}

qreal Settings::defaultWindowOpacity() const { return 1.0; }

qreal Settings::windowOpacity() const {
  return contains("windowOpacity") ? value("windowOpacity").toReal()
                                   : defaultWindowOpacity();
}

void Settings::setWindowOpacity(qreal opacity) {
  if (windowOpacity() == opacity) return;

  setValue("windowOpacity", opacity);
  emit windowOpacityChanged();
}

bool Settings::defaultFpsVisible() const { return false; }

bool Settings::isFpsVisible() const {
  return contains("fpsVisible") ? value("fpsVisible").toBool()
                                : defaultFpsVisible();
}

void Settings::setFpsVisible(bool fpsVisible) {
  if (fpsVisible == value("fpsVisible", defaultFpsVisible()).toBool()) return;

  setValue("fpsVisible", fpsVisible);
  emit fpsVisibleChanged();
}

void Settings::resetShortcutsToDefaults() {
  static QVector<QString> allShortcuts;
  if (allShortcuts.isEmpty()) {
    allShortcuts.append(QLatin1String("newShortcut"));
    allShortcuts.append(QLatin1String("openShortcut"));
    allShortcuts.append(QLatin1String("saveShortcut"));
    allShortcuts.append(QLatin1String("saveAsShortcut"));
    allShortcuts.append(QLatin1String("quitShortcut"));
    allShortcuts.append(QLatin1String("undoShortcut"));
    allShortcuts.append(QLatin1String("redoShortcut"));
    allShortcuts.append(QLatin1String("copyShortcut"));
    allShortcuts.append(QLatin1String("cutShortcut"));
    allShortcuts.append(QLatin1String("pasteShortcut"));
    allShortcuts.append(QLatin1String("optionsShortcut"));
    allShortcuts.append(QLatin1String("fullScreenToggleShortcut"));
  }

  foreach (const QString &shortcut, allShortcuts) {
    remove(shortcut);
  }
}

#define GET_SHORTCUT(shortcutName, defaultValueFunction)         \
  return contains(shortcutName) ? value(shortcutName).toString() \
                                : defaultValueFunction();

#define SET_SHORTCUT(shortcutName, defaultValueFunction, notifySignal) \
  QVariant existingValue = value(shortcutName);                        \
  QString existingStringValue = defaultValueFunction();                \
  if (contains(shortcutName)) {                                        \
    existingStringValue = existingValue.toString();                    \
  }                                                                    \
                                                                       \
  if (shortcut == existingStringValue) return;                         \
                                                                       \
  setValue(shortcutName, shortcut);                                    \
  emit notifySignal();

QString Settings::defaultNewShortcut() const {
  return QKeySequence(QKeySequence::New).toString();
}

QString Settings::newShortcut() const {
  GET_SHORTCUT("newShortcut", defaultNewShortcut)
}

void Settings::setNewShortcut(const QString &shortcut){
    SET_SHORTCUT("newShortcut", defaultNewShortcut, newShortcutChanged)}

QString Settings::defaultQuitShortcut() const {
  return QKeySequence(QKeySequence::Quit).toString();
}

QString Settings::quitShortcut() const {
  GET_SHORTCUT("quitShortcut", defaultQuitShortcut)
}
void Settings::setQuitShortcut(const QString &shortcut){
    SET_SHORTCUT("quitShortcut", defaultQuitShortcut, quitShortcutChanged)}

QString Settings::defaultOptionsShortcut() const {
  return QKeySequence(QKeySequence::Preferences).toString();
}

QString Settings::optionsShortcut() const {
  GET_SHORTCUT("optionsShortcut", defaultOptionsShortcut)
}

void Settings::setOptionsShortcut(const QString &shortcut){
    SET_SHORTCUT("optionsShortcut", defaultOptionsShortcut,
                 optionsShortcutChanged)}

QString Settings::defaultFullScreenShortcut() const {
  return QKeySequence(QKeySequence::FullScreen).toString();
}

QString Settings::fullScreenShortcut() const {
  GET_SHORTCUT("fullScreenShortcut", defaultFullScreenShortcut)
}

void Settings::setFullScreenShortcut(const QString &shortcut) {
  SET_SHORTCUT("fullScreenShortcut", defaultFullScreenShortcut,
               fullScreenShortcutChanged)
}

}  // namespace llm_chat
