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
  return value("language", defaultLanguage()).toString();
}

void Settings::setLanguage(const QString &language) {
  if (language == value("language", defaultLanguage()).toString()) return;

  setValue("language", language);
  emit languageChanged();
}

qreal Settings::defaultWindowOpacity() const { return 1.0; }

qreal Settings::windowOpacity() const {
  return value("windowOpacity", defaultWindowOpacity()).toReal();
}

void Settings::setWindowOpacity(const qreal opacity) {
  if (windowOpacity() == opacity) return;

  setValue("windowOpacity", opacity);
  emit windowOpacityChanged();
}

bool Settings::defaultFpsVisible() const { return false; }

bool Settings::isFpsVisible() const {
  return contains("fpsVisible") ? value("fpsVisible").toBool()
                                : defaultFpsVisible();
}

void Settings::setFpsVisible(const bool fps_visible) {
  if (fps_visible == value("fpsVisible", defaultFpsVisible()).toBool()) return;

  setValue("fpsVisible", fps_visible);
  emit fpsVisibleChanged();
}

void Settings::resetShortcutsToDefaults() {
  static const QVector<QString> all_shortcuts = {
      QLatin1String("quitShortcut"), QLatin1String("optionsShortcut"),
      QLatin1String("fullScreenShortcut")};
  for (const auto &shortcut : all_shortcuts) {
    remove(shortcut);
  }
}

QString Settings::defaultQuitShortcut() const {
  return QKeySequence(Qt::CTRL | Qt::Key_Q).toString();
}

QString Settings::quitShortcut() const {
  return value("quitShortcut", defaultQuitShortcut()).toString();
}

void Settings::setQuitShortcut(const QString &shortcut) {
  if (shortcut == value("quitShortcut", defaultQuitShortcut()).toString()) {
    return;
  }
  setValue("quitShortcut", shortcut);
  emit quitShortcutChanged();
}

QString Settings::defaultOptionsShortcut() const {
  return QKeySequence(Qt::CTRL | Qt::Key_O).toString();
}

QString Settings::optionsShortcut() const {
  return value("optionsShortcut", defaultOptionsShortcut()).toString();
}

void Settings::setOptionsShortcut(const QString &shortcut) {
  if (shortcut ==
      value("optionsShortcut", defaultOptionsShortcut()).toString()) {
    return;
  }
  setValue("optionsShortcut", shortcut);
  emit optionsShortcutChanged();
}

QString Settings::defaultFullScreenShortcut() const {
  return QKeySequence(Qt::CTRL | Qt::Key_F).toString();
}

QString Settings::fullScreenShortcut() const {
  return value("fullScreenShortcut", defaultFullScreenShortcut()).toString();
}

void Settings::setFullScreenShortcut(const QString &shortcut) {
  if (shortcut ==
      value("fullScreenShortcut", defaultFullScreenShortcut()).toString()) {
    return;
  }
  setValue("fullScreenShortcut", shortcut);
  emit fullScreenShortcutChanged();
}

}  // namespace llm_chat
