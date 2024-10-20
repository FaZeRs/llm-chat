#pragma once

#include <QSettings>
#include <QString>

class QObject;

namespace llm_chat {

/// @brief The Settings class is a wrapper around QSettings that provides a more
/// convenient interface for accessing and modifying application settings.
class Settings : public QSettings {
  Q_OBJECT

  Q_PROPERTY(
      QString language READ language WRITE setLanguage NOTIFY languageChanged)
  Q_PROPERTY(qreal windowOpacity READ windowOpacity WRITE setWindowOpacity
                 NOTIFY windowOpacityChanged)
  Q_PROPERTY(bool fpsVisible READ isFpsVisible WRITE setFpsVisible NOTIFY
                 fpsVisibleChanged)

  Q_PROPERTY(QString quitShortcut READ quitShortcut WRITE setQuitShortcut NOTIFY
                 quitShortcutChanged)
  Q_PROPERTY(QString optionsShortcut READ optionsShortcut WRITE
                 setOptionsShortcut NOTIFY optionsShortcutChanged)
  Q_PROPERTY(QString fullScreenShortcut READ fullScreenShortcut WRITE
                 setFullScreenShortcut NOTIFY fullScreenShortcutChanged)

 public:
  explicit Settings(QObject *parent = nullptr);

  /// @brief Returns the default language for the application.
  /// @return The default language.
  [[nodiscard]] QString defaultLanguage() const;
  /// @brief Returns the current language for the application.
  /// @return The current language.
  [[nodiscard]] QString language() const;
  /// @brief Sets the current language for the application.
  /// @param language The new language.
  void setLanguage(const QString &language);

  /// @brief Returns the default window opacity for the application.
  /// @return The default window opacity.
  [[nodiscard]] qreal defaultWindowOpacity() const;
  /// @brief Returns the current window opacity for the application.
  /// @return The current window opacity.
  [[nodiscard]] qreal windowOpacity() const;
  /// @brief Sets the current window opacity for the application.
  /// @param opacity The new window opacity.
  void setWindowOpacity(const qreal opacity);

  /// @brief Returns the default FPS visibility for the application.
  /// @return The default FPS visibility.
  [[nodiscard]] bool defaultFpsVisible() const;
  /// @brief Returns the current FPS visibility for the application.
  /// @return The current FPS visibility.
  [[nodiscard]] bool isFpsVisible() const;
  /// @brief Sets the current FPS visibility for the application.
  /// @param fps_visible The new FPS visibility.
  void setFpsVisible(const bool fps_visible);

  /// @brief Resets all shortcuts to their default values.
  Q_INVOKABLE void resetShortcutsToDefaults();

  /// @brief Returns the default quit shortcut for the application.
  /// @return The default quit shortcut.
  [[nodiscard]] QString defaultQuitShortcut() const;
  /// @brief Returns the current quit shortcut for the application.
  /// @return The current quit shortcut.
  [[nodiscard]] QString quitShortcut() const;
  /// @brief Sets the current quit shortcut for the application.
  /// @param shortcut The new quit shortcut.
  void setQuitShortcut(const QString &shortcut);

  /// @brief Returns the default options shortcut for the application.
  /// @return The default options shortcut.
  [[nodiscard]] QString defaultOptionsShortcut() const;
  /// @brief Returns the current options shortcut for the application.
  /// @return The current options shortcut.
  [[nodiscard]] QString optionsShortcut() const;
  /// @brief Sets the current options shortcut for the application.
  /// @param shortcut The new options shortcut.
  void setOptionsShortcut(const QString &shortcut);

  /// @brief Returns the default full screen shortcut for the application.
  /// @return The default full screen shortcut.
  [[nodiscard]] QString defaultFullScreenShortcut() const;
  /// @brief Returns the current full screen shortcut for the application.
  /// @return The current full screen shortcut.
  [[nodiscard]] QString fullScreenShortcut() const;
  /// @brief Sets the current full screen shortcut for the application.
  /// @param shortcut The new full screen shortcut.
  void setFullScreenShortcut(const QString &shortcut);

 signals:
  /// @brief Emitted when the language is changed.
  void languageChanged();
  /// @brief Emitted when the window opacity is changed.
  void windowOpacityChanged();
  /// @brief Emitted when the FPS visibility is changed.
  void fpsVisibleChanged();

  /// @brief Emitted when the quit shortcut is changed.
  void quitShortcutChanged();
  /// @brief Emitted when the options shortcut is changed.
  void optionsShortcutChanged();
  /// @brief Emitted when the full screen shortcut is changed.
  void fullScreenShortcutChanged();
};

}  // namespace llm_chat
